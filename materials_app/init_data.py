from datetime import datetime

from sqlalchemy.orm import Session

from .auth import get_password_hash
from .database import Base, SessionLocal, engine
from .models import Category, InboundRecord, Material, User, UserRole


def seed_categories(db: Session):
    existing = db.query(Category).count()
    if existing:
        return

    chemicals = Category(name="化学试剂")
    reagents = Category(name="分析试剂", parent=chemicals)
    acids = Category(name="酸类", parent=reagents)

    electronics = Category(name="电子元件")
    sensors = Category(name="传感器", parent=electronics)
    temperature = Category(name="温度传感器", parent=sensors)

    consumables = Category(name="教学耗材")
    tools = Category(name="工具", parent=consumables)
    safety = Category(name="安全防护", parent=tools)

    db.add_all([
        chemicals,
        reagents,
        acids,
        electronics,
        sensors,
        temperature,
        consumables,
        tools,
        safety,
    ])
    db.commit()


def seed_users(db: Session):
    if db.query(User).count():
        return

    users = [
        User(
            username="admin",
            role=UserRole.ADMIN,
            hashed_password=get_password_hash("123456"),
            is_active=True,
        ),
        User(
            username="lab",
            role=UserRole.TECHNICIAN,
            hashed_password=get_password_hash("123456"),
            is_active=True,
        ),
        User(
            username="stu",
            role=UserRole.STUDENT,
            hashed_password=get_password_hash("123456"),
            is_active=True,
        ),
    ]
    db.add_all(users)
    db.commit()


def seed_materials(db: Session):
    if db.query(Material).count():
        return

    categories = {c.name: c for c in db.query(Category).all()}
    materials = [
        Material(
            name="无水乙醇",
            specification="AR 500mL",
            unit="瓶",
            unit_price=38.5,
            category_id=categories["酸类"].id,
            description="实验清洗及萃取用",
            safety_stock=5,
            current_stock=20,
        ),
        Material(
            name="PT100 温度探头",
            specification="0-200℃",
            unit="支",
            unit_price=120.0,
            category_id=categories["温度传感器"].id,
            description="实验炉温监控",
            safety_stock=2,
            current_stock=6,
        ),
        Material(
            name="防割手套",
            specification="L 号",
            unit="副",
            unit_price=25.0,
            category_id=categories["安全防护"].id,
            description="实验安全装备",
            safety_stock=10,
            current_stock=15,
        ),
    ]
    db.add_all(materials)
    db.commit()

    admin_user = db.query(User).filter(User.username == "lab").first()
    for material in materials:
        record = InboundRecord(
            material_id=material.id,
            quantity=material.current_stock,
            supplier="示例供应商",
            note="初始化库存",
            recorded_by_id=admin_user.id,
            recorded_at=datetime.utcnow(),
        )
        db.add(record)
    db.commit()


def init_db():
    Base.metadata.create_all(bind=engine)
    db = SessionLocal()
    try:
        seed_categories(db)
        seed_users(db)
        seed_materials(db)
    finally:
        db.close()
