﻿//© Copyright 2010 - 2011 BlackTopp Studios Inc.
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

#ifndef terrainmanager_cpp
#define terrainmanager_cpp

#include <vector>

#include "terrainmanager.h"
#include "meshterrain.h"

namespace Mezzanine
{
    template<> TerrainManager* Singleton<TerrainManager>::SingletonPtr = 0;

    TerrainManager::TerrainManager()
    {
        Priority = -32;
    }

    TerrainManager::~TerrainManager()
    {
        DestroyAllTerrains();
    }

    TerrainBase* TerrainManager::GetTerrainByIndex(const Whole& Index)
    {
        return Terrains.at(Index);
    }

    TerrainBase* TerrainManager::GetTerrainByName(const String& Name)
    {
        for( std::vector<TerrainBase*>::iterator it = Terrains.begin(); it != Terrains.end(); ++it )
        {
            if( Name == (*it)->GetName() )
                return (*it);
        }
        return NULL;
    }

    Whole TerrainManager::GetNumTerrains() const
    {
        return Terrains.size();
    }

    void TerrainManager::AddTerrain(TerrainBase* Terrain)
    {
        Terrains.push_back(Terrain);
        Terrain->AddToWorld();
    }

    void TerrainManager::RemoveTerrain(const Whole& Index)
    {
        std::vector<TerrainBase*>::iterator it = Terrains.begin() + Index;
        (*it)->RemoveFromWorld();
        Terrains.erase(it);
    }

    void TerrainManager::RemoveTerrain(TerrainBase* ToBeRemoved)
    {
        for( std::vector<TerrainBase*>::iterator it = Terrains.begin() ; it != Terrains.end() ; ++it )
        {
            if(ToBeRemoved == (*it))
            {
                (*it)->RemoveFromWorld();
                Terrains.erase(it);
                return;
            }
        }
    }

    void TerrainManager::RemoveAllTerrains()
    {
        /// @todo When adding more types of terrains, it should be remembered that code should be added to clear the extra vectors.
        if( Terrains.empty() )
            return;
        for( std::vector<TerrainBase*>::iterator it = Terrains.begin() ; it != Terrains.end() ; ++it )
            (*it)->RemoveFromWorld();
        Terrains.clear();
        MeshTerrains.clear();
    }

    void TerrainManager::DestroyTerrain(const Whole& Index)
    {
        std::vector<TerrainBase*>::iterator it = Terrains.begin() + Index;
        (*it)->RemoveFromWorld();
        delete (*it);
        Terrains.erase(it);
    }

    void TerrainManager::DestroyTerrain(TerrainBase* ToBeDestroyed)
    {
        for( std::vector<TerrainBase*>::iterator it = Terrains.begin() ; it != Terrains.end() ; ++it )
        {
            if(ToBeDestroyed == (*it))
            {
                (*it)->RemoveFromWorld();
                delete (*it);
                Terrains.erase(it);
                return;
            }
        }
    }

    void TerrainManager::DestroyAllTerrains()
    {
        if( Terrains.empty() )
            return;
        for( std::vector<TerrainBase*>::iterator it = Terrains.begin() ; it != Terrains.end() ; ++it )
        {
            (*it)->RemoveFromWorld();
            delete (*it);
        }
        Terrains.clear();
        MeshTerrains.clear();
    }

    MeshTerrain* TerrainManager::CreateMeshTerrain(const Vector3& InitPosition, const String& name, const String& file, const String& group)
    {
        /*MeshTerrain* Terrain = new MeshTerrain(InitPosition, name, file, group);
        MeshTerrains.push_back(Terrain);
        Terrain->AddTerrainToWorld();
        return Terrain;//*/
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Inherited from Managerbase
    void TerrainManager::Initialize()
    {
    }

    void TerrainManager::DoMainLoopItems()
    {
    }

    ManagerBase::ManagerTypeName TerrainManager::GetType() const
        { return ManagerBase::TerrainManager; }
}

#endif