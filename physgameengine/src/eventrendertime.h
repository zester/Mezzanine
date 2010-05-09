//© Copyright 2010 Joseph Toppi and John Blackwood
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
#ifndef EVENTRENDERTIME_H
#define EVENTRENDERTIME_H
///////////////////////////////////////////////////////////////////////////////
// This will store the amount of time since rendering events that occured recently
///////////////////////////////////////

#include "event.h"
#include "physdatatypes.h"

using namespace std;

namespace phys
{
    ///////////////////////////////////////////////////////////////////////////////
    /// @class EventRenderTime
    /// @headerfile eventrendertime.h
    /// @brief This communicates the amount of time since the world was rendered
    /// @details This stores in milliseconds the amount of time since the last
    /// rendering of the physworld.
    class EventRenderTime : public Event
    {
        private:
            PhysWhole Rendertime;

        public:

            /// @brief The Constructor
            /// @details This is the only way to set the time
            /// @param Milliseconds As it says, the amount of milliseconds since the last rendering
            EventRenderTime (PhysWhole Milliseconds);

            /// @brief Returns that this event is a EventType::RenderTime
            /// @details This is primarily for the benefit of sorting thorugh event pointers. If
            /// this functions returns EventType::RenderTime, then and event pointer can safely be
            /// cast to phys::EventRenderTime . This method is inherited from phys::Event .
            virtual EventType getEventType() const;

            /// @brief Returns the a floating point value with the amount of time.
            /// @pdetails Returns the a floating point value with the amount of time.
            /// @return A floating point value with the amount of time.
            PhysWhole getMilliSecondsSinceLastFrame();
    };
}

#endif