//© Copyright 2010 - 2011 BlackTopp Studios Inc.
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
#ifndef actorcontainervector_h
#define actorcontainervector_h

#include "actorcontainerbase.h"
#include "crossplatformexport.h"
#include "datatypes.h"
#include "managerbase.h"

namespace Ogre
{
    class Node;
}

namespace Mezzanine
{
    // Used by the scripting language binder to help create bindgings for this class. SWIG does know to creation template instances
    #ifdef SWIG
    %template(VectorActorPtr) std::vector< ActorBase * >;
    #endif

    ///////////////////////////////////////////////////////////////////////////////
    /// @class ActorContainerVector
    /// @headerfile actorcontainervector.h
    /// @brief A simple Actor Container using a vector
    /// @details This inherits from std::vector and our Mezzanine::ActorContainerBase to
    /// allow us to have access to a container through a standardized structure this
    /// way the Mezzanine::world doesn't need to worry about the details when accessing
    /// and storing actors
    class MEZZ_LIB ActorContainerVector : public ActorContainerBase, public std::vector<ActorBase*>
    {
        public:
            /// @brief Simple Constructor
            /// @details This creates and empty usable container based on std::vector.
            ActorContainerVector ();
            /// @brief Class destructor
            virtual ~ActorContainerVector();

            //Simply Inherited mmembers
            virtual void AddActor(ActorBase* ActorToAdd);

            virtual ActorBase* LastActorAdded();

            virtual void RemoveActor(ActorBase* ActorToRemove);

            virtual void RemoveActorAtCursor();

            virtual void RemoveAllActors();

            virtual Whole GetActorCount() const;

            virtual void CursorToFirst();

            virtual void CursorToPrevious();

            virtual void CursorToNext();

            virtual void CursorToLast();

            virtual ActorBase* GetAtCursor() const;

            virtual ActorBase* GetFirst() const;

            virtual ActorBase* GetLast() const;

            virtual String GetContainerType() const;

            virtual World* GetGameWorld() const;

            /// @brief This finds an actor by searching for a graphics subsystem nodes.
            /// @details This will iterate through each Actor in the container until it finds one with a matching Node. This runs in linear time.
            /// @return This returns a pointer to and ActorBase that has a matching node.
            /// @param GraphicsNode This is a pointer to a GraphicsNode that the Actor you want to find will have.
            virtual ActorBase* FindActor(Ogre::SceneNode* GraphicsNode);

            /// @brief This finds an actor by searching for a physics subsystem object.
            /// @details This will iterate through each Actor in the container until it finds one with a matching physics object. This runs in linear time.
            /// @return This returns a pointer to and ActorBase that has a physics object.
            /// @param PhysicsObject This is a pointer to a physics object that the Actor you want to find will have.
            virtual ActorBase* FindActor(btCollisionObject* PhysicsObject);

            /// @brief This finds an actor based on its name.
            /// @details This will iterate through each Actor in the container until it finds one with a matching Name. This runs in linear time.
            /// @return This returns a pointer to and ActorBase that has a matching name.
            /// @param Name This is the name of the Actor you want to find.
            virtual ActorBase* FindActor(String Name);

            /// @brief This is used to store information about the cursor.
            /// @details This implementation of ActorContainerBase will use this, and only this to access the cursor
            /// so feel free to use this instead.
            vector<ActorBase*>::iterator cursor;

            /// @brief This safely move all the Actors out of or into a Mezzanine::World
            /// @details This Removes all scene nodes from the Ogre the graphics subsystem, and removes all bodies from the physics system
            /// if a Mezzanine::World is present. Then this sets up all actors with the new World unless it is NULL
            /// @param GameWorld_ This is a pointer to the new Mezzanine::World to work with.
            virtual void SetGameWorld( World* GameWorld_ );

            /// @brief Optionally move actors into or out of a world
            /// @param GameWorld_ This is a pointer to the new Mezzanine::World to work with.
            /// @param AddToWorld True to add AddActors if valid world pointer was supplied, false to not add
            /// @param RemoveFromWorld True to remove AddActors if valid world pointer was supplied, false to not remove
            virtual void SetGameWorld( World* GameWorld_ , bool AddToWorld, bool RemoveFromWorld);

        private:
            /// @brief This is used to implement LastActorAdded()
            ActorBase* RecentlyAdded;

            /// @brief The world in which we are running.
            World* GameWorld;
    };
}

#endif