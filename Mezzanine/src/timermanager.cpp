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
#ifndef _timermanager_cpp
#define _timermanager_cpp

#include "timermanager.h"
#include "extendedtimer.h"
#include "simpletimer.h"
#include <Ogre.h>

namespace Mezzanine
{
    template<> TimerManager* Singleton<TimerManager>::SingletonPtr = 0;

    TimerManager::TimerManager()
    {
        this->Priority = 5;
        MasterTimer = new Ogre::Timer();
    }

    TimerManager::~TimerManager()
    {
        for( Whole x = 0 ; x < Timers.size() ; x++ )
            { delete Timers[x]; }
        Timers.clear();
        delete MasterTimer;
    }

    SimpleTimer* TimerManager::CreateSimpleTimer(const Timer::TimerStyle Style)
    {
        SimpleTimer* SimTimer = new SimpleTimer(Style);
        Timers.push_back(SimTimer);
        return SimTimer;
    }

    ExtendedTimer* TimerManager::CreateExtendedTimer(const Timer::TimerStyle Style)
    {
        ExtendedTimer* ExTimer = new ExtendedTimer(Style);
        Timers.push_back(ExTimer);
        return ExTimer;
    }

    Timer* TimerManager::GetTimer(const Whole Index)
    {
        return Timers[Index];
    }

    Whole TimerManager::GetNumTimers()
    {
        return Timers.size();
    }

    void TimerManager::DestroyTimer(Timer* ToBeDestroyed)
    {
        if(!ToBeDestroyed)
            return;
        if(Timers.empty())
            return;
        for( std::vector<Timer*>::iterator it = Timers.begin() ; it != Timers.end() ; it++ )
        {
            if( ToBeDestroyed == (*it) )
            {
                delete ToBeDestroyed;
                Timers.erase(it);
                return;
            }
        }
    }

    void TimerManager::Initialize()
    {
    }

    void TimerManager::DoMainLoopItems()
    {
        Whole Elapsed = MasterTimer->getMicroseconds();
        MasterTimer->reset();
        if(Timers.empty())
            return;
        for( std::vector<Timer*>::iterator it = Timers.begin() ; it != Timers.end() ; it++ )
        {
            (*it)->Update(Elapsed);
        }
    }

    ManagerBase::ManagerTypeName TimerManager::GetType() const
    { return ManagerBase::TimerManager; }
}

#endif