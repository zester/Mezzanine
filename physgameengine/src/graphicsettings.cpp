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
#ifndef _graphicsettings_cpp
#define _graphicsettings_cpp

#include "physworld.h"
#include "physeventmanager.h"
#include "graphicsettings.h"

namespace phys
{

    /////////////////////////////////////
    // Functions for managing settings
    ////
    GraphicsSettings::GraphicsSettings()
    {
        Construct(640,480,false);
    }

    GraphicsSettings::GraphicsSettings( const PhysWhole &Width_, const PhysWhole &Height_, const bool &FullScreen_ )
    {
        Construct( Width_, Height_, FullScreen_ );
    }

    void GraphicsSettings::Construct( const PhysWhole &Width_, const PhysWhole &Height_, const bool &FullScreen_ )
    {
        this->Fullscreen = FullScreen_;
        this->RenderHeight = Height_;
        this->RenderWidth = Width_;
    }

    //returns: True if user wants fullscreen
    bool GraphicsSettings::getFullscreen() const
    {
        return this->Fullscreen;
    }

    //returns: false if changes could not be made
    void GraphicsSettings::setFullscreen(const bool &Fullscreen_)
    {
        //wierd checks go here to make sure it worked
        this->Fullscreen = Fullscreen_;
    }

    //Returns the height of the render window
    PhysWhole GraphicsSettings::getRenderHeight() const
    {
        return this->RenderHeight;
    }

    //Returns the width of the render window
    PhysWhole GraphicsSettings::getRenderWidth() const
    {
        return this->RenderWidth;
    }
    //returns: false if changes could not be made
    void GraphicsSettings::setRenderHeight(const PhysWhole &Height_)
    {
        this->RenderHeight = Height_;
    }

    //returns: false if changes could not be made
    void GraphicsSettings::setRenderWidth(const PhysWhole &Width_)
    {
        this->RenderWidth = Width_;
    }

}//namespace phys

#endif