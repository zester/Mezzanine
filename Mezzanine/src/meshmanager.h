//© Copyright 2010 - 2013 BlackTopp Studios Inc.
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
#ifndef _meshmanager_h
#define _meshmanager_h

#include "vector3.h"
#include "colourvalue.h"
#include "managerbase.h"
#include "managerfactory.h"
#include "singleton.h"

namespace Mezzanine
{
    namespace Graphics
    {
        class Mesh;
    }
    namespace Physics
    {
        class CollisionShape;
    }

    // Used by the scripting language binder to help create bindgings for this class. SWIG does know to creation template instances
    #ifdef SWIG
    %template(SingletonMeshManager) Singleton<MeshManager>;
    #endif

    ///////////////////////////////////////////////////////////////////////////////
    /// @class MeshManager
    /// @headerfile meshmanager.h
    /// @brief This manager handles the storage, generation, and query of of Graphics Meshes.
    /// @details
    ///////////////////////////////////////
    class MEZZ_LIB MeshManager : public ManagerBase, public Singleton<MeshManager>
    {
    public:
        typedef std::map< String, Graphics::Mesh* >    MeshContainer;
        typedef MeshContainer::iterator                MeshIterator;
        typedef MeshContainer::const_iterator          ConstMeshIterator;
    protected:
        MeshContainer GeneratedMeshes;
        MeshContainer LoadedMeshes;
    public:
        /// @brief Class constructor.
        MeshManager();
        /// @brief XML constructor.
        /// @param XMLNode The node of the xml document to construct from.
        MeshManager(XML::Node& XMLNode);
        /// @brief Class destructor.
        virtual ~MeshManager();

        ///////////////////////////////////////////////////////////////////////////////
        // Generic Mesh Utility

        /// @brief Gets a mesh stored in this manager.
        /// @return Returns a pointer to the requested mesh.
        /// @param MeshName The name of the mesh to retrieve.
        Graphics::Mesh* GetMesh(const String& MeshName);
        /// @brief Clears this manager of all meshes, both loaded and generated.
        void DestroyAllMeshes();

        ///////////////////////////////////////////////////////////////////////////////
        // Non-Generated Mesh Management

        /// @brief Loads a mesh file from disk and prepares it for use.
        /// @return Returns a pointer to the loaded mesh.
        /// @param MeshName The name of the mesh file to be loaded.
        /// @param Group The resource group from which the mesh file should be loaded.
        virtual Graphics::Mesh* LoadMesh(const String& MeshName, const String& Group);
        /// @brief Unloads a mesh file.
        /// @param MeshName The name of the mesh to be unloaded.
        virtual void UnloadMesh(const String& MeshName);
        /// @brief Gets the number of currently loaded mesh files.
        /// @return Returns a whole representing the number of mesh files currently loaded.
        virtual Whole GetNumLoadedMeshes();
        /// @brief Unloads every mesh that is currently loaded.
        virtual void UnloadAllLoadedMeshes();

        ///////////////////////////////////////////////////////////////////////////////
        // Generated Mesh Management

        /// @brief Gets the number of meshes this generator has created and are in use.
        /// @return Returns a Whole representing the number of meshes created by this generator.
        virtual Whole GetNumGeneratedMeshes();
        /// @brief Destroys a named Mesh, freeing it's resources.
        /// @param MeshName The name of the mesh to be destroyed.
        virtual void DestroyGeneratedMesh(const String& MeshName);
        /// @brief Destroys all the meshes generated by this generator.
        virtual void DestroyAllGeneratedMeshes();

        ///////////////////////////////////////////////////////////////////////////////
        // Mesh Generation

        /// @brief Creates a box graphical mesh.
        /// @return Returns a pointer to the created Mesh.
        /// @param MeshName The name for the mesh which will be created.  Use this to reference the mesh when creating other objects that need a mesh.
        /// @param MaterialName The name of the material script which will be applied to this mesh.
        /// @param HalfExtents Half of the full dimentions of the final object in world units.  This allows the objects origin to be it's center.
        virtual Graphics::Mesh* CreateBoxMesh(const String& MeshName, const String& MaterialName, const Vector3& HalfExtents);
        /// @brief Creates a box graphical mesh.
        /// @return Returns a pointer to the created Mesh.
        /// @param MeshName The name for the mesh which will be created.  Use this to reference the mesh when creating other objects that need a mesh.
        /// @param Colour The colour to generate the material with that will be applied to the mesh.  The created material's name will be autogenerated to "[Meshname]+Mat".
        /// @param HalfExtents Half of the full dimentions of the final object in world units.  This allows the objects origin to be it's center.
        virtual Graphics::Mesh* CreateBoxMesh(const String& MeshName, const ColourValue& Colour, const Vector3& HalfExtents);
        /// @brief Creates a cylinder graphical mesh.
        /// @return Returns a pointer to the created Mesh.
        /// @param MeshName The name for the mesh which will be created.  Use this to reference the mesh when creating other objects that need a mesh.
        /// @param MaterialName The name of the material script which will be applied to this mesh.
        /// @param HalfExtents Half of the full dimentions of the final object in world units.  This allows the objects origin to be it's center.
        /// @param AxisOrientation Vector3 representing which axis the cylinder should be aligned on.  Should be one of the three: (1,0,0), (0,1,0), (0,0,1).
        /// @param CircleRes The number of segments the circle should be comprised of.  Determines the "resolution" of the cylinder.
        /// @param Segments Optional parameter to specify the number of segments the cylinder should be comprised of.  Mostly just useful if a special material is made for his.
        virtual Graphics::Mesh* CreateCylinderMesh(const String& MeshName, const String& MaterialName, const Vector3& HalfExtents, const Vector3& AxisOrientation, const Whole& CircleRes = 16, const Whole& Segments = 1);
        /// @brief Creates a cylinder graphical mesh and simple material.
        /// @return Returns a pointer to the created Mesh.
        /// @param MeshName The name for the mesh which will be created.  Use this to reference the mesh when creating other objects that need a mesh.
        /// @param Colour The colour to generate the material with that will be applied to the mesh.  The created material's name will be autogenerated to "[Meshname]+Mat".
        /// @param HalfExtents Half of the full dimentions of the final object in world units.  This allows the objects origin to be it's center.
        /// @param AxisOrientation Vector3 representing which axis the cylinder should be aligned on.  Should be one of the three: (1,0,0), (0,1,0), (0,0,1).
        /// @param CircleRes The number of segments the circle should be comprised of.  Determines the "resolution" of the cylinder.
        /// @param Segments Optional parameter to specify the number of segments the cylinder should be comprised of.  Mostly just useful if a special material is made for his.
        virtual Graphics::Mesh* CreateCylinderMesh(const String& MeshName, const ColourValue& Colour, const Vector3& HalfExtents, const Vector3& AxisOrientation, const Whole& CircleRes = 16, const Whole& Segments = 1);
        /// @brief Creates a sphere graphical mesh.
        /// @return Returns a pointer to the created Mesh.
        /// @param MeshName The name for the mesh which will be created.  Use this to reference the mesh when creating other objects that need a mesh.
        /// @param MaterialName The name of the material script which will be applied to this mesh.
        /// @param Radius The radius to generate the sphere with in world units.
        /// @param Rings The number of horizontal rings the sphere is to be comprised of.
        /// This along with the segments parameter controls the overall resolution of the sphere.  Less then 16 is not recommended.
        /// @param Segments The number of vertical rings the sphere is to be comprised of.
        /// This along with the rings parameter controls the overall resolution of the sphere.  Less then 16 is not recommended.
        virtual Graphics::Mesh* CreateSphereMesh(const String& MeshName, const String& MaterialName, const Real& Radius, const Real& Rings = 16, const Real& Segments = 16);
        /// @brief Creates a sphere graphical mesh and simple material.
        /// @return Returns a pointer to the created Mesh.
        /// @param MeshName The name for the mesh which will be created.  Use this to reference the mesh when creating other objects that need a mesh.
        /// @param Colour The colour to generate the material with that will be applied to the mesh.  The created material's name will be autogenerated to "[Meshname]+Mat".
        /// @param Radius The radius to generate the sphere with in world units.
        /// @param Rings The number of horizontal rings the sphere is to be comprised of.
        /// This along with the segments parameter controls the overall resolution of the sphere.  Less then 16 is not recommended.
        /// @param Segments The number of vertical rings the sphere is to be comprised of.
        /// This along with the rings parameter controls the overall resolution of the sphere.  Less then 16 is not recommended.
        virtual Graphics::Mesh* CreateSphereMesh(const String& MeshName, const ColourValue& Colour, const Real& Radius, const Real& Rings = 16, const Real& Segments = 16);
        /// @brief Creates a mesh composed of boxes that outline the corner edges of a larger box.
        /// @return Returns a pointer to the created Mesh.
        /// @param MeshName The name for the mesh which will be created.  Use this to reference the mesh when creating other objects that need a mesh.
        /// @param MaterialName The name of the material script which will be applied to this mesh.
        /// @param HalfExtents Half of the full dimentions of the final object in world units.  This allows the objects origin to be it's center.
        /// @param BoxThickness The width/thickness of the smaller boxes that will outline the corners of the larger box.
        virtual Graphics::Mesh* CreateBoxCornerMesh(const String& MeshName, const String& MaterialName, const Vector3& HalfExtents, const Real& BoxThickness);
        /// @brief Creates a mesh composed of boxes that outline the corner edges of a larger box.
        /// @return Returns a pointer to the created Mesh.
        /// @param MeshName The name for the mesh which will be created.  Use this to reference the mesh when creating other objects that need a mesh.
        /// @param Colour The colour to generate the material with that will be applied to the mesh.  The created material's name will be autogenerated to "[Meshname]+Mat".
        /// @param HalfExtents Half of the full dimentions of the final object in world units.  This allows the objects origin to be it's center.
        /// @param BoxThickness The width/thickness of the smaller boxes that will outline the corners of the larger box.
        virtual Graphics::Mesh* CreateBoxCornerMesh(const String& MeshName, const ColourValue& Colour, const Vector3& HalfExtents, const Real& BoxThickness);

        /// @brief Generates a mesh based on a collision shape.
        /// @remarks This is just a convenience function.  You can fetch the information and build the mesh yourself with greater flexability then what
        /// this function has to offer.  This function only supports building the shapes this manager can generate, and it will only use default options
        /// when deciding the parameters to use(if it's a parameter it can't get from the shape).
        /// @return Returns a pointer to the created mesh.
        /// @param MeshName The name that will be given to the generated mesh.
        /// @param MaterialName The name of the material to use with this mesh.
        /// @param Shape The shape to base the mesh on.
        virtual Graphics::Mesh* CreateMeshFromShape(const String& MeshName, const String& MaterialName, Physics::CollisionShape* Shape);

        ///////////////////////////////////////////////////////////////////////////////
        // Material Utilities - Until we can get a Material Manager

        /// @brief Creates a basic material in code using the provided colour.
        /// @return Returns a string containing the name of the created Material.
        /// This is actually the same string as whats passed in as the MatName,
        /// just here as convenience when calling the function as an argument in other functions.
        /// @param MatName The name to assign to the created material.
        /// @param Colour The colour to assign to the created material.
        /// @param Group The resource group where to place this material.  Will be placed in an internal resouce group if left blank.
        virtual const String& CreateColouredMaterial(const String& MatName, const ColourValue& Colour, const String& Group = "");

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @copydoc ManagerBase::Initialize()
        virtual void Initialize();
        /// @copydoc ManagerBase::Deinitialize()
        virtual void Deinitialize();

        ///////////////////////////////////////////////////////////////////////////////
        // Type Identifier Methods

        /// @copydoc ManagerBase::GetInterfaceType()
        virtual ManagerType GetInterfaceType() const;
        /// @copydoc ManagerBase::GetImplementationTypeName()
        virtual String GetImplementationTypeName() const;
    };//MeshManager

    ///////////////////////////////////////////////////////////////////////////////
    /// @class DefaultMeshManagerFactory
    /// @headerfile meshmanager.h
    /// @brief A factory responsible for the creation and destruction of the default meshmanager.
    ///////////////////////////////////////
    class MEZZ_LIB DefaultMeshManagerFactory : public ManagerFactory
    {
    public:
        /// @brief Class constructor.
        DefaultMeshManagerFactory();
        /// @brief Class destructor.
        virtual ~DefaultMeshManagerFactory();

        /// @copydoc ManagerFactory::GetManagerTypeName()
        String GetManagerTypeName() const;

        /// @copydoc ManagerFactory::CreateManager(NameValuePairList&)
        ManagerBase* CreateManager(NameValuePairList& Params);
        /// @copydoc ManagerFactory::CreateManager(XML::Node&)
        ManagerBase* CreateManager(XML::Node& XMLNode);
        /// @copydoc ManagerFactory::DestroyManager(ManagerBase*)
        void DestroyManager(ManagerBase* ToBeDestroyed);
    };//DefaultMeshManagerFactory
}//Mezzanine

#endif
