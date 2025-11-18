from datetime import datetime
from typing import List, Optional

from pydantic import BaseModel, Field

from .models import RequestStatus, UserRole


class Token(BaseModel):
    access_token: str
    token_type: str = "bearer"
    user: "UserOut"


class TokenData(BaseModel):
    username: Optional[str] = None


class UserBase(BaseModel):
    username: str
    role: UserRole
    is_active: bool


class UserCreate(BaseModel):
    username: str = Field(..., min_length=3)
    password: str = Field(..., min_length=6)
    role: UserRole
    is_active: bool = True


class UserUpdate(BaseModel):
    password: Optional[str] = Field(default=None, min_length=6)
    role: Optional[UserRole] = None
    is_active: Optional[bool] = None


class UserOut(UserBase):
    id: int
    created_at: datetime

    class Config:
        orm_mode = True


Token.update_forward_refs()


class CategoryBase(BaseModel):
    name: str
    parent_id: Optional[int] = Field(default=None, description="上级类目 ID")


class CategoryCreate(CategoryBase):
    pass


class CategoryUpdate(CategoryBase):
    pass


class CategoryOut(CategoryBase):
    id: int
    children: List["CategoryOut"] = Field(default_factory=list)

    class Config:
        orm_mode = True


class MaterialBase(BaseModel):
    name: str
    specification: Optional[str] = None
    unit: str
    unit_price: Optional[float] = None
    category_id: int
    description: Optional[str] = None
    safety_stock: int = 0


class MaterialCreate(MaterialBase):
    pass


class MaterialUpdate(BaseModel):
    name: Optional[str] = None
    specification: Optional[str] = None
    unit: Optional[str] = None
    unit_price: Optional[float] = None
    category_id: Optional[int] = None
    description: Optional[str] = None
    safety_stock: Optional[int] = None
    current_stock: Optional[int] = None


class MaterialOut(MaterialBase):
    id: int
    current_stock: int
    category: Optional[CategoryOut]

    class Config:
        orm_mode = True


class InboundRecordCreate(BaseModel):
    material_id: int
    quantity: int = Field(..., gt=0)
    supplier: Optional[str] = None
    note: Optional[str] = None
    recorded_at: Optional[datetime] = None


class InboundRecordOut(BaseModel):
    id: int
    material: MaterialOut
    quantity: int
    supplier: Optional[str]
    note: Optional[str]
    recorded_by: UserOut
    recorded_at: datetime

    class Config:
        orm_mode = True


class IssueRequestCreate(BaseModel):
    material_id: int
    quantity: int = Field(..., gt=0)
    purpose: str
    course_code: Optional[str] = None


class IssueReview(BaseModel):
    status: RequestStatus
    reviewer_comment: Optional[str] = None


class IssueRequestOut(BaseModel):
    id: int
    material: MaterialOut
    quantity: int
    purpose: str
    course_code: Optional[str]
    requester: UserOut
    status: RequestStatus
    reviewer: Optional[UserOut]
    reviewer_comment: Optional[str]
    created_at: datetime
    reviewed_at: Optional[datetime]

    class Config:
        orm_mode = True


class InventoryItem(BaseModel):
    material: MaterialOut
    is_below_threshold: bool


class InventoryExportRow(BaseModel):
    material_name: str
    specification: Optional[str]
    unit: str
    current_stock: int
    safety_stock: int

CategoryOut.update_forward_refs()
