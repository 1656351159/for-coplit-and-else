from datetime import datetime
from io import StringIO
from typing import List, Optional

from fastapi import Depends, FastAPI, HTTPException, Query
from fastapi.middleware.cors import CORSMiddleware
from fastapi.responses import StreamingResponse
from fastapi.security import OAuth2PasswordRequestForm
from sqlalchemy import or_
from sqlalchemy.orm import Session

from . import schemas
from .auth import get_current_user, login, require_roles
from .database import get_db
from .init_data import init_db
from .models import Category, InboundRecord, IssueRequest, Material, RequestStatus, User, UserRole

app = FastAPI(title="材智通——智慧材料库数字化管理系统")
app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)


@app.on_event("startup")
def startup_event():
    init_db()


@app.post("/auth/login", response_model=schemas.Token, tags=["auth"])
async def login_route(form_data: OAuth2PasswordRequestForm = Depends()):
    return await login(form_data)


@app.get("/auth/me", response_model=schemas.UserOut, tags=["auth"])
def read_me(current_user: User = Depends(get_current_user)):
    return current_user


# User management -------------------------------------------------------------
@app.get(
    "/users",
    response_model=List[schemas.UserOut],
    dependencies=[Depends(require_roles(UserRole.ADMIN))],
)
def list_users(include_disabled: bool = False, db: Session = Depends(get_db)):
    query = db.query(User)
    if not include_disabled:
        query = query.filter(User.is_active.is_(True))
    return query.order_by(User.created_at).all()


@app.post(
    "/users",
    response_model=schemas.UserOut,
    dependencies=[Depends(require_roles(UserRole.ADMIN))],
)
def create_user(payload: schemas.UserCreate, db: Session = Depends(get_db)):
    if db.query(User).filter(User.username == payload.username).first():
        raise HTTPException(status_code=400, detail="用户名已存在")
    from .auth import get_password_hash

    user = User(
        username=payload.username,
        hashed_password=get_password_hash(payload.password),
        role=payload.role,
        is_active=payload.is_active,
    )
    db.add(user)
    db.commit()
    db.refresh(user)
    return user


@app.patch(
    "/users/{user_id}",
    response_model=schemas.UserOut,
    dependencies=[Depends(require_roles(UserRole.ADMIN))],
)
def update_user(user_id: int, payload: schemas.UserUpdate, db: Session = Depends(get_db)):
    user = db.query(User).get(user_id)
    if not user:
        raise HTTPException(status_code=404, detail="用户不存在")
    from .auth import get_password_hash

    if payload.password:
        user.hashed_password = get_password_hash(payload.password)
    if payload.role:
        user.role = payload.role
    if payload.is_active is not None:
        user.is_active = payload.is_active
    db.commit()
    db.refresh(user)
    return user


# Category management ---------------------------------------------------------
@app.get(
    "/categories",
    response_model=List[schemas.CategoryOut],
    dependencies=[Depends(require_roles(UserRole.STUDENT, UserRole.TECHNICIAN, UserRole.ADMIN))],
)
def list_categories(db: Session = Depends(get_db)):
    categories = db.query(Category).all()
    roots = [c for c in categories if c.parent_id is None]
    return roots


@app.post(
    "/categories",
    response_model=schemas.CategoryOut,
    dependencies=[Depends(require_roles(UserRole.ADMIN))],
)
def create_category(payload: schemas.CategoryCreate, db: Session = Depends(get_db)):
    category = Category(name=payload.name, parent_id=payload.parent_id)
    db.add(category)
    db.commit()
    db.refresh(category)
    return category


@app.put(
    "/categories/{category_id}",
    response_model=schemas.CategoryOut,
    dependencies=[Depends(require_roles(UserRole.ADMIN))],
)
def update_category(category_id: int, payload: schemas.CategoryUpdate, db: Session = Depends(get_db)):
    category = db.query(Category).get(category_id)
    if not category:
        raise HTTPException(status_code=404, detail="类目不存在")
    category.name = payload.name
    category.parent_id = payload.parent_id
    db.commit()
    db.refresh(category)
    return category


# Material management ---------------------------------------------------------
@app.get(
    "/materials",
    response_model=List[schemas.MaterialOut],
    dependencies=[Depends(require_roles(UserRole.STUDENT, UserRole.TECHNICIAN, UserRole.ADMIN))],
)
def list_materials(
    keyword: Optional[str] = Query(default=None, description="名称或关键字"),
    category_id: Optional[int] = None,
    skip: int = 0,
    limit: int = Query(default=20, le=200),
    db: Session = Depends(get_db),
):
    query = db.query(Material)
    if keyword:
        like = f"%{keyword}%"
        query = query.filter(or_(Material.name.like(like), Material.description.like(like)))
    if category_id:
        query = query.filter(Material.category_id == category_id)
    materials = query.offset(skip).limit(limit).all()
    return materials


@app.post(
    "/materials",
    response_model=schemas.MaterialOut,
    dependencies=[Depends(require_roles(UserRole.ADMIN))],
)
def create_material(payload: schemas.MaterialCreate, db: Session = Depends(get_db)):
    material = Material(**payload.dict())
    db.add(material)
    db.commit()
    db.refresh(material)
    return material


@app.put(
    "/materials/{material_id}",
    response_model=schemas.MaterialOut,
    dependencies=[Depends(require_roles(UserRole.ADMIN))],
)
def update_material(material_id: int, payload: schemas.MaterialUpdate, db: Session = Depends(get_db)):
    material = db.query(Material).get(material_id)
    if not material:
        raise HTTPException(status_code=404, detail="材料不存在")
    for field, value in payload.dict(exclude_unset=True).items():
        setattr(material, field, value)
    db.commit()
    db.refresh(material)
    return material


# Inbound management ----------------------------------------------------------
@app.post(
    "/inventory/receipts",
    response_model=schemas.InboundRecordOut,
)
def create_inbound_record(
    payload: schemas.InboundRecordCreate,
    current_user: User = Depends(require_roles(UserRole.TECHNICIAN, UserRole.ADMIN)),
    db: Session = Depends(get_db),
):
    material = db.query(Material).get(payload.material_id)
    if not material:
        raise HTTPException(status_code=404, detail="材料不存在")
    record = InboundRecord(
        material_id=payload.material_id,
        quantity=payload.quantity,
        supplier=payload.supplier,
        note=payload.note,
        recorded_by_id=current_user.id,
        recorded_at=payload.recorded_at or datetime.utcnow(),
    )
    material.current_stock += payload.quantity
    db.add(record)
    db.commit()
    db.refresh(record)
    return record


@app.get(
    "/inventory/receipts",
    response_model=List[schemas.InboundRecordOut],
    dependencies=[Depends(require_roles(UserRole.TECHNICIAN, UserRole.ADMIN))],
)
def list_inbound_records(
    start: Optional[datetime] = None,
    end: Optional[datetime] = None,
    material_id: Optional[int] = None,
    db: Session = Depends(get_db),
):
    query = db.query(InboundRecord)
    if start:
        query = query.filter(InboundRecord.recorded_at >= start)
    if end:
        query = query.filter(InboundRecord.recorded_at <= end)
    if material_id:
        query = query.filter(InboundRecord.material_id == material_id)
    return query.order_by(InboundRecord.recorded_at.desc()).all()


# Issue / request management --------------------------------------------------
@app.post(
    "/requests",
    response_model=schemas.IssueRequestOut,
)
def create_request(
    payload: schemas.IssueRequestCreate,
    current_user: User = Depends(require_roles(UserRole.STUDENT, UserRole.TECHNICIAN, UserRole.ADMIN)),
    db: Session = Depends(get_db),
):
    material = db.query(Material).get(payload.material_id)
    if not material:
        raise HTTPException(status_code=404, detail="材料不存在")
    if payload.quantity <= 0:
        raise HTTPException(status_code=400, detail="领用数量必须大于0")
    request = IssueRequest(
        material_id=payload.material_id,
        quantity=payload.quantity,
        purpose=payload.purpose,
        course_code=payload.course_code,
        requester_id=current_user.id,
    )
    db.add(request)
    db.commit()
    db.refresh(request)
    return request


@app.get(
    "/requests/me",
    response_model=List[schemas.IssueRequestOut],
)
def list_my_requests(
    current_user: User = Depends(require_roles(UserRole.STUDENT, UserRole.TECHNICIAN, UserRole.ADMIN)),
    db: Session = Depends(get_db),
):
    return (
        db.query(IssueRequest)
            .filter(IssueRequest.requester_id == current_user.id)
            .order_by(IssueRequest.created_at.desc())
            .all()
    )


@app.get(
    "/requests",
    response_model=List[schemas.IssueRequestOut],
    dependencies=[Depends(require_roles(UserRole.TECHNICIAN, UserRole.ADMIN))],
)
def list_requests(
    status: Optional[RequestStatus] = None, db: Session = Depends(get_db)
):
    query = db.query(IssueRequest)
    if status:
        query = query.filter(IssueRequest.status == status)
    return query.order_by(IssueRequest.created_at.desc()).all()


@app.post(
    "/requests/{request_id}/review",
    response_model=schemas.IssueRequestOut,
)
def review_request(
    request_id: int,
    payload: schemas.IssueReview,
    current_user: User = Depends(require_roles(UserRole.TECHNICIAN, UserRole.ADMIN)),
    db: Session = Depends(get_db),
):
    request = db.query(IssueRequest).get(request_id)
    if not request:
        raise HTTPException(status_code=404, detail="记录不存在")
    if request.status != RequestStatus.PENDING:
        raise HTTPException(status_code=400, detail="该申请已处理")
    material = request.material
    if payload.status == RequestStatus.APPROVED:
        if material.current_stock < request.quantity:
            raise HTTPException(status_code=400, detail="库存不足，无法批准")
        material.current_stock -= request.quantity
        request.status = RequestStatus.APPROVED
        request.reviewed_at = datetime.utcnow()
        request.reviewer_id = current_user.id
        request.reviewer_comment = payload.reviewer_comment
    elif payload.status == RequestStatus.REJECTED:
        request.status = RequestStatus.REJECTED
        request.reviewed_at = datetime.utcnow()
        request.reviewer_id = current_user.id
        request.reviewer_comment = payload.reviewer_comment
    else:
        raise HTTPException(status_code=400, detail="无效的审批状态")
    db.commit()
    db.refresh(request)
    return request


# Inventory overview ---------------------------------------------------------
@app.get(
    "/inventory/overview",
    response_model=List[schemas.InventoryItem],
    dependencies=[Depends(require_roles(UserRole.TECHNICIAN, UserRole.ADMIN))],
)
def inventory_overview(keyword: Optional[str] = None, db: Session = Depends(get_db)):
    query = db.query(Material)
    if keyword:
        like = f"%{keyword}%"
        query = query.filter(Material.name.like(like))
    materials = query.order_by(Material.name).all()
    items = []
    for material in materials:
        is_low = material.safety_stock > 0 and material.current_stock < material.safety_stock
        items.append(schemas.InventoryItem(material=material, is_below_threshold=is_low))
    return items


# Reports --------------------------------------------------------------------
@app.get(
    "/reports/issues",
    dependencies=[Depends(require_roles(UserRole.ADMIN))],
)
def export_issue_report(
    start: Optional[datetime] = None,
    end: Optional[datetime] = None,
    db: Session = Depends(get_db),
):
    query = db.query(IssueRequest).filter(IssueRequest.status == RequestStatus.APPROVED)
    if start:
        query = query.filter(IssueRequest.created_at >= start)
    if end:
        query = query.filter(IssueRequest.created_at <= end)
    rows = query.order_by(IssueRequest.created_at).all()

    buffer = StringIO()
    buffer.write("材料名称,规格型号,数量,单位,使用人,用途,时间\n")
    for row in rows:
        buffer.write(
            f"{row.material.name},{row.material.specification or ''},{row.quantity},{row.material.unit},{row.requester.username},{row.purpose},{row.reviewed_at or row.created_at}\n"
        )
    buffer.seek(0)
    headers = {
        "Content-Disposition": "attachment; filename=issue_report.csv",
        "Content-Type": "text/csv; charset=utf-8",
    }
    return StreamingResponse(buffer, headers=headers)


@app.get(
    "/reports/inventory",
    dependencies=[Depends(require_roles(UserRole.ADMIN))],
)
def export_inventory_report(db: Session = Depends(get_db)):
    materials = db.query(Material).order_by(Material.name).all()
    buffer = StringIO()
    buffer.write("材料名称,规格型号,单位,当前库存,安全库存\n")
    for material in materials:
        buffer.write(
            f"{material.name},{material.specification or ''},{material.unit},{material.current_stock},{material.safety_stock}\n"
        )
    buffer.seek(0)
    headers = {
        "Content-Disposition": "attachment; filename=inventory_report.csv",
        "Content-Type": "text/csv; charset=utf-8",
    }
    return StreamingResponse(buffer, headers=headers)


@app.get("/")
def root():
    return {
        "message": "材智通样机已运行，访问 /docs 查看交互式接口文档。",
        "demo_accounts": {
            "admin": "123456",
            "lab": "123456",
            "stu": "123456",
        },
    }
