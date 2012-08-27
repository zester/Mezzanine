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
#ifndef _inputcontroller_cpp
#define _inputcontroller_cpp

#include "Input/controller.h"

#include "SDL.h"
#include "../src/joystick/SDL_sysjoystick.h"

namespace Mezzanine
{
    namespace Input
    {
        Controller::Controller(void* InternalControl)
        {
            SDL_Joystick* InternalController = (SDL_Joystick*)InternalControl;
            Axes.resize( SDL_JoystickNumAxes(InternalController) );
            Trackballs.resize( SDL_JoystickNumBalls(InternalController) );
            Buttons.resize( SDL_JoystickNumButtons(InternalController) );
            Hats.resize( SDL_JoystickNumHats(InternalController) );
            memset( &Axes[0],0,sizeof(Axes[0]) * Axes.size() );
            memset( &Trackballs[0],0,sizeof(Trackballs[0]) * Trackballs.size() );
            memset( &Buttons[0],Input::BUTTON_UP,sizeof(Buttons[0]) * Buttons.size() );
            memset( &Hats[0],Input::CONTROLLERHAT_CENTERED,sizeof(Hats[0]) * Hats.size() );
            Index = SDL_JoystickIndex(InternalController);
            DeviceName = SDL_JoystickName(Index);
        }

        Controller::~Controller()
        {
        }

        void Controller::UpdateImpl(std::vector<MetaCode>& Codes)
        {
            for( Whole X = 0 ; X < Codes.size() ; ++X )
            {
                MetaCode& CurrCode = Codes[X];
                if(CurrCode.GetCode() >= Input::CONTROLLERAXIS_FIRST && CurrCode.GetCode() <= Input::CONTROLLERAXIS_LAST)
                {
                    Axes.at( CurrCode.GetCode() - Input::CONTROLLERAXIS_FIRST ) = CurrCode.GetMetaValue();
                }
                else if(CurrCode.GetCode() >= Input::CONTROLLERBALL_FIRST && CurrCode.GetCode() <= Input::CONTROLLERBALL_LAST)
                {
                    if( Input::CONTROLLERBALL_1_HORIZONTAL == CurrCode.GetCode() ){
                        Trackballs.at(0).X = CurrCode.GetMetaValue();
                    }else if( Input::CONTROLLERBALL_1_VERTICAL == CurrCode.GetCode() ){
                        Trackballs.at(0).Y = CurrCode.GetMetaValue();
                    }else if( Input::CONTROLLERBALL_2_HORIZONTAL == CurrCode.GetCode() ){
                        Trackballs.at(1).X = CurrCode.GetMetaValue();
                    }else if( Input::CONTROLLERBALL_2_VERTICAL == CurrCode.GetCode() ){
                        Trackballs.at(1).Y = CurrCode.GetMetaValue();
                    }
                }
                else if(CurrCode.GetCode() >= Input::CONTROLLERBUTTON_FIRST && CurrCode.GetCode() <= Input::CONTROLLERBUTTON_LAST)
                {
                    TransitioningIndexes.push_back( CurrCode.GetCode() - Input::CONTROLLERBUTTON_FIRST );
                    Buttons.at( CurrCode.GetCode() - Input::CONTROLLERBUTTON_FIRST ) = static_cast<Input::ButtonState>(CurrCode.GetMetaValue());
                }
                else if(CurrCode.GetCode() >= Input::CONTROLLERHAT_FIRST && CurrCode.GetCode() <= Input::CONTROLLERHAT_LAST)
                {
                    Hats.at( CurrCode.GetCode() - Input::CONTROLLERHAT_FIRST ) = static_cast<Input::HatState>(CurrCode.GetMetaValue());
                }
            }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Query Methods

        UInt16 Controller::GetDeviceIndex() const
        {
            return Index;
        }

        const String& Controller::GetDeviceName() const
        {
            return DeviceName;
        }

        UInt16 Controller::GetNumAxes() const
        {
            return Axes.size();
        }

        UInt16 Controller::GetNumTrackballs() const
        {
            return Trackballs.size();
        }

        UInt16 Controller::GetNumHats() const
        {
            return Hats.size();
        }

        bool Controller::IsHatPushedInDirection(const UInt16 Hat, const Input::HatState& WhichWay) const
        {
            Input::HatState State = GetHatState(Hat);
            return State == WhichWay;
        }

        Int16 Controller::GetAxis(const UInt16 Axis) const
        {
            return Axes.at( Axis - 1 );
        }

        Vector2 Controller::GetTrackballDelta(const UInt16 Trackball) const
        {
            return Trackballs.at( Trackball - 1 );
        }

        Real Controller::GetTrackballDeltaX(const UInt16 Trackball) const
        {
            return GetTrackballDelta(Trackball).X;
        }

        Real Controller::GetTrackballDeltaY(const UInt16 Trackball) const
        {
            return GetTrackballDelta(Trackball).Y;
        }

        const Input::HatState& Controller::GetHatState(const UInt16 Hat) const
        {
            return Hats.at( Hat - 1 );
        }

        Int16 Controller::GetAxis(const Input::InputCode& Axis) const
        {
            return Axes.at( Axis - Input::CONTROLLERAXIS_FIRST );
        }

        Real Controller::GetTrackballDelta(const Input::InputCode& Trackball) const
        {
            if( Input::CONTROLLERBALL_1_HORIZONTAL == Trackball ){
                return Trackballs.at(0).X;
            }else if( Input::CONTROLLERBALL_1_VERTICAL == Trackball ){
                return Trackballs.at(0).Y;
            }else if( Input::CONTROLLERBALL_2_HORIZONTAL == Trackball ){
                return Trackballs.at(1).X;
            }else if( Input::CONTROLLERBALL_2_VERTICAL == Trackball ){
                return Trackballs.at(1).Y;
            }else{
                MEZZ_EXCEPTION(Exception::INVALID_PARAMETERS_EXCEPTION,"Invalid Trackball parameter.");
            }
        }

        const Input::ButtonState& Controller::GetButtonState(const UInt16 Button) const
        {
            return Buttons.at( Button - 1 );
        }

        const Input::ButtonState& Controller::GetButtonState(const Input::InputCode& Button) const
        {
            return Buttons.at( Button - Input::CONTROLLERBUTTON_FIRST );
        }

        const Input::HatState& Controller::GetHatState(const Input::InputCode& Hat) const
        {
            return Hats.at( Hat - Input::CONTROLLERHAT_FIRST );
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Configuration Methods

        ///////////////////////////////////////////////////////////////////////////////
        // Utility Methods
    }//Input
}//Mezzanine

#endif