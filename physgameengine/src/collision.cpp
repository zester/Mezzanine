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
#ifndef _collision_cpp
#define _collision_cpp

#include "actorcontainerbase.h"
#include "actormanager.h"
#include "collision.h"
#include "stringtool.h"
#include "world.h"

#include <btBulletDynamicsCommon.h>
#include <memory>

namespace phys {
    Collision::Collision(ActorBase* actora, ActorBase* actorb, btPersistentManifold* PhysicsManifold)
    {
        ActorA=actora;
        ActorB=actorb;
        Manifold=PhysicsManifold;

        ActorA->_NotifyCollisionState(this,Collision::Col_Begin);
        ActorB->_NotifyCollisionState(this,Collision::Col_Begin);

        NumContacts = Manifold->getNumContacts();
        //for( int X = 0 ; X < NumContacts ; ++X )
        //    ContactLifeTimes.push_back(Manifold->getContactPoint(X)->m_lifeTime);
    }

    Collision::Collision()
    {
        ActorA=NULL;
        ActorB=NULL;
        Manifold=NULL;
    }

    Collision::Collision(const Collision& Other)
    {
        ActorA=Other.ActorA;
        ActorB=Other.ActorB;
        Manifold=Other.Manifold;

        // Double notifies seems like a bad idea.
        //ActorA->_NotifyCollisionState(this,Collision::Col_Begin);
        //ActorB->_NotifyCollisionState(this,Collision::Col_Begin);
    }

    Collision::~Collision()
    {
        ActorA->_NotifyCollisionState(this,Collision::Col_End);
        ActorB->_NotifyCollisionState(this,Collision::Col_End);
    }

    void Collision::SetActorA(ActorBase* A)
    {
        if(ActorA)
        {
            World::GetWorldPointer()->Log("Attepting to change Actor pointer Member in Collision.  This is not permitted.");
        }else{
            ActorA = A;
            ActorA->_NotifyCollisionState(this,Collision::Col_Begin);
        }
    }

    ActorBase* Collision::GetActorA() const
    {
        return ActorA;
    }

    void Collision::SetActorB(ActorBase* B)
    {
        if(ActorB)
        {
            World::GetWorldPointer()->Log("Attepting to change Actor pointer Member in Collision.  This is not permitted.");
        }else{
            ActorB = B;
            ActorB->_NotifyCollisionState(this,Collision::Col_Begin);
        }
    }

    ActorBase* Collision::GetActorB() const
    {
        return ActorB;
    }

    Whole Collision::GetNumContactPoints()
    {
        return (Whole)Manifold->getNumContacts();
    }

    Vector3 Collision::GetWorldLocation(const Whole& Point)
    {
        btVector3 PointA = Manifold->getContactPoint(Point).m_localPointA;
        btVector3 PointB = Manifold->getContactPoint(Point).m_localPointB;
        return Vector3((PointA+PointB) /= 2);
    }

    Vector3 Collision::GetLocalALocation(const Whole& Point)
    {
        return Vector3(Manifold->getContactPoint(Point).m_localPointA);
    }

    Vector3 Collision::GetLocalBLocation(const Whole& Point)
    {
        return Vector3(Manifold->getContactPoint(Point).m_localPointB);
    }

    Vector3 Collision::GetNormal(const Whole& Point)
    {
        return Vector3(Manifold->getContactPoint(Point).m_normalWorldOnB);
    }

    Real Collision::GetAppliedImpulse(const Whole& Point)
    {
        return Manifold->getContactPoint(Point).m_appliedImpulse;
    }

    Real Collision::GetDistance(const Whole& Point)
    {
        return Manifold->getContactPoint(Point).m_distance1;
    }

    Whole Collision::GetAge(const Whole& Point)
    {
        return (Whole)Manifold->getContactPoint(Point).m_lifeTime;
    }

    bool Collision::PairsMatch(ActorBase* A, ActorBase* B) const
    {
        bool ContainsA = (A == ActorA) || (A == ActorB);
        bool ContainsB = (B == ActorA) || (B == ActorB);
        return (ContainsA && ContainsB);
    }

    void Collision::Update()
    {
        if( NumContacts != Manifold->getNumContacts() )
        {
            ActorA->_NotifyCollisionState(this,Collision::Col_Contacts_Updated);
            ActorB->_NotifyCollisionState(this,Collision::Col_Contacts_Updated);
            NumContacts = Manifold->getNumContacts();
            return;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// Class External << Operators for streaming or assignment
#ifdef PHYSXML
std::ostream& operator << (std::ostream& stream, const phys::Collision& Col)
{
    stream  << "<EventCollision Version=\"1" //Impulse=\"" << Ev.Impulse
            << "\" ActorA=\"" << Col.GetActorA()->GetName()
            << "\" ActorB=\"" << Col.GetActorB()->GetName()
            << "\" >"
            //<<  Ev.WorldLocation
            << "</EventCollision>";
    return stream;
}

std::istream& PHYS_LIB operator >> (std::istream& stream, phys::Collision& Col)
{
    phys::String OneTag( phys::xml::GetOneTag(stream) );
    std::auto_ptr<phys::xml::Document> Doc( phys::xml::PreParseClassFromSingleTag("phys::", "EventCollision", OneTag) );

    Doc->GetFirstChild() >> Col;

    return stream;
}

/// @todo This whole thing needs to be updated, and have a serializer added.

void operator >> (const phys::xml::Node& OneNode, phys::Collision& Col)
{
    if ( phys::String(OneNode.Name())==phys::String("EventCollision"))
    {
        if(OneNode.GetAttribute("Version").AsInt() == 1)
        {

            Col.SetActorA(phys::World::GetWorldPointer()->GetActorManager()->GetActor(OneNode.GetAttribute("ActorA").AsString()));
            Col.SetActorB(phys::World::GetWorldPointer()->GetActorManager()->GetActor(OneNode.GetAttribute("ActorB").AsString()));
            //Col.Impulse=OneNode.GetAttribute("Impulse").AsReal();

            //if(OneNode.GetFirstChild())
            //{
            //    OneNode.GetFirstChild() >> Col.WorldLocation;
            //}else{
            //    throw(phys::Exception("Normal not found while parsing phys::Collision"));
            //}

        }else{
            throw( phys::Exception("Incompatible XML Version for Collision: Not Version 1"));
        }
    }else{
        throw( phys::Exception(phys::StringTool::StringCat("Attempting to deserialize a Collision, found a ", OneNode.Name())));
    }
}
#endif // \PHYSXML

#endif
