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
#ifndef _ray_h
#define _ray_h

#include "datatypes.h"
#include "vector3.h"

namespace phys
{
    ///////////////////////////////////////////////////////////////////////////////
    /// @class Ray
    /// @headerfile ray.h
    /// @brief This is used to indicate a line with one end.
    /// @details Currently this is just a collection of two vector3s with a couple
    /// of constructors.
    class Ray
    {
        public:
            /// @brief The origin point of the Vector
            Vector3 From;

            /// @brief A point on the line of the ray.
            Vector3 To;

            /// @brief Default constructor
            /// @details This create a ray starting at 0,0,0 pointing to 0,1,0,
            Ray();

            /// @brief Thorough constructor
            /// @details This accepts 2 Vector3s and uses them to build the ray
            /// @param From_ The origin fo the new Ray
            /// @param To_ A point along the line for the destination line.
            Ray(Vector3 From_, Vector3 To_);


    };
}// /phys

std::ostream& operator << (std::ostream& stream, const phys::Ray& x);

/*Ogre::Vector3& operator << (Ogre::Vector3& VecTo, const phys::Vector3& VecFrom);
Ogre::Vector3& operator << (Ogre::Vector3& VecTo, const btVector3& VecFrom);

btVector3& operator << (btVector3& VecTo, const Ogre::Vector3& VecFrom);
btVector3& operator << (btVector3& VecTo, const phys::Vector3& VecFrom);

phys::Vector3& operator << (phys::Vector3& VecTo, const Ogre::Vector3& VecFrom);
phys::Vector3& operator << (phys::Vector3& VecTo, const btVector3& VecFrom);
*/
#endif