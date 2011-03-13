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
#ifndef _meshgenerator_h
#define _meshgenerator_h

#include "vector3.h"

namespace phys
{
    ///////////////////////////////////////////////////////////////////////////////
    /// @class MeshGenerator
    /// @headerfile meshgenerator.h
    /// @brief This class allows the creation of graphical meshes from code.
    /// @details All meshes created by this class aren't retured, but are instead added to the list of meshes
    /// which can be referenced and used.  Simply use the same Mesh Name used to create the mesh when creating
    /// an actor or having to specify a mesh.
    ///////////////////////////////////////
    class MeshGenerator
    {
        protected:
        public:
            /// @brief Class constructor.
            MeshGenerator();
            /// @brief Class destructor.
            ~MeshGenerator();
            /// @brief Creates a box graphical mesh which can be used for actors.
            /// @param MeshName The name for the mesh which will be created.  Use this to reference the mesh when creating other objects that need a mesh.
            /// @param MaterialName The name of the material script which will be applied to this mesh.
            /// @param HalfExtents Half of the full dimentions of the final object in world units.  This allows the objects origin to be it's center.
            static void CreateBoxMesh(const String& MeshName, const String& MaterialName, const Vector3& HalfExtents);
            /// @brief Creates a cylinder graphical mesh which can be used for actors.
            /// @param MeshName The name for the mesh which will be created.  Use this to reference the mesh when creating other objects that need a mesh.
            /// @param MaterialName The name of the material script which will be applied to this mesh.
            /// @param HalfExtents Half of the full dimentions of the final object in world units.  This allows the objects origin to be it's center.
            /// @param AxisOrientation Vector3 representing which axis the cylinder should be aligned on.  Should be one of the three: (1,0,0), (0,1,0), (0,0,1).
            static void CreateCylinderMesh(const String& MeshName, const String& MaterialName, const Vector3& HalfExtents, const Vector3& AxisOrientation);
            /// @brief Creates a sphere graphical mesh which can be used for actors.
            /// @param MeshName The name for the mesh which will be created.  Use this to reference the mesh when creating other objects that need a mesh.
            /// @param MaterialName The name of the material script which will be applied to this mesh.
            /// @param Radius The radius to generate the sphere with in world units.
            /// @param Rings The number of horizontal rings the sphere is to be comprised of.
            /// This along with the segments parameter controls the overall resolution of the sphere.  Less then 16 is not recommended.
            /// @param Segments The number of vertical rings the sphere is to be comprised of.
            /// This along with the rings parameter controls the overall resolution of the sphere.  Less then 16 is not recommended.
            static void CreateSphereMesh(const String& MeshName, const String& MaterialName, const Real& Radius, const Real& Rings = 16, const Real& Segments = 16);
    };
}

#endif