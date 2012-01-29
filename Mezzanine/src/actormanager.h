//� Copyright 2010 - 2011 BlackTopp Studios Inc.
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
#ifndef actormanager_h
#define actormanager_h

#include "datatypes.h"
#include "managerbase.h"
#include "singleton.h"

namespace Mezzanine
{
    class ActorBase;
    class ActorRigid;
    class ActorSoft;
    class ActorCharacter;

    // Used by the scripting language binder to help create bindgings for this class. SWIG does know to creation template instances
    #ifdef SWIG
    %template(SingletonActorManager) Singleton<ActorManager>;
    #endif

    ///////////////////////////////////////////////////////////////////////////////
    /// @class ActorManager
    /// @headerfile actormanager.h
    /// @brief A manager responsible for the storage and management of all actors in use.
    /// @details More or less Management point for a container of actors to help keep them sorted.
    ///////////////////////////////////////
    class MEZZ_LIB ActorManager : public ManagerBase, public Singleton<ActorManager>
    {
        protected:
            /// @brief The actual actor container
            std::vector<ActorBase*> Actors;

            /// @brief A Second listing of All the Rigid actors
            std::vector<ActorRigid*> RigidActors;

            /// @brief A Second listing of All the Soft actors
            std::vector<ActorSoft*> SoftActors;

            /// @brief A Second listing of All the Character actors
            std::vector<ActorCharacter*> CharacterActors;
        public:
            /// @brief Class constructor.
            ActorManager();
            /// @brief Class destructor.
            virtual ~ActorManager();

            typedef std::vector<ActorBase*>::iterator ActorIterator;
            typedef std::vector<ActorBase*>::const_iterator ConstActorIterator;
            typedef std::vector<ActorRigid*>::iterator ActorRigidIterator;
            typedef std::vector<ActorRigid*>::const_iterator ConstActorRigidIterator;
            typedef std::vector<ActorSoft*>::iterator ActorSoftIterator;
            typedef std::vector<ActorSoft*>::const_iterator ConstActorSoftIterator;
            typedef std::vector<ActorCharacter*>::iterator ActorCharacterIterator;
            typedef std::vector<ActorCharacter*>::const_iterator ConstActorCharacterIterator;

            ///////////////////////////////////////////////////////////////////////////////
            // Managing all actors

            /// @brief Gets an Actor by Index.
            /// @param Index The index of the actor you wish to retrieve.
            /// @return Returns a pointer to the actor at the specified index.
            virtual ActorBase* GetActor(const Whole& Index);
            /// @brief Gets an Actor by Name.
            /// @param Name The name of the actor you wish to retrieve.
            /// @return Returns a pointer to the actor of the specified name.
            virtual ActorBase* GetActor(const String& Name);
            /// @brief Gets the number of actors stored in this manager.
            /// @return Returns a whole representing the current actor count.
            virtual Whole GetNumActors() const;
            /// @brief Adds a pre-created actor to the manager.
            /// @details In some cases you may want to add and remove an actor from the world without destroying it and do some special
            /// manipulations to it to achieve some special/unique affects.  This function along with the "RemoveActor()"
            /// function facilitates this. @n
            /// This function is also necessary for anyone inheriting from our actors to add their actors to the world.
            /// @param Actor The actor to be added to the manager.
            virtual void AddActor(ActorBase* Actor);
            /// @brief Removes an actor from this manager without destroying it.
            /// @details In some cases you may want to add and remove an actor from the world without destroying it and do some special
            /// manipulations to it to achieve some special/unique affects.  This function along with the "RemoveActor()"
            /// function facilitates this. @n
            /// This function is also necessary for anyone inheriting from our actors to remove their actors from the world.
            /// @param Index The index at which to remove the actor.
            virtual void RemoveActor(const Whole& Index);
            /// @brief Removes an actor from this manager without destroying it.
            /// @details In some cases you may want to add and remove an actor from the world without destroying it and do some special
            /// manipulations to it to achieve some special/unique affects.  This function along with the "RemoveActor()"
            /// function facilitates this. @n
            /// This function is also necessary for anyone inheriting from our actors to remove their actors from the world.
            /// @param ToBeRemoved The actor to be removed from the manager.
            virtual void RemoveActor(ActorBase* ToBeRemoved);
            /// @brief Removes all actors from this manager without destroying them.
            virtual void RemoveAllActors();
            /// @brief Destroys an actor at the specified index.
            /// @param Index The index at which to destroy the actor.
            virtual void DestroyActor(const Whole& Index);
            /// @brief Destroys an actor.
            /// @param ToBeDestroyed The actor to be destroyed.
            virtual void DestroyActor(ActorBase* ToBeDestroyed);
            /// @brief Destroys all actors currently within this manager.
            virtual void DestroyAllActors();

            ///////////////////////////////////////////////////////////////////////////////
            // ActorRigid Management

            ///////////////////////////////////////////////////////////////////////////////
            // ActorSoft Management

            ///////////////////////////////////////////////////////////////////////////////
            // ActorCharacter Management

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @brief Calls to update every actor currently stored in the manager.
            virtual void UpdateAllActors();

            //Inherited from ManagerBase
            /// @brief Empty initializer that has been implemented from ManagerBase.
            virtual void Initialize();
            /// @brief Physics stepping during the main loop
            /// @details This increments the the physics world the required amount to keep it in sync with the Graphics/Timing.
            virtual void DoMainLoopItems();
            /// @brief This returns the type of this manager.
            /// @return This returns ManagerTypeName::PhysicsManager
            virtual ManagerTypeName GetType() const;
    };//actormanager
}//Mezzanine

#endif