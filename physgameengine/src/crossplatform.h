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
#ifndef _crossplatform_h
#define _crossplatform_h
///////////////////////////////////////////////////////////////////////////////
/// @namespace phys::crossplatform
/// @brief All functionality that needs different implemenations per platform will go in here
/// @details If we did our jobs right You not need to change anything to compile on
/// different platforms exvept the build target. If you want, the platform can
/// be manually defined in this section and this should be the only place that you
/// need to change to compile this on a supported platform. Just remark all the
/// lines that are not your platform using "//" and unremark your platform.
/// \n\n
/// Should you choose to port this to your platform, make sure that all the
/// required libraries are installed, then make sure to write an implementation
/// for each of the functions in crossplatform.cpp, then you should get to the
/// nitty gritty of making the minor platforms inconsistencies work.
/// \n\n
/// For most games there will be no need to directly call these functions, however if you decide
/// you game is an exception, there is one key thing to remember about all of these functions.
/// All of these may perform/behave slightly differently.
///////////////////////////////////////////////////////////////////////////////

/// @todo Make main page in documentation with wiki link and lisitng of required steps (build target and OS define), and dependencies required to build this.

//Unremark one of these when
//#define LINUX
//#define MACOSX
//#define WINDOWS

//End Choose Your Platform
///////////////////////////////////////////////////////////////////////////////

// Includes
#include "physdatatypes.h"

#include <string>

using namespace std;

class PhysWorld;

namespace phys
{
    //Forward Declarations should go here

    namespace crossplatform
    {
        /// @internal
        /// @brief Returns a string with a path/filename to the default Plugins config file
        /// @details Plugins.cfg is the file that determines which graphics plugins are loaded.
        /// This is a feature of the graphics subsystem and is generally not needed outside
        /// of engine code.
        /// @return A string which contains the path and filename of the plugins file
        string GetPluginsDotCFG();

        /// @internal
        /// @brief Returns a string with a path/filename to the default Graphics Subsytem settings file
        /// @details Settings.cfg is the file that determines how graphics settings are configured by default.
        /// This is a feature of the graphics subsystem and is generally not needed outside of engine code.
        /// @return A string which contains the path and filename of the graphics setts file
        string GetSettingsDotCFG();

        //Gets the default locatio for Game Data

        /// @brief Gets the Default Data Directory
        /// @details The directory returned by this function can be used to easily graphics objects. In general
        /// the Graphics subsystem can easily open files in this location with just their filename
        /// @return A string containing the path to the default Data Directory.
        string GetDataDirectory();


        //This returns a named parameter list with valid settings to use Ogre rendering
        // on a pre-existing SDL context
        /// @internal
        /// @brief This creates a data structure that can help SDL(User Input Subsystem) with Ogre(graphics subsystem)
        /// @details This creates a data structure that can help SDL(User Input Subsystem) with Ogre(graphics subsystem)
        /// @warning This is an engine internal, and shouldn't be used anywhere else. For all practical purposes is return gibberish
        void* GetSDLOgreBinder();

        //A simple crossplatform sleep function, unused, prefer SDLsleep instead
        /// @brief Pauses the program for a given period of time.
        /// @details Pauses the program for a given period of time.
        /// @param WaitTime The amount of time in milliseconds to wait
        void WaitMilliseconds(PhysWhole WaitTime);

        //Handles the actual cross platform swapping of buffers.
        /// @brief Renders the current world contents to the screen.
        /// @details This makes use of PhysWorld internals to Render to the screen, So it is advised against calling this directly.
        /// Currently there is no known issue with calling this directly, but it is not thread safe and is run during the main loop
        /// at the aproppriate times.
        /// @param TheWorld This is a pointer to the PhysWorld to be rendered.
        void RenderPhysWorld(PhysWorld *TheWorld);
    }
}
#endif