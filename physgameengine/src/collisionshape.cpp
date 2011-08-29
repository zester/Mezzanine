//� Copyright 2010 - 2011 BlackTopp Studios Inc.
/* This file is part of The PhysGame Engine.

    The PhysGame Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The PhysGame Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The PhysGame Engine.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'Docs' folder. See 'gpl.txt'
*/
/* We welcome the use of The PhysGame anyone, including companies who wish to
   Build professional software and charge for their product.

   However there are some practical restrictions, so if your project involves
   any of the following you should contact us and we will try to work something
   out:
    - DRM or Copy Protection of any kind(except Copyrights)
    - Software Patents You Do Not Wish to Freely License
    - Any Kind of Linking to Non-GPL licensed Works
    - Are Currently In Violation of Another Copyright Holder's GPL License
    - If You want to change our code and not add a few hundred MB of stuff to
        your distribution

   These and other limitations could cause serious legal problems if you ignore
   them, so it is best to simply contact us or the Free Software Foundation, if
   you have any questions.

   Joseph Toppi - toppij@gmail.com
   John Blackwood - makoenergy02@gmail.com
*/
#ifndef _collisionshape_cpp
#define _collisionshape_cpp

#include "collisionshape.h"
#include "world.h"

#include "btBulletDynamicsCommon.h"
#include "BulletCollision/Gimpact/btGImpactShape.h"
#include "BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h"
#include "BulletSoftBody/btSoftBodyInternals.h"

namespace phys
{
    /////////////////////////////////////////
    // CollisionShape Functions

    CollisionShape::CollisionShape()
        : ShapeBase(NULL)
    {
    }

    CollisionShape::~CollisionShape()
    {
    }

    const String& CollisionShape::GetName()
    {
        return Name;
    }

    void CollisionShape::SetMargin(const Real& Margin)
    {
        ShapeBase->setMargin(Margin);
    }

    Real CollisionShape::GetMargin() const
    {
        return ShapeBase->getMargin();
    }

    void CollisionShape::SetScaling(const Vector3& Scaling)
    {
        ShapeBase->setLocalScaling(Scaling.GetBulletVector3());
    }

    Vector3 CollisionShape::GetScaling() const
    {
        Vector3 Scaling(ShapeBase->getLocalScaling());
        return Scaling;
    }

    btCollisionShape* CollisionShape::GetBulletShape() const
    {
        return ShapeBase;
    }

    /////////////////////////////////////////
    // PrimitiveCollisionShape Functions

    PrimitiveCollisionShape::PrimitiveCollisionShape()
        : PrimitiveShapeBase(NULL)
    {
    }

    PrimitiveCollisionShape::~PrimitiveCollisionShape()
    {
    }

    void PrimitiveCollisionShape::SetPointers(btConvexShape* Shape)
    {
        PrimitiveShapeBase = Shape;
        ShapeBase = Shape;
    }

    /////////////////////////////////////////
    // MeshCollisionShape Functions

    MeshCollisionShape::MeshCollisionShape()
        : MeshShapeBase(NULL)
    {
    }

    MeshCollisionShape::~MeshCollisionShape()
    {
    }

    void MeshCollisionShape::SetPointers(btConcaveShape* Shape)
    {
        MeshShapeBase = Shape;
        ShapeBase = Shape;
    }

    /////////////////////////////////////////
    // CompoundCollisionShape Functions

    CompoundCollisionShape::CompoundCollisionShape(const String& Name)
    {
        this->Name = Name;
        CompoundShape = new btCompoundShape(false);
        ShapeBase = CompoundShape;
    }

    CompoundCollisionShape::CompoundCollisionShape(const String& Name, btCompoundShape* BulletShape)
    {
        this->Name = Name;
        CompoundShape = BulletShape;
        ShapeBase = CompoundShape;

        Whole NumChildren = CompoundShape->getNumChildShapes();
        for( Whole X = 0 ; X < NumChildren ; X++ )
        {
            btCollisionShape* CurrChild = CompoundShape->getChildShape(X);
            std::stringstream namestream;
            namestream << this->Name << "Child" << X;
            String ChildType(CurrChild->getName());
            switch(CurrChild->getShapeType())
            {
                case BOX_SHAPE_PROXYTYPE:
                {
                    BoxCollisionShape* BoxShape = new BoxCollisionShape(namestream.str(),(btBoxShape*)CurrChild);
                    ChildShapes.push_back(BoxShape);
                    break;
                }
                case CAPSULE_SHAPE_PROXYTYPE:
                {
                    CapsuleCollisionShape* CapsuleShape = new CapsuleCollisionShape(namestream.str(),(btCapsuleShape*)CurrChild);
                    ChildShapes.push_back(CapsuleShape);
                    break;
                }
                case CONE_SHAPE_PROXYTYPE:
                {
                    ConeCollisionShape* ConeShape = new ConeCollisionShape(namestream.str(),(btConeShape*)CurrChild);
                    ChildShapes.push_back(ConeShape);
                    break;
                }
                case CONVEX_HULL_SHAPE_PROXYTYPE:
                {
                    ConvexHullCollisionShape* ConvexHullShape = new ConvexHullCollisionShape(namestream.str(),(btConvexHullShape*)CurrChild);
                    ChildShapes.push_back(ConvexHullShape);
                    break;
                }
                case CYLINDER_SHAPE_PROXYTYPE:
                {
                    CylinderCollisionShape* CylinderShape = new CylinderCollisionShape(namestream.str(),(btCylinderShape*)CurrChild);
                    ChildShapes.push_back(CylinderShape);
                    break;
                }
                case MULTI_SPHERE_SHAPE_PROXYTYPE:
                {
                    MultiSphereCollisionShape* MultiSphereShape = new MultiSphereCollisionShape(namestream.str(),(btMultiSphereShape*)CurrChild);
                    ChildShapes.push_back(MultiSphereShape);
                    break;
                }
                case SPHERE_SHAPE_PROXYTYPE:
                {
                    SphereCollisionShape* SphereShape = new SphereCollisionShape(namestream.str(),(btSphereShape*)CurrChild);
                    ChildShapes.push_back(SphereShape);
                    break;
                }
                case GIMPACT_SHAPE_PROXYTYPE:
                {
                    DynamicMeshCollisionShape* GImpactShape = new DynamicMeshCollisionShape(namestream.str(),(btGImpactMeshShape*)CurrChild);
                    ChildShapes.push_back(GImpactShape);
                    break;
                }
                case TERRAIN_SHAPE_PROXYTYPE:
                {
                    HeightfieldCollisionShape* HeightFieldShape = new HeightfieldCollisionShape(namestream.str(),(btHeightfieldTerrainShape*)CurrChild);
                    ChildShapes.push_back(HeightFieldShape);
                    break;
                }
                case STATIC_PLANE_PROXYTYPE:
                {
                    PlaneCollisionShape* PlaneShape = new PlaneCollisionShape(namestream.str(),(btStaticPlaneShape*)CurrChild);
                    ChildShapes.push_back(PlaneShape);
                    break;
                }
                case SOFTBODY_SHAPE_PROXYTYPE:
                {
                    ActorSoftCollisionShape* SoftBodyShape = new ActorSoftCollisionShape(namestream.str(),(btSoftBodyCollisionShape*)CurrChild);
                    ChildShapes.push_back(SoftBodyShape);
                    break;
                }
                case TRIANGLE_MESH_SHAPE_PROXYTYPE:
                {
                    StaticMeshCollisionShape* BvhShape = new StaticMeshCollisionShape(namestream.str(),(btBvhTriangleMeshShape*)CurrChild);
                    ChildShapes.push_back(BvhShape);
                    break;
                }
                case COMPOUND_SHAPE_PROXYTYPE: // holy recursive batman
                {
                    CompoundCollisionShape* Compound = new CompoundCollisionShape(namestream.str(),(btCompoundShape*)CurrChild);
                    ChildShapes.push_back(Compound);
                    break;
                }
                default:
                {
                    World::GetWorldPointer()->LogAndThrow(Exception("Attempting to add an unsupported/unwrapped Collision Shape in CompoundShape constructor."));
                }
            }
        }
    }

    CompoundCollisionShape::~CompoundCollisionShape()
    {
        for( Whole X = 0 ; X < ChildShapes.size() ; X++ )
        {
            //CompoundShape->removeChildShapeByIndex(X);
            delete ChildShapes[X];
        }
        ChildShapes.clear();
        delete CompoundShape;
    }

    void CompoundCollisionShape::AddChildShape(CollisionShape* Child, const Vector3& ChildLocation, const Quaternion& ChildRotation)
    {
        btTransform ChildTrans(ChildRotation.GetBulletQuaternion(),ChildLocation.GetBulletVector3());
        CompoundShape->addChildShape(ChildTrans,Child->GetBulletShape());
        ChildShapes.push_back(Child);
    }

    void CompoundCollisionShape::AddChildShape(CollisionShape* Child, const Vector3& ChildLocation)
    {
        btTransform ChildTrans;
        ChildTrans.setIdentity();
        ChildTrans.setOrigin(ChildLocation.GetBulletVector3());
        CompoundShape->addChildShape(ChildTrans,Child->GetBulletShape());
        ChildShapes.push_back(Child);
    }

    Whole CompoundCollisionShape::GetNumChildren()
    {
        return ChildShapes.size();
    }

    CollisionShape::ShapeType CompoundCollisionShape::GetType() const
    {
        return CollisionShape::ST_Compound;
    }

    /////////////////////////////////////////
    // BoxCollisionShape Functions

    BoxCollisionShape::BoxCollisionShape(const String& Name, const Vector3& HalfExtents)
    {
        this->Name = Name;
        BoxShape = new btBoxShape(HalfExtents.GetBulletVector3());
        SetPointers(BoxShape);
    }

    BoxCollisionShape::BoxCollisionShape(const String& Name, btBoxShape* BulletShape)
    {
        this->Name = Name;
        BoxShape = BulletShape;
        SetPointers(BoxShape);
    }

    BoxCollisionShape::~BoxCollisionShape()
    {
        delete BoxShape;
    }

    Vector3 BoxCollisionShape::GetHalfExtents() const
    {
        Vector3 HalfExtents(BoxShape->getHalfExtentsWithoutMargin());
        return HalfExtents;
    }

    Vector3 BoxCollisionShape::GetHalfExtentsWithMargin() const
    {
        Vector3 HalfExtents(BoxShape->getHalfExtentsWithMargin());
        return HalfExtents;
    }

    bool BoxCollisionShape::IsInside(const Vector3& Location, const Real& Tolorance) const
    {
        return BoxShape->isInside(Location.GetBulletVector3(),Tolorance);
    }

    CollisionShape::ShapeType BoxCollisionShape::GetType() const
    {
        return CollisionShape::ST_Box;
    }

    /////////////////////////////////////////
    // CapsuleCollisionShape Functions

    CapsuleCollisionShape::CapsuleCollisionShape(const String& Name, const Real& Radius, const Real& Height, const Vector3& UpAxis)
    {
        this->Name = Name;
        if(Vector3::Unit_Y() == UpAxis) CapsuleShape = new btCapsuleShape(Radius,Height);
        else if(Vector3::Unit_X() == UpAxis) CapsuleShape = new btCapsuleShapeX(Radius,Height);
        else if(Vector3::Unit_Z() == UpAxis) CapsuleShape = new btCapsuleShapeZ(Radius,Height);
        else World::GetWorldPointer()->LogAndThrow(Exception("Non-supported up Axis passed into CapsuleCollisionShape constructor."));
        SetPointers(CapsuleShape);
    }

    CapsuleCollisionShape::CapsuleCollisionShape(const String& Name, btCapsuleShape* BulletShape)
    {
        this->Name = Name;
        CapsuleShape = BulletShape;
        SetPointers(CapsuleShape);
    }

    CapsuleCollisionShape::~CapsuleCollisionShape()
    {
        delete CapsuleShape;
    }

    Real CapsuleCollisionShape::GetRadius() const
    {
        return CapsuleShape->getRadius();
    }

    Real CapsuleCollisionShape::GetHeight() const
    {
        return (CapsuleShape->getHalfHeight() * 2.0);
    }

    Vector3 CapsuleCollisionShape::GetUpAxis() const
    {
        switch(CapsuleShape->getUpAxis())
        {
            case 0: return Vector3::Unit_X();
            case 1: return Vector3::Unit_Y();
            case 2: return Vector3::Unit_Z();
        }
    }

    CollisionShape::ShapeType CapsuleCollisionShape::GetType() const
    {
        return CollisionShape::ST_Capsule;
    }

    /////////////////////////////////////////
    // ConeCollisionShape Functions

    ConeCollisionShape::ConeCollisionShape(const String& Name, const Real& Radius, const Real& Height, const Vector3& UpAxis)
    {
        this->Name = Name;
        if(Vector3::Unit_Y() == UpAxis) ConeShape = new btConeShape(Radius,Height);
        else if(Vector3::Unit_X() == UpAxis) ConeShape = new btConeShapeX(Radius,Height);
        else if(Vector3::Unit_Z() == UpAxis) ConeShape = new btConeShapeZ(Radius,Height);
        else World::GetWorldPointer()->LogAndThrow(Exception("Non-supported up Axis passed into ConeCollisionShape constructor."));
        SetPointers(ConeShape);
    }

    ConeCollisionShape::ConeCollisionShape(const String& Name, btConeShape* BulletShape)
    {
        this->Name = Name;
        ConeShape = BulletShape;
        SetPointers(ConeShape);
    }

    ConeCollisionShape::~ConeCollisionShape()
    {
        delete ConeShape;
    }

    Real ConeCollisionShape::GetRadius() const
    {
        return ConeShape->getRadius();
    }

    Real ConeCollisionShape::GetHeight() const
    {
        return ConeShape->getHeight();
    }

    Vector3 ConeCollisionShape::GetUpAxis() const
    {
        switch(ConeShape->getConeUpIndex())
        {
            case 0: return Vector3::Unit_X();
            case 1: return Vector3::Unit_Y();
            case 2: return Vector3::Unit_Z();
        }
    }

    CollisionShape::ShapeType ConeCollisionShape::GetType() const
    {
        return CollisionShape::ST_Cone;
    }

    /////////////////////////////////////////
    // ConvexHullCollisionShape Functions

    ConvexHullCollisionShape::ConvexHullCollisionShape(const String& Name, const std::vector<Vector3>& Points)
    {
        btScalar* BulletPoints = new btScalar[Points.size() * 3];
        for( Whole X = 0 ; X < Points.size() ; X+=3 )
        {
            btVector3 Point = Points[X].GetBulletVector3();
            BulletPoints[X] = Point.getX();
            BulletPoints[X+1] = Point.getY();
            BulletPoints[X+2] = Point.getZ();
        }

        this->Name = Name;
        ConvexHullShape = new btConvexHullShape(BulletPoints,Points.size());
        SetPointers(ConvexHullShape);
        delete BulletPoints;
    }

    ConvexHullCollisionShape::ConvexHullCollisionShape(const String& Name, btConvexHullShape* BulletShape)
    {
        this->Name = Name;
        ConvexHullShape = BulletShape;
        SetPointers(ConvexHullShape);
    }

    ConvexHullCollisionShape::~ConvexHullCollisionShape()
    {
        delete ConvexHullShape;
    }

    void ConvexHullCollisionShape::AddPoint(const Vector3& Point)
    {
        ConvexHullShape->addPoint(Point.GetBulletVector3());
    }

    Vector3 ConvexHullCollisionShape::GetPoint(const Whole& Index) const
    {
        Vector3 Point(ConvexHullShape->getScaledPoint(Index));
        return Point;
    }

    Whole ConvexHullCollisionShape::GetNumPoints() const
    {
        return ConvexHullShape->getNumPoints();
    }

    bool ConvexHullCollisionShape::IsInside(const Vector3& Location, const Real& Tolorance) const
    {
        return ConvexHullShape->isInside(Location.GetBulletVector3(),Tolorance);
    }

    CollisionShape::ShapeType ConvexHullCollisionShape::GetType() const
    {
        return CollisionShape::ST_ConvexHull;
    }

    /////////////////////////////////////////
    // CylinderCollisionShape Functions

    CylinderCollisionShape::CylinderCollisionShape(const String& Name, const Real& Radius, const Real& Height, const Vector3& UpAxis)
    {
        this->Name = Name;
        if(Vector3::Unit_Y() == UpAxis) CylinderShape = new btCylinderShape(btVector3(Radius,Height,Radius));
        else if(Vector3::Unit_X() == UpAxis) CylinderShape = new btCylinderShapeX(btVector3(Radius,Height,Radius));
        else if(Vector3::Unit_Z() == UpAxis) CylinderShape = new btCylinderShapeZ(btVector3(Radius,Height,Radius));
        else World::GetWorldPointer()->LogAndThrow(Exception("Non-supported up Axis passed into CylinderCollisionShape constructor."));
        SetPointers(CylinderShape);
    }

    CylinderCollisionShape::CylinderCollisionShape(const String& Name, btCylinderShape* BulletShape)
    {
        this->Name = Name;
        CylinderShape = BulletShape;
        SetPointers(CylinderShape);
    }

    CylinderCollisionShape::~CylinderCollisionShape()
    {
        delete CylinderShape;
    }

    Vector3 CylinderCollisionShape::GetHalfExtents() const
    {
        Vector3 HalfExtents(CylinderShape->getHalfExtentsWithoutMargin());
        return HalfExtents;
    }

    Vector3 CylinderCollisionShape::GetHalfExtentsWithMargin() const
    {
        Vector3 HalfExtents(CylinderShape->getHalfExtentsWithMargin());
        return HalfExtents;
    }

    Real CylinderCollisionShape::GetRadius() const
    {
        return CylinderShape->getRadius();
    }

    Vector3 CylinderCollisionShape::GetUpAxis() const
    {
        switch(CylinderShape->getUpAxis())
        {
            case 0: return Vector3::Unit_X();
            case 1: return Vector3::Unit_Y();
            case 2: return Vector3::Unit_Z();
        }
    }

    CollisionShape::ShapeType CylinderCollisionShape::GetType() const
    {
        return CollisionShape::ST_Cylinder;
    }

    /////////////////////////////////////////
    // MultiSphereCollisionShape Functions

    MultiSphereCollisionShape::MultiSphereCollisionShape(const String& Name, const std::vector<Vector3>& Locations, const std::vector<Real>& Radii)
    {
        if(Locations.size() != Radii.size())
            World::GetWorldPointer()->LogAndThrow(Exception("Vector size mismatch between Locations and Radii in MultiSphereCollisionShape constructor."));
        this->Name = Name;
        Whole Spheres = Locations.size();
        btVector3* BulletLocs = new btVector3[Spheres];
        btScalar* BulletRadii = new btScalar[Spheres];
        for( Whole X = 0 ; X < Spheres ; X++ )
        {
            BulletLocs[X] = Locations[X].GetBulletVector3();
            BulletRadii[X] = Radii[X];
        }
        MultiSphereShape = new btMultiSphereShape(BulletLocs,BulletRadii,Spheres);
        SetPointers(MultiSphereShape);
        delete BulletLocs;
        delete BulletRadii;
    }

    MultiSphereCollisionShape::MultiSphereCollisionShape(const String& Name, btMultiSphereShape* BulletShape)
    {
        this->Name = Name;
        MultiSphereShape = BulletShape;
        SetPointers(MultiSphereShape);
    }

    MultiSphereCollisionShape::~MultiSphereCollisionShape()
    {
        delete MultiSphereShape;
    }

    Vector3 MultiSphereCollisionShape::GetSphereLocation(const Whole& Index) const
    {
        Vector3 Loc(MultiSphereShape->getSpherePosition(Index));
        return Loc;
    }

    Real MultiSphereCollisionShape::GetSphereRadius(const Whole& Index) const
    {
        return MultiSphereShape->getSphereRadius(Index);
    }

    Whole MultiSphereCollisionShape::GetNumSpheres() const
    {
        return MultiSphereShape->getSphereCount();
    }

    CollisionShape::ShapeType MultiSphereCollisionShape::GetType() const
    {
        return CollisionShape::ST_MultiSphere;
    }

    /////////////////////////////////////////
    // SphereCollisionShape Functions

    SphereCollisionShape::SphereCollisionShape(const String& Name, const Real& Radius)
    {
        this->Name = Name;
        SphereShape = new btSphereShape(Radius);
        SetPointers(SphereShape);
    }

    SphereCollisionShape::SphereCollisionShape(const String& Name, btSphereShape* BulletShape)
    {
        this->Name = Name;
        SphereShape = BulletShape;
        SetPointers(SphereShape);
    }

    SphereCollisionShape::~SphereCollisionShape()
    {
        delete SphereShape;
    }

    Real SphereCollisionShape::GetRadius() const
    {
        return SphereShape->getRadius();
    }

    CollisionShape::ShapeType SphereCollisionShape::GetType() const
    {
        return CollisionShape::ST_Sphere;
    }

    /////////////////////////////////////////
    // DynamicMeshCollisionShape Functions

    DynamicMeshCollisionShape::DynamicMeshCollisionShape(const String& Name, btGImpactMeshShape* BulletShape)
    {
        this->Name = Name;
        GImpactShape = BulletShape;
        SetPointers(GImpactShape);
    }

    DynamicMeshCollisionShape::~DynamicMeshCollisionShape()
    {
        delete GImpactShape;
    }

    CollisionShape::ShapeType DynamicMeshCollisionShape::GetType() const
    {
        return CollisionShape::ST_GImpact;
    }

    /////////////////////////////////////////
    // HeightfieldCollisionShape Functions

    HeightfieldCollisionShape::HeightfieldCollisionShape(const String& Name)
    {

    }

    HeightfieldCollisionShape::HeightfieldCollisionShape(const String& Name, btHeightfieldTerrainShape* BulletShape)
    {
        this->Name = Name;
        HeightfieldShape = BulletShape;
        SetPointers(HeightfieldShape);
    }

    HeightfieldCollisionShape::~HeightfieldCollisionShape()
    {
        delete HeightfieldShape;
    }

    CollisionShape::ShapeType HeightfieldCollisionShape::GetType() const
    {
        return CollisionShape::ST_Heightfield;
    }

    /////////////////////////////////////////
    // PlaneCollisionShape Functions

    PlaneCollisionShape::PlaneCollisionShape(const String& Name)
    {

    }

    PlaneCollisionShape::PlaneCollisionShape(const String& Name, btStaticPlaneShape* BulletShape)
    {
        this->Name = Name;
        PlaneShape = BulletShape;
        SetPointers(PlaneShape);
    }

    PlaneCollisionShape::~PlaneCollisionShape()
    {
        delete PlaneShape;
    }

    CollisionShape::ShapeType PlaneCollisionShape::GetType() const
    {
        return CollisionShape::ST_Plane;
    }

    /////////////////////////////////////////
    // ActorSoftCollisionShape Functions

    ActorSoftCollisionShape::ActorSoftCollisionShape(const String& Name, btSoftBodyCollisionShape* BulletShape)
    {
        this->Name = Name;
        ActorSoftShape = BulletShape;
        SetPointers(ActorSoftShape);
    }

    ActorSoftCollisionShape::~ActorSoftCollisionShape()
    {
        delete ActorSoftShape;
    }

    CollisionShape::ShapeType ActorSoftCollisionShape::GetType() const
    {
        return CollisionShape::ST_ActorSoft;
    }

    /////////////////////////////////////////
    // StaticMeshCollisionShape Functions

    StaticMeshCollisionShape::StaticMeshCollisionShape(const String& Name, btBvhTriangleMeshShape* BulletShape)
    {
        /// @todo May need to create a btScaledBvhTriangleMeshShape for scaling this shape appropriately.
        this->Name = Name;
        BvhShape = BulletShape;
        SetPointers(BvhShape);
    }

    StaticMeshCollisionShape::~StaticMeshCollisionShape()
    {
        delete BvhShape;
    }

    CollisionShape::ShapeType StaticMeshCollisionShape::GetType() const
    {
        return CollisionShape::ST_TriMesh;
    }
}

#endif