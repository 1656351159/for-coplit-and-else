from datetime import datetime
from enum import Enum

from sqlalchemy import (
    Boolean,
    Column,
    DateTime,
    Enum as SqlEnum,
    Float,
    ForeignKey,
    Integer,
    String,
    Text,
    func,
)
from sqlalchemy.orm import relationship

from .database import Base


class UserRole(str, Enum):
    STUDENT = "student"
    TECHNICIAN = "technician"
    ADMIN = "admin"


class RequestStatus(str, Enum):
    PENDING = "pending"
    APPROVED = "approved"
    REJECTED = "rejected"


class User(Base):
    __tablename__ = "users"

    id = Column(Integer, primary_key=True, index=True)
    username = Column(String(50), unique=True, nullable=False, index=True)
    hashed_password = Column(String(128), nullable=False)
    role = Column(SqlEnum(UserRole), nullable=False)
    is_active = Column(Boolean, default=True)
    created_at = Column(DateTime, server_default=func.now())

    requests = relationship("IssueRequest", back_populates="requester", cascade="all,delete")


class Category(Base):
    __tablename__ = "categories"

    id = Column(Integer, primary_key=True)
    name = Column(String(100), nullable=False)
    parent_id = Column(Integer, ForeignKey("categories.id"), nullable=True)

    parent = relationship("Category", remote_side=[id], backref="children")


class Material(Base):
    __tablename__ = "materials"

    id = Column(Integer, primary_key=True)
    name = Column(String(200), nullable=False)
    specification = Column(String(200), nullable=True)
    unit = Column(String(50), nullable=False, default="件")
    unit_price = Column(Float, nullable=True)
    category_id = Column(Integer, ForeignKey("categories.id"), nullable=False)
    description = Column(Text, nullable=True)
    safety_stock = Column(Integer, nullable=False, default=0)
    current_stock = Column(Integer, nullable=False, default=0)
    created_at = Column(DateTime, server_default=func.now())

    category = relationship("Category")
    inbound_records = relationship("InboundRecord", back_populates="material", cascade="all,delete")
    issue_requests = relationship("IssueRequest", back_populates="material")


class InboundRecord(Base):
    __tablename__ = "inbound_records"

    id = Column(Integer, primary_key=True)
    material_id = Column(Integer, ForeignKey("materials.id"), nullable=False)
    quantity = Column(Integer, nullable=False)
    supplier = Column(String(200), nullable=True)
    note = Column(Text, nullable=True)
    recorded_by_id = Column(Integer, ForeignKey("users.id"), nullable=False)
    recorded_at = Column(DateTime, default=datetime.utcnow)

    material = relationship("Material", back_populates="inbound_records")
    recorded_by = relationship("User")


class IssueRequest(Base):
    __tablename__ = "issue_requests"

    id = Column(Integer, primary_key=True)
    material_id = Column(Integer, ForeignKey("materials.id"), nullable=False)
    quantity = Column(Integer, nullable=False)
    purpose = Column(String(255), nullable=False)
    course_code = Column(String(100), nullable=True)
    requester_id = Column(Integer, ForeignKey("users.id"), nullable=False)
    status = Column(SqlEnum(RequestStatus), default=RequestStatus.PENDING, nullable=False)
    reviewer_id = Column(Integer, ForeignKey("users.id"), nullable=True)
    reviewer_comment = Column(Text, nullable=True)
    created_at = Column(DateTime, default=datetime.utcnow)
    reviewed_at = Column(DateTime, nullable=True)

    material = relationship("Material", back_populates="issue_requests")
    requester = relationship("User", back_populates="requests", foreign_keys=[requester_id])
    reviewer = relationship("User", foreign_keys=[reviewer_id])
