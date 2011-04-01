//� Copyright 2010 BlackTopp Studios Inc.
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
#ifndef _cameracontroller_cpp
#define _cameracontroller_cpp

#include "cameracontroller.h"
#include "camera.h"

namespace phys
{
    CameraController::CameraController(Camera* ToBeControlled)
        : Controlled(ToBeControlled),
          CurrentMode(CCM_Fly),
          HoverHeight(1),
          YawRad(0),
          PitchRad(0),
          RollRad(0)
    {
    }

    CameraController::~CameraController()
    {
    }

    Real CameraController::FindDistanceToGround()
    {

    }

    Camera* CameraController::GetControlledCamera() const
    {
        return Controlled;
    }

    void CameraController::SetMovementMode(const CameraController::MovementMode& MoveMode)
    {
        CurrentMode = MoveMode;
    }

    CameraController::MovementMode CameraController::GetMovementMode() const
    {
        return CurrentMode;
    }

    void CameraController::SetHoverHeight(const Real& Hover)
    {
        HoverHeight = Hover;
    }

    Real CameraController::GetHoverHeight() const
    {
        return HoverHeight;
    }

    void CameraController::MoveForward(Real Units)
    {
        Vector3 Move(0,0,-Units);
        Controlled->MoveRelative(Move);
    }

    void CameraController::MoveBackward(Real Units)
    {
        Vector3 Move(0,0,Units);
        Controlled->MoveRelative(Move);
    }

    void CameraController::StrafeLeft(Real Units)
    {
        Vector3 Move(-Units,0,0);
        Controlled->MoveRelative(Move);
    }

    void CameraController::StrafeRight(Real Units)
    {
        Vector3 Move(Units,0,0);
        Controlled->MoveRelative(Move);
    }

    void CameraController::Rotate(Real Yaw, Real Pitch)
    {
        YawRad+=Yaw;
        PitchRad+=Pitch;
        Quaternion CamRot = Quaternion(-YawRad,Vector3::Unit_Y()) * Quaternion(-PitchRad,Vector3::Unit_X());
        Controlled->SetOrientation(CamRot);
    }

    void CameraController::Rotate(Real Yaw, Real Pitch, Real Roll)
    {
        Quaternion CamRot(Controlled->GetOrientation());
        CamRot = CamRot *
            Quaternion(-Yaw,Vector3::Unit_Y()) *
            Quaternion(-Pitch,Vector3::Unit_X()) *
            Quaternion(-Roll,Vector3::Unit_Z());
        Controlled->SetOrientation(CamRot);// */
    }
}

#endif