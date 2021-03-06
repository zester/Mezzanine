//© Copyright 2010 - 2012 BlackTopp Studios Inc.
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
#ifndef _inputbuttondevice_cpp
#define _inputbuttondevice_cpp

#include "Input/buttondevice.h"

namespace Mezzanine
{
    namespace Input
    {
        ButtonDevice::ButtonDevice()
        {
        }

        ButtonDevice::~ButtonDevice()
        {
        }

        void ButtonDevice::UpdateButtonTransitions()
        {
            for( std::vector<Whole>::iterator It = TransitioningIndexes.begin() ; It != TransitioningIndexes.end() ; ++It )
            {
                Input::ButtonState State = Buttons.at( (*It) );
                if( Input::BUTTON_PRESSING == State )
                    Buttons.at( (*It) ) = Input::BUTTON_DOWN;
                else if( Input::BUTTON_LIFTING == State )
                    Buttons.at( (*It) ) = Input::BUTTON_UP;
            }
            TransitioningIndexes.clear();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Query Methods

        UInt16 ButtonDevice::GetNumButtons() const
        {
            return Buttons.size();
        }

        bool ButtonDevice::IsButtonPressed(const UInt16 Button) const
        {
            return Input::BUTTON_UP < GetButtonState(Button);
        }

        bool ButtonDevice::IsButtonPressed(const Input::InputCode& Button) const
        {
            return Input::BUTTON_UP < GetButtonState(Button);
        }

        bool ButtonDevice::IsButtonPressing(const UInt16 Button) const
        {
            return Input::BUTTON_PRESSING == GetButtonState(Button);
        }

        bool ButtonDevice::IsButtonPressing(const Input::InputCode& Button) const
        {
            return Input::BUTTON_PRESSING == GetButtonState(Button);
        }

        bool ButtonDevice::IsButtonLifting(const UInt16 Button) const
        {
            return Input::BUTTON_LIFTING == GetButtonState(Button);
        }

        bool ButtonDevice::IsButtonLifting(const Input::InputCode& Button) const
        {
            return Input::BUTTON_LIFTING == GetButtonState(Button);
        }

        bool ButtonDevice::IsButtonTransitioning(const UInt16 Button) const
        {
            Input::ButtonState State = GetButtonState(Button);
            return (Input::BUTTON_LIFTING == State || Input::BUTTON_PRESSING == State);
        }

        bool ButtonDevice::IsButtonTransitioning(const Input::ButtonState& Button) const
        {
            Input::ButtonState State = GetButtonState(Button);
            return (Input::BUTTON_LIFTING == State || Input::BUTTON_PRESSING == State);
        }

        bool ButtonDevice::CheckButtonState(const UInt16 Button, const Input::ButtonState& State) const
        {
            return State == GetButtonState(Button);
        }

        bool ButtonDevice::CheckButtonState(const Input::InputCode& Button, const Input::ButtonState& State) const
        {
            return State == GetButtonState(Button);
        }

        void ButtonDevice::_Update(const MetaCodeContainer& DeltaCodes, MetaCodeContainer& GeneratedCodes)
        {
            UpdateButtonTransitions();
            UpdateImpl(DeltaCodes,GeneratedCodes);
            AddPressedButtons(GeneratedCodes);
        }
    }//Input
}//Mezzanine

#endif
