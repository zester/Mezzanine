//© Copyright 2010 - 2013 BlackTopp Studios Inc.
/* This file is part of The Mezzanine Engine.

    The Mezzanine Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Mezzanine Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The Mezzanine Engine.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'Docs' folder. See 'gpl.txt'
*/
/* We welcome the use of the Mezzanine engine to anyone, including companies who wish to
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
#ifndef _actorrigid_cpp
#define _actorrigid_cpp

#include "btBulletDynamicsCommon.h"
#include "BulletSoftBody/btSoftRigidDynamicsWorld.h"

#include "entresol.h"
#include "meshmanager.h"
#include "Physics/physicsmanager.h"
#include "scenemanager.h"
#include "actorrigid.h"
#include "Physics/collision.h"
#include "Physics/generic6dofconstraint.h"
#include "Internal/motionstate.h.cpp" // This is required for the internal physmotionstate :(
#include "Internal/meshtools.h.cpp"
#include "serialization.h"
#include "mathtool.h"

namespace Mezzanine
{
    ///////////////////////////////////
    // ActorRigid class functions

    ActorRigid::ActorRigid(const Real& mass, const String& name, const String& file, const String& group)
        : ActorBase ()
    {
        Name = name;
        // this isn't required to operate, but it does allow the mesh manager to know what is loaded.
        MeshManager::GetSingletonPtr()->LoadMesh(file,group);

        this->GraphicsObject = Entresol::GetSingletonPtr()->GetSceneManager()->GetGraphicsWorldPointer()->createEntity(name, file, group);
        this->MotionState = new Internal::AttachableMotionState(this);
        this->CreateRigidObject(mass);
        this->GraphicsSettings = new WorldObjectGraphicsSettings(this,GraphicsObject);
        this->PhysicsSettings = new ActorRigidPhysicsSettings(this,PhysicsRigidBody);
    }

    ActorRigid::~ActorRigid()
    {
        delete PhysicsRigidBody;
        delete GraphicsSettings;
        delete PhysicsSettings;
        PhysicsObject = NULL;
    }

    void ActorRigid::CreateRigidObject(const Real& pmass)
    {
        btScalar bmass = pmass;
        this->PhysicsRigidBody = new btRigidBody(bmass, this->MotionState, this->PhysicsShape);
        PhysicsObject = PhysicsRigidBody;
        Ogre::Any OgreRef( (WorldObject*)this );
        GraphicsObject->setUserAny(OgreRef);
        PhysicsObject->setUserPointer( (WorldObject*)this );
        if(0.0 == bmass)
        {
            PhysicsObject->setCollisionFlags(btCollisionObject::CF_STATIC_OBJECT);
        }else{
            PhysicsObject->setCollisionFlags(PhysicsObject->getCollisionFlags() & (~btCollisionObject::CF_STATIC_OBJECT));
        }
    }

    ActorRigidPhysicsSettings* ActorRigid::GetPhysicsSettings() const
        { return static_cast<ActorRigidPhysicsSettings*>(this->PhysicsSettings); }

    void ActorRigid::SetLinearMovementFactor(const Vector3& Factor)
        { this->PhysicsRigidBody->setLinearFactor(Factor.GetBulletVector3()); }

    Vector3 ActorRigid::GetLinearMovementFactor() const
        { return Vector3(this->PhysicsRigidBody->getLinearFactor()); }

    void ActorRigid::SetAngularMovementFactor(const Vector3& Factor)
        { this->PhysicsRigidBody->setAngularFactor(Factor.GetBulletVector3()); }

    Vector3 ActorRigid::GetAngularMovementFactor() const
        { return Vector3(this->PhysicsRigidBody->getAngularFactor()); }

    WorldAndSceneObjectType ActorRigid::GetType() const
        { return Mezzanine::WSO_ActorRigid; }

    void ActorRigid::AddToWorld()
    {
        if( IsInWorld() )
            return;
        Entresol::GetSingletonPtr()->GetPhysicsManager()->_GetPhysicsWorldPointer()->addRigidBody(this->PhysicsRigidBody,GetPhysicsSettings()->GetCollisionGroup(),GetPhysicsSettings()->GetCollisionMask());
        this->AttachToGraphics();
    }

    void ActorRigid::RemoveFromWorld()
    {
        if( !IsInWorld() )
            return;

        try{
            Physics::PhysicsManager* PhysMan = Entresol::GetSingletonPtr()->GetPhysicsManager();
            btSoftRigidDynamicsWorld* BWorld = PhysMan->_GetPhysicsWorldPointer();
            BWorld->removeRigidBody(this->PhysicsRigidBody);
            DetachAllChildren();
        }catch (...) {
            Entresol::GetSingletonPtr()->Log("Failed to locate PhysicsManager While Destructing ActorRigid. This is not a problem if already shutting down.");
        }

        //first remove any collision metadata
        /*if( !CurrentCollisions.empty() )
        {
            PhysMan->RemoveCollisionsContainingActor(this);
        }// */

        this->DetachFromGraphics();
    }

    void ActorRigid::_Update()
    {
        StickyData* StickyD = GetPhysicsSettings()->GetStickyData();
        if(StickyD->CreationQueue.empty())
            return;
        RemoveFromWorld();
        AddToWorld();
        for( Whole X = 0 ; X < StickyD->CreationQueue.size() ; ++X )
        {
            StickyConstraintConstructionInfo& CurrInfo = StickyD->CreationQueue[X];
            /*ObjectPair NewPair(CurrInfo.ActA,CurrInfo.ActB);
            Collision* Col = Entresol::GetSingletonPtr()->GetPhysicsManager()->GetCollision(&NewPair);
            if(Col) Entresol::GetSingletonPtr()->GetPhysicsManager()->RemoveCollision(Col);//*/
            StickyConstraint* NewSticky = new StickyConstraint(CurrInfo.ActA,CurrInfo.ActB,CurrInfo.TransA,CurrInfo.TransB);
            NewSticky->SetAngularLimitLower(Vector3());
            NewSticky->SetAngularLimitUpper(Vector3());
            NewSticky->SetLinearLimitLower(Vector3());
            NewSticky->SetLinearLimitUpper(Vector3());
            /*NewSticky->SetUpperLinLimit(0.0);
            NewSticky->SetUpperAngLimit(0.0);
            NewSticky->SetLowerLinLimit(0.0);
            NewSticky->SetLowerAngLimit(0.0);//*/
            Entresol::GetSingletonPtr()->GetPhysicsManager()->_GetPhysicsWorldPointer()->addConstraint(NewSticky->GetConstraintBase(),true);
            StickyD->StickyConstraints.push_back(NewSticky);
            CurrInfo.ActA->GetPhysicsSettings()->GetStickyData()->StickyConstraints.push_back(NewSticky);
            NewSticky->GetConstraintBase()->setOverrideNumSolverIterations(100);
        }
        StickyD->CreationQueue.clear();
    }

    void ActorRigid::_NotifyCollisionState(Physics::Collision* Col, const Physics::Collision::CollisionState& State)
    {
        WorldObject::_NotifyCollisionState(Col,State);
        StickyData* StickyD = GetPhysicsSettings()->GetStickyData();
        bool UseA = Col->GetObjectA() != this;
        // We don't care if sticky behavior isn't set or if the collision has ended.
        // If it's ended, then we've probably already done our logic.
        if(0 == StickyD->MaxNumContacts || Physics::Collision::Col_End == State)
            return;
        // Constraints only work well with other rigid bodies, so for now just other actorrigids.
        /// @todo Update this to be workable with other objects that have rigid bodies internally, and maybe soft bodies.
        if( (UseA ? (Col->GetObjectA()->GetType() != Mezzanine::WSO_ActorRigid) : (Col->GetObjectB()->GetType() != Mezzanine::WSO_ActorRigid)) )
            return;
        // We need a contact point to be present for this to work, since a collision without contact points is an AABB overlap.
        // So confirm there are contact points.
        if(Col->GetNumContactPoints() < 1)
            return;
        // Do we have room for additional sticky constraints?
        if(StickyD->StickyConstraints.size() < StickyD->MaxNumContacts)
        {
            // Does this sticky constraint already exist for this pair?
            for( Whole X = 0 ; X < StickyD->StickyConstraints.size() ; ++X )
            {
                StickyConstraint* StickyCon = StickyD->StickyConstraints.at(X);
                if( Col->PairsMatch(StickyCon->GetActorA(),StickyCon->GetActorB()) )
                    return;
            }
            // Alternatively, is this sticky constraint about to be constructed?
            for( Whole Y = 0 ; Y < StickyD->CreationQueue.size() ; ++Y )
            {
                StickyConstraintConstructionInfo& CurrInfo = StickyD->CreationQueue.at(Y);
                if( Col->PairsMatch(CurrInfo.ActA,CurrInfo.ActB) )
                    return;
            }
            // Does this collision have a contact that is actually penetrating?
            // Does this collision have actual force?
            bool NegativeDistFound = false;
            Whole FoundIndex = 0;
            Real BestMatch = 123000;
            for( Whole X = 0 ; X < Col->GetNumContactPoints() ; ++X )
            {
                Real Dist = Col->GetDistance(X);
                if( MathTools::Fabs(Dist) < BestMatch && Dist <= 0 &&
                    0 != Col->GetAppliedImpulse(X) )
                {
                    FoundIndex = X;
                    BestMatch = MathTools::Fabs(Dist);
                    NegativeDistFound = true;
                    break;
                }
            }
            if(!NegativeDistFound)
                return;
            // Ok, it's passed all the tests, the constraint is being made.
            // Get the actor and it's sticky data.
            StickyConstraintConstructionInfo NewInfo;
            NewInfo.ActA = dynamic_cast<ActorRigid*>(UseA ? Col->GetObjectA() : Col->GetObjectB());
            NewInfo.ActB = this;
            //StickyData* ActorAStickyData = ActorA->GetPhysicsSettings()->GetStickyData();
            // Calculate the transforms.
            Vector3 ALoc = UseA ? Col->GetLocalALocation(FoundIndex) : Col->GetLocalBLocation(FoundIndex);
            Vector3 BLoc = UseA ? Col->GetLocalBLocation(FoundIndex) : Col->GetLocalALocation(FoundIndex);
            NewInfo.TransA = Transform(ALoc,NewInfo.ActA->GetOrientation());
            NewInfo.TransB = Transform(BLoc,this->GetOrientation());
            StickyD->CreationQueue.push_back(NewInfo);

            /*ObjectPair CollisionPair(ActorA,this);
            Entresol::GetSingletonPtr()->GetPhysicsManager()->RemoveCollision(PhysicsManager::GetSingletonPtr()->GetCollision(&CollisionPair));

            // Create and configure the constraint.
            StickyConstraint* NewSticky = new StickyConstraint(ActorA,this,TransA,TransB);
            NewSticky->SetUpperLinLimit(0.0);
            NewSticky->SetUpperAngLimit(0.0);
            NewSticky->SetLowerLinLimit(0.0);
            NewSticky->SetLowerAngLimit(0.0);
            // Add the constraint to the world and other necessary structures.
            Entresol::GetSingletonPtr()->GetPhysicsManager()->_GetPhysicsWorldPointer()->addConstraint(NewSticky->GetConstraintBase(),true);
            StickyD->StickyConstraints.push_back(NewSticky);
            ActorAStickyData->StickyConstraints.push_back(NewSticky);// */
        }
    }

    btRigidBody* ActorRigid::GetBulletObject()
        { return PhysicsRigidBody; }

///////////////////////////////////////////////////////////////////////////////
// Serialization
///////////////////////////////////////
    void ActorRigid::ThrowSerialError(const String& Fail) const
        { SerializeError(Fail, SerializableName()); }

    String ActorRigid::GraphicsSettingsSerializableName() const
        { return WorldObjectGraphicsSettings::SerializableName(); }

    String ActorRigid::PhysicsSettingsSerializableName() const
        { return "ActorRigidPhysicsSettings"; }

    void ActorRigid::ProtoSerialize(XML::Node& CurrentRoot) const
    {
        XML::Node ActorNode = CurrentRoot.AppendChild("ActorRigid");
        if (!ActorNode) { ThrowSerialError("create ActorRigidNode");}

        XML::Attribute Version = ActorNode.AppendAttribute("Version");
        if (Version)
            { Version.SetValue(1); }
        else
            { SerializeError("Create set Version on ActorRigid node", SerializableName()); }

        XML::Attribute ActorName = ActorNode.AppendAttribute("Name");
            ActorName.SetValue(this->GetName());
        XML::Attribute ActorFile = ActorNode.AppendAttribute("File");
            ActorFile.SetValue(this->GraphicsObject->getMesh()->getName());
        XML::Attribute ActorGroup = ActorNode.AppendAttribute("Group");
            ActorGroup.SetValue(this->GraphicsObject->getMesh()->getGroup());
        if( !(ActorName && ActorFile && ActorGroup) )
            { ThrowSerialError("creating ActorRigid Attributes");}

        XML::Node LinearMovementFactor = ActorNode.AppendChild("LinearMovementFactor");
        if (!LinearMovementFactor) { ThrowSerialError("create LinearMovementFactor Node"); }
        this->GetLinearMovementFactor().ProtoSerialize(LinearMovementFactor);

        XML::Node AngularMovementFactor = ActorNode.AppendChild("AngularMovementFactor");
        if (!AngularMovementFactor) { ThrowSerialError("create AngularMovementFactor Node"); }
        this->GetAngularMovementFactor().ProtoSerialize(AngularMovementFactor);

        ActorBase::ProtoSerialize(ActorNode);
    }

    void ActorRigid::ProtoDeSerialize(const XML::Node& OneNode)
    {
        if ( Mezzanine::String(OneNode.Name())==this->ActorRigid::SerializableName() )
        {
            if(OneNode.GetAttribute("Version").AsInt() == 1)
            {
                this->ActorBase::ProtoDeSerialize(OneNode.GetChild(this->ActorBase::SerializableName()));

                Vector3 TempVec;
                XML::Node LinearMovementFactor = OneNode.GetChild("LinearMovementFactor").GetFirstChild();
                if(!LinearMovementFactor)
                    { DeSerializeError("locate LinearMovementFactor node",SerializableName()); }
                TempVec.ProtoDeSerialize(LinearMovementFactor);
                this->SetLinearMovementFactor(TempVec);

                XML::Node AngularMovementFactor = OneNode.GetChild("AngularMovementFactor").GetFirstChild();
                if(!AngularMovementFactor)
                    { DeSerializeError("locate AngularMovementFactor node",SerializableName()); }
                TempVec.ProtoDeSerialize(AngularMovementFactor);
                this->SetAngularMovementFactor(TempVec);
                // could not do Name, File, Group - done in ActorDeSerializer
            }else{
                DeSerializeError("find usable serialization version",SerializableName());
            }
        }else{
            DeSerializeError(String("find correct class to deserialize, found a ")+OneNode.Name(),SerializableName());
        }
    }

    String ActorRigid::SerializableName()
        {   return String("ActorRigid"); }

}

std::ostream& operator << (std::ostream& stream, const Mezzanine::ActorRigid& ActorToSerialize)
{
    Serialize(stream, ActorToSerialize);
    return stream;
}

std::istream& operator >> (std::istream& stream, Mezzanine::ActorRigid& x)
    { return DeSerialize(stream, x); }

void operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::ActorRigid& x)
    { x.ProtoDeSerialize(OneNode); }


#endif
