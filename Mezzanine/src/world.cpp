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
#ifndef _world_cpp
#define _world_cpp
///////////////////////////////////////////////////////////////////////////////
//The world that integrates everything
//
//This is where all the calls to the the underlying libraries should be from.
//World is an abstraction layer to all of the non-ui libraries. The ui
//layers wx and ogre both have their own main loops, so it is almost mandatory
//for other code to interact with those libraries directly.
///////////////////////////////////////////////////////////////////////////////
//Includes

#include "mezzanine.h"

#include <SDL.h>
#include <Ogre.h>
#include <btBulletDynamicsCommon.h>

#include <sstream>
#include <string>

using namespace std;

namespace Mezzanine
{
    World* World::TheRealWorld = 0;

    ///////////////////////////////////////////////////////////////////////////////
    // Mezzanine constructors
    World::World()
    {
        PhysicsConstructionInfo PhysicsInfo;
        std::vector <ManagerBase*> temp;

        this->Construct(PhysicsInfo, SceneManager::Generic, "plugins.cfg",".", "Mezzanine.log", temp);
    }

    World::World(   const PhysicsConstructionInfo& PhysicsInfo,
                    SceneManager::SceneManagerType SceneType,
                    const String &PluginsFileName,
                    const String &EngineDataPath,
                    std::string LogFileName)
    {
        std::vector <ManagerBase*> temp;
        this->Construct(PhysicsInfo,
                        SceneType,
                        PluginsFileName,
                        EngineDataPath,
                        LogFileName,
                        temp );
    }

    World::World(  const PhysicsConstructionInfo& PhysicsInfo,
                    SceneManager::SceneManagerType SceneType,
                    const String &PluginsFileName,
                    const String &EngineDataPath,
                    const std::string &LogFileName,
                    const std::vector <ManagerBase*> &ManagerToBeAdded)
    {
        this->Construct(PhysicsInfo,
                        SceneType,
                        PluginsFileName,
                        EngineDataPath,
                        LogFileName,
                        ManagerToBeAdded );

    }

    World* World::GetWorldPointer()
    {
        assert(0!=World::TheRealWorld);
        return TheRealWorld;
    }

    namespace
    {
        // Since there doesn't seem to be anyway to check if the ogre root exists yet.
        // if a you haven't made an ogre root, then Ogre::Root::getSingletonPtr(); appears to return gibberish rather that a zero.
        Ogre::Root* OgreCore = 0;
    }

    void World::Construct(  const PhysicsConstructionInfo& PhysicsInfo,
                                SceneManager::SceneManagerType SceneType,
                                String PluginsFileName,
                                String EngineDataPath,
                                String LogFileName,
                                std::vector <ManagerBase*> ManagerToBeAdded)
    {
        //Set some sane Defaults for some values
        this->TargetFrameLength=16;
        this->FrameTime = 0;
        this->ManualLoopBreak = false;

        this->SetLoggingFrequency(LogOncePerFrame);


        if ( 0 == OgreCore )
            { OgreCore = new Ogre::Root(EngineDataPath+PluginsFileName,"",LogFileName); }
        else
            { OgreCore = Ogre::Root::getSingletonPtr(); }

        World::TheRealWorld = this;

        //add each manager that was passed in to the manager list
        for(std::vector<ManagerBase*>::iterator iter = ManagerToBeAdded.begin(); iter!= ManagerToBeAdded.end(); iter++)
            { this->AddManager(*iter); }

        //Create and add any managers that have not been taken care of yet.
        if(this->GetActorManager()==0)
            { this->AddManager(new ActorManager()); }
        if(this->GetResourceManager()==0)
            { this->AddManager(new ResourceManager(EngineDataPath)); }
        if(this->GetGraphicsManager()==0)
            { this->AddManager(new GraphicsManager()); }
        if(this->GetAudioManager()==0)
            { this->AddManager(new AudioManager()); }
        if(this->GetEventManager()==0)
            { this->AddManager(new EventManager()); }
        if(this->GetPhysicsManager()==0)
            { this->AddManager(new PhysicsManager(PhysicsInfo)); }
        if(this->GetSceneManager()==0)
            { this->AddManager(new SceneManager(SceneType)); }
        if(this->GetUIManager()==0)
            { this->AddManager(new UIManager()); }
        if(this->GetTimerManager()==0)
            { this->AddManager(new TimerManager()); }
        if(this->GetMeshManager()==0)
            { this->AddManager(new MeshManager()); }
        if(this->GetCollisionShapeManager()==0)
            { this->AddManager(new CollisionShapeManager()); }

        // This Tests various assumptions about the way the platform works, and will not act
        SanityChecks();
    }

    void World::SanityChecks()
    {
        crossplatform::WaitMilliseconds(1500);
        //Perform a Test that only needs to be done once for the SDL/userinputevent system.`
        Log("Verifying size of userinput events:");
        Log(sizeof(MetaCode::InputCode));
        Log(sizeof(SDL_Scancode));
        Log(sizeof(int));
        if(sizeof(MetaCode::InputCode) != sizeof(SDL_Scancode))
        {
            LogAndThrow("User input subsystem Event Sizes Don't match, userinput subsystem will go down faster than 'that' girl on prom night.");
        }else{
            Log("External User input subsystem Event Sizes match, the User Input subsystem won't crash instantly");
        }

        if(sizeof(MetaCode::InputCode) != sizeof(int))
        {
            LogAndThrow("Internal User input subsystem Event Sizes Don't match, userinput subsystem cannot function.");
        }else{
            Log("Internal User input subsystem Event Sizes match, the User Input subsystem won't crash instantly");
        }
    }

    void World::OneTimeMainLoopInit()
    {
        GraphicsManager::GetSingletonPtr()->ResetRenderTimer();
    }

    ///////////////////////////////////////////////////////////////////////////////
    //tears the world down
    World::~World()
    {
        ManagerBase* Current;
        //for( std::list<ManagerBase*>::iterator iter = --this->ManagerList.end(); !ManagerList.empty(); iter = --this->ManagerList.end() ) //Backward
        for( std::list<ManagerBase*>::iterator iter = this->ManagerList.begin(); !ManagerList.empty(); iter = this->ManagerList.begin() ) //forward
        {
            #ifdef MEZZDEBUG
            //Be careful using this. Because the UImanager is deleted during scenemanager deconstruction, it may not be obvious when that memory is freed.
            String TypeName = (*iter)->GetTypeName();
            std::cout << "Deleting " << TypeName << std::endl;
            #endif
            Current = (*iter);
            RemoveManager(Current);
            delete Current;
        }

        SDL_Quit();

        Ogre::Root::getSingleton().shutdown();
        delete Ogre::Root::getSingletonPtr(); // This should be done by the shutdown method shouldn't it?
        OgreCore=0;

    }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility

    void World::PauseWorld(bool Pause)
    {
        PhysicsManager::GetSingletonPtr()->PauseSimulation(Pause);
        SceneManager::GetSingletonPtr()->PauseAllParticles(Pause);
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Logging

    // anonymous namespace for function pointer for logging commit mess
    namespace
    {
        Whole FrequencyCounter__=0;     // Used by some loggin functions, and interpretted differently by each
        SimpleTimer *LogTimer=0;

        void SetupXSecondTimer();

        void Never()
        {
            World::GetWorldPointer()->LogStream.str("");
        }

        void EachFrame()
        {
            #ifdef MEZZDEBUG
            static Whole LogIteration = 0;
            World::GetWorldPointer()->LogStream << "Logging - every frame, iteration: " << LogIteration;
            ++LogIteration;
            #endif
            World::GetWorldPointer()->DoMainLoopLogging();
        }

        void EachXFrame()
        {
            #ifdef MEZZDEBUG
            static Whole LogIteration = 0;
            World::GetWorldPointer()->LogStream << "Logging - per X frames, iteration: " << LogIteration;
            ++LogIteration;
            #endif
            static Whole X=0;
            if (X>FrequencyCounter__)
            {
                World::GetWorldPointer()->DoMainLoopLogging();
                X=0;
            }else{
                ++X;
            }
        }

        void EachXSeconds() //function still exists because we need it to identify what kind of logging frequency we are using.
            { }

        //helper functions and classes
        class TimerLogInX : public TimerCallback
        {
            virtual void DoCallbackItems()
            {
                #ifdef MEZZDEBUG
                static Whole LogIteration = 0;
                World::GetWorldPointer()->LogStream << "Logging - per X seconds, iteration: " << LogIteration;
                ++LogIteration;
                #endif
                World::GetWorldPointer()->DoMainLoopLogging();
                LogTimer->Reset();
                LogTimer->Start();
            }
        };

        void SetupXSecondTimer()
        {
            if (LogTimer)
            {
                TimerManager::GetSingletonPtr()->DestroyTimer(LogTimer);
                LogTimer=0;
            }
            LogTimer = TimerManager::GetSingletonPtr()->CreateSimpleTimer(Timer::StopWatch);
            LogTimer->SetInitialTime(FrequencyCounter__ * 1000000);
            LogTimer->SetGoalTime(0);
            LogTimer->Reset();
            //LogTimer->SetAutoReset(true);
            LogTimer->Start();
            LogTimer->SetCallback(new TimerLogInX);
        }

    }

    void World::SetLoggingFrequency(World::LoggingFrequency HowOften, Whole FrequencyCounter)
    {
        if (LogTimer)
        {
            TimerManager::GetSingletonPtr()->DestroyTimer(LogTimer);
            LogTimer=0;
        }
        FrequencyCounter__=FrequencyCounter;

        switch (HowOften)
        {
            case LogNever:
                LogCommitFunc=&Never;
                break;
            case LogOncePerFrame:
                LogCommitFunc=&EachFrame;
                break;
            case LogOncePerXFrames:
                LogCommitFunc=&EachXFrame;
                break;
            case LogOncePerXSeconds:
                SetupXSecondTimer();
                LogCommitFunc=&EachXSeconds;
                break;
        }
    }

    World::LoggingFrequency World::GetLoggingFrequency()
    {
        if (&Never==LogCommitFunc)
            return LogNever;

        if (&EachFrame==LogCommitFunc)
            return LogOncePerFrame;

        if (&EachXFrame==LogCommitFunc)
            return LogOncePerXFrames;

        if (&EachXSeconds==LogCommitFunc)
            return LogOncePerXSeconds;
    }

    void World::CommitLog()
        { this->DoMainLoopLogging(); }

    void World::LogString(const String& Message)
    {
        // if it is in the Audiologs then it has already happened so it needs to be logged first
        AudioManager* AudioMan = this->GetAudioManager();
        if(AudioMan && AudioMan->GetLogs())
            { this->LogStream << AudioMan->GetLogs()->str(); }
        if(Message.size()>0)
            { this->LogStream << endl << Message; }
    }

    void World::Log()
    {
        this->LogString("");
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Start the Game already
    void World::GameInit( const bool &CallMainLoop )
    {
        //#define MEZZDEBUG
        #ifdef MEZZDEBUG
        this->Log("Entering GameInit()");
        #endif

        #ifdef MEZZDEBUG
        this->Log("Loaded Graphics Settings");
        #endif

        //this->CreateRenderWindow();
        #ifdef MEZZDEBUG
        this->Log("Created the Render Work");
        #endif

        for (std::list< ManagerBase* >::iterator Iter=this->ManagerList.begin(); Iter!=this->ManagerList.end(); ++Iter )
        {
            this->LogStream << "Initializing " << (*Iter)->GetTypeName() << " Manager" << endl;
            if((*Iter)->GetType() != ManagerBase::GraphicsManager)
            {
                (*Iter)->Initialize();
            }
        }
        GraphicsManager::GetSingletonPtr()->Initialize();

        if(CallMainLoop)
        {
            #ifdef MEZZDEBUG
            this->Log("Starting the main loop");
            #endif
            this->MainLoop();
        }else{
            #ifdef MEZZDEBUG
            this->Log("Not calling the main loop");
            #endif
        }
    }

    void World::MainLoop()
    {
        /// @todo create a lighting manager and put this in there
        //this->OgreSceneManager->setAmbientLight( Ogre::ColourValue( 1, 1, 1 ) );

        /*! @page mainloop1 Main Loop Structure and Flow
         The MainLoop is heart of most video games and simulations.

         @section mainloopoverview1 Main loop Overview
         The Main loop runs in World.MainLoop() which is called by default from @ref World.GameInit(). By default this Method also starts the render, the physics andthe input systems. It does very
         little on it's own. The main loop then calls the PreMainLoopItems(), DoMainLoopItems and PreMainLoopItems(), for each manager in the order of their priority from Lowest to Highest.
         \n Here is a listing of  default priorities for each of the managers the a world intantiates by default:
            -50	User Input and events
            -40	Actors
            -30	Physics
            -20 Camera
            -10 Lighting (Not yet implemented)
              0 Graphics
             10	Sound
             20 Resources
        */

        Ogre::Timer* LoopTimer = new Ogre::Timer();
        this->OneTimeMainLoopInit();
        //As long as all the CallBacks return true the game continues
        bool DoNotBreak=true;
        while (DoNotBreak)
        {
            for (std::list< ManagerBase* >::iterator Iter=this->ManagerList.begin(); Iter!=this->ManagerList.end(); ++Iter )
            {
                #ifdef MEZZDEBUG
                this->LogStream << "Current Manager: " << (*Iter)->GetTypeName() << " - Priority: " << (*Iter)->GetPriority();
                //this->DoMainLoopLogging();
                #endif

                #ifdef MEZZPROFILE
                LoopTimer->reset();
                #endif

                //Actual main loop work
                if( !(*Iter)->PreMainLoopItems() )
                    { DoNotBreak=false; }

                (*Iter)->DoMainLoopItems();

                if( !(*Iter)->PostMainLoopItems() )
                    { DoNotBreak=false; }

                #ifdef MEZZPROFILE
                this->LogStream << (*Iter)->GetTypeName() << " took " << LoopTimer->getMicroseconds() << " microseconds.";
                this->Log();
                #endif

                LogCommitFunc();
            }
            if(ManualLoopBreak)
                    break;
        }//End of main loop

        //Some after loop cleanup
        //this->DestroyRenderWindow();
        ManualLoopBreak = false;
        delete LoopTimer;
    }

    void World::DoMainLoopLogging()
    {
        if( this->LogStream.str().size() > 0 )
        {
            Ogre::LogManager::getSingleton().logMessage(this->LogStream.str());
            this->LogStream.str("");
        }
    }

    void World::BreakMainLoop()
    {
        ManualLoopBreak = true;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Simple get and Set functions
    ///////////////////////////////////////

    Whole World::GetTargetFrameTime()
    {
        return this->TargetFrameLength;
    }

    void World::SetTargetFrameTime(const Whole &NewTargetTime)
    {
        this->TargetFrameLength = NewTargetTime;
    }

    void World::SetTargetFrameRate(const Whole &NewFrameRate)
    {
        this->SetTargetFrameTime( 1000/NewFrameRate );
    }

    Whole World::GetFrameTime()
    {
        return this->FrameTime;
    }

    void World::SetFrameTime( const Whole &FrameTime_ )
    {
        this->FrameTime = FrameTime_;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Upper Management
    ///////////////////////////////////////
    void World::AddManager(ManagerBase* ManagerToAdd)
    {
        #ifdef MEZZDEBUG
        this->LogStream << "Calling World::AddManager("<<ManagerToAdd<<") size before:" <<this->ManagerList.size();
        #endif
        if(!this->ManagerList.empty())
        {
            for(std::list< ManagerBase* >::iterator ManIter = this->ManagerList.begin(); ManIter!=this->ManagerList.end(); ++ManIter )
            {
                if( (*ManIter)->GetPriority() > ManagerToAdd->GetPriority())
                {
                    this->ManagerList.insert(ManIter, ManagerToAdd);
                    #ifdef MEZZDEBUG
                    this->LogStream << " - Added by sorted insertion:"<<ManagerToAdd<<" - size after:" <<this->ManagerList.size() ;
                    #endif
                    return;
                }
            }
        }
        ManagerList.push_back(ManagerToAdd);
        #ifdef MEZZDEBUG
        this->LogStream << " - Added by push_back:"<<ManagerToAdd<<" - size after:" <<this->ManagerList.size() << endl;
        #endif
    }

    void World::RemoveManager(ManagerBase* ManagerToRemove)
    {
        if(this->ManagerList.empty())
        {
            // do nothing
        }else{
            for(std::list< ManagerBase* >::iterator ManIter = this->ManagerList.begin(); ManIter!=this->ManagerList.end(); ++ManIter )
            {
                if( *ManIter == ManagerToRemove )
                {
                    this->ManagerList.erase(ManIter);
                    break;
                }
            }
        }
    }

    void World::RemoveManager(const ManagerBase::ManagerTypeName &ManagersToRemoveType, short unsigned int WhichOne)
    {
        if(this->ManagerList.empty())
        {
            //do nothing
        }else{
            for(std::list< ManagerBase* >::iterator ManIter = this->ManagerList.begin(); ManIter!=this->ManagerList.end(); ++ManIter )
            {
                if( (*ManIter)->GetType() == ManagersToRemoveType )
                {
                    if(0==WhichOne)     // we use our copy of WhichOne as a countdown to 0
                    {
                        this->ManagerList.erase(ManIter);
                    }else{
                        --WhichOne;
                    }
                }
            }
        }
    }

    ManagerBase* World::GetManager(const ManagerBase::ManagerTypeName &ManagersToGet, short unsigned int WhichOne)
    {
        #ifdef MEZZDEBUG
        //this->LogStream << "Calling World::GetManager(Type:"<<ManagersToGet<<") searching through "<<this->ManagerList.size()<<" Items.";
        #endif
        if(this->ManagerList.empty())
        {
            return 0;
        }else{
            for(std::list< ManagerBase* >::iterator ManIter = this->ManagerList.begin(); ManIter!=this->ManagerList.end(); ++ManIter )
            {
                if( (*ManIter)->GetType() == ManagersToGet )
                {
                    if(0==WhichOne)     // we use our copy of WhichOne as a countdown to 0
                    {
                        #ifdef MEZZDEBUG
                        //this->LogStream << " - Got:" << *ManIter;
                        //this->Log();
                        #endif
                        return *ManIter;
                    }else{
                        --WhichOne;
                    }
                }
            }
        }
        #ifdef MEZZDEBUG
        this->LogStream << " - Got:Nothing" << endl;
        #endif
        return 0;
    }

    void World::UpdateManagerOrder(ManagerBase* ManagerToChange, short int Priority_)
    {
        ManagerToChange->SetPriority(Priority_);
        if(this->ManagerList.empty())
        {
            ManagerList.push_back(ManagerToChange);
        }else{
            for(std::list< ManagerBase* >::iterator ManIter = this->ManagerList.begin(); ManIter!=this->ManagerList.end(); ++ManIter )
            {
                if( (*ManIter)->GetPriority() > ManagerToChange->GetPriority() )
                {
                    this->ManagerList.insert(ManIter,ManagerToChange);
                }else if(*ManIter == ManagerToChange )
                {
                    this->ManagerList.erase(ManIter);
                }
            }
        }
    }

    void World::UpdateManagerOrder()
    {
        if(this->ManagerList.empty())
        {
            //do nothing
        }else{
            std::list< ManagerBase* > temp; //(this->ManagerList.begin(),this->ManagerList.end());
            temp.swap(this->ManagerList);
            for(std::list< ManagerBase* >::iterator TempIter = temp.begin(); TempIter!=temp.end(); ++TempIter )
            {
                this->AddManager(*TempIter);
            }
        }
    }

    ActorManager* World::GetActorManager(const short unsigned int &WhichOne)
    {
        return dynamic_cast<ActorManager*> (this->GetManager(ManagerBase::ActorManager, WhichOne));
    }

    AudioManager* World::GetAudioManager(const short unsigned int &WhichOne)
    {
        return dynamic_cast<AudioManager*> (this->GetManager(ManagerBase::AudioManager, WhichOne));
    }

    CameraManager* World::GetCameraManager(const short unsigned int &WhichOne)
    {
        return dynamic_cast<CameraManager*> (this->GetManager(ManagerBase::CameraManager, WhichOne));
    }

    CollisionShapeManager* World::GetCollisionShapeManager(const short unsigned int &WhichOne)
    {
        return dynamic_cast<CollisionShapeManager*> (this->GetManager(ManagerBase::CollisionShapeManager, WhichOne));
    }

    EventManager* World::GetEventManager(const short unsigned int &WhichOne)
    {
        return dynamic_cast<EventManager*> (this->GetManager(ManagerBase::EventManager, WhichOne));
    }

    GraphicsManager* World::GetGraphicsManager(const short unsigned int &WhichOne)
    {
        return dynamic_cast<GraphicsManager*> (this->GetManager(ManagerBase::GraphicsManager, WhichOne));
    }

    MeshManager* World::GetMeshManager(const short unsigned int &WhichOne)
    {
        return dynamic_cast<MeshManager*> (this->GetManager(ManagerBase::MeshManager, WhichOne));
    }
    #ifdef MEZZNETWORK
    NetworkManager* World::GetNetworkManager(const short unsigned int &WhichOne)
    {
        return dynamic_cast<NetworkManager*> (this->GetManager(ManagerBase::NetworkManager, WhichOne));
    }
    #endif
    PhysicsManager* World::GetPhysicsManager(const short unsigned int &WhichOne)
    {
        return dynamic_cast<PhysicsManager*> (this->GetManager(ManagerBase::PhysicsManager, WhichOne));
    }

    SceneManager* World::GetSceneManager(const short unsigned int &WhichOne)
    {
        return dynamic_cast<SceneManager*> (this->GetManager(ManagerBase::SceneManager, WhichOne));
    }

    ResourceManager* World::GetResourceManager(const short unsigned int &WhichOne)
    {
        return dynamic_cast<ResourceManager*> (this->GetManager(ManagerBase::ResourceManager, WhichOne));
    }

    TimerManager* World::GetTimerManager(const short unsigned int &WhichOne)
    {
        return dynamic_cast<TimerManager*> (this->GetManager(ManagerBase::TimerManager, WhichOne));
    }

    UIManager* World::GetUIManager(const short unsigned int &WhichOne)
    {
        return dynamic_cast<UIManager*> (this->GetManager(ManagerBase::UIManager, WhichOne));
    }

}
#endif