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
#ifndef _worldquerytool_cpp
#define _worldquerytool_cpp

#include "worldquerytool.h"
#include "actorcontainerbase.h"

#include <Ogre.h>

namespace phys
{
    WorldQueryTool::WorldQueryTool(World* GameWorld_)
    {
        this->GameWorld = GameWorld_;
        this->MouseButtonCache.reset();
        this->KeyboardButtonCache.reset();
        this->MouseXCache=0;
        this->MouseYCache=0;

        // create the ray scene query object
        this->RayQuery = this->GameWorld->OgreSceneManager->createRayQuery(Ogre::Ray(), Ogre::SceneManager::WORLD_GEOMETRY_TYPE_MASK);
        if (NULL == this->RayQuery)
            {this->GameWorld->LogAndThrow("Failed to create RaySceneQuery instance in WorldQueryTool"); }
        this->RayQuery->setSortByDistance(true);
    }

    WorldQueryTool::~WorldQueryTool()
    {
        delete this->RayQuery;
    }

    Whole WorldQueryTool::GetMouseX()
        {return this->MouseXCache;}

    Whole WorldQueryTool::GetMouseY()
        {return this->MouseYCache;}

    bool WorldQueryTool::IsMouseButtonPushed(short unsigned int MouseButton)
    {
        if(MouseButton >= this->MouseButtonLimit)
            {this->GameWorld->LogAndThrow("Unsupported mouse button access through WorldQueryTool");}
        return this->MouseButtonCache[MouseButton];
    }

    bool WorldQueryTool::IsKeyboardButtonPushed(MetaCode::InputCode KeyboardButton)
        {return this->KeyboardButtonCache[KeyboardButton];}


    void WorldQueryTool::GatherEvents(bool ClearEventsFromEventMgr)
    {
        std::list<EventUserInput*>* UserInput = this->GameWorld->Events->GetAllUserInputEvents();   // Get the updated list of events
        if( ClearEventsFromEventMgr )
            { this->GameWorld->Events->RemoveAllSpecificEvents(EventBase::UserInput); }

        //For each metacode adjust any needed info
        for(std::list<EventUserInput*>::iterator Iter = UserInput->begin(); Iter!=UserInput->end(); Iter++) //for each event
        {
            for(unsigned int c = 0; c<(*Iter)->GetMetaCodeCount(); c++) //For each metacode in the event
            {                                                           //Newer Items should take precedence of older ones, so only store the oldest ones
                if( (*Iter)->GetMetaCode(c).IsKeyboardButton() ) //is it a Key
                {
                    if(0 <= (*Iter)->GetMetaCode(c).GetMetaValue()) //see MetaCode::ButtonState
                    {
                        this->KeyboardButtonCache.reset( (*Iter)->GetMetaCode(c).GetCode() );
                    }else{
                        this->KeyboardButtonCache.set( (*Iter)->GetMetaCode(c).GetCode() );
                    }
                }

                if ( MetaCode::MOUSEABSOLUTEHORIZONTAL == (*Iter)->GetMetaCode(c).GetCode() )
                    { this->MouseXCache = (*Iter)->GetMetaCode(c).GetMetaValue(); }

                if ( MetaCode::MOUSEABSOLUTEVERTICAL == (*Iter)->GetMetaCode(c).GetCode() )
                    { this->MouseYCache = (*Iter)->GetMetaCode(c).GetMetaValue(); }

                if ( MetaCode::MOUSEBUTTON == (*Iter)->GetMetaCode(c).GetCode() )
                {
                    if(0 <= (*Iter)->GetMetaCode(c).GetMetaValue()) //see MetaCode::ButtonState
                    {
                        this->MouseButtonCache.reset( (*Iter)->GetMetaCode(c).GetID() );
                    }else{
                        this->MouseButtonCache.set( (*Iter)->GetMetaCode(c).GetID() );
                    }
                }
               /// @todo Add support for joystick events to WorldQueryTool
            }
        }

        if( ClearEventsFromEventMgr )//Erase everything if we were asked to.
        {
            for(std::list<EventUserInput*>::iterator Iter = UserInput->begin(); !UserInput->empty(); Iter = UserInput->begin())
            {
                delete(*Iter);
                UserInput->remove(*Iter);
            }
        }
    } // \GatherEvents

    VectorWActor3* WorldQueryTool::GetFirstActorOnRayByPolygon(Ray ActorRay)
    {
        Ogre::Ray Ooray = ActorRay.GetOgreRay();

        if(NULL != this->RayQuery)          //Double check that the Rayquery is valid
        {
            this->RayQuery->setRay(Ooray);
            if( this->RayQuery->execute().size() <= 0 ) //Did we hit anything
            {
                return NULL;
            }
        }else{                          //Whoopsie something Failed
            this->GameWorld->LogAndThrow("Attempting to run a query on Null RaySceneQuery");
        }

        // at this point we have raycast to a series of different objects bounding boxes.
        // we need to test these different objects to see which is the first polygon hit.
        // there are some minor optimizations (distance based) that mean we wont have to
        // check all of the objects most of the time, but the worst case scenario is that
        // we need to test every triangle of every object.
        Ogre::Real closest_distance = -1.0f;
        Vector3 closest_result;
        Ogre::RaySceneQueryResult &query_result = this->RayQuery->getLastResults();
        VectorWActor3* ClosestActor = new VectorWActor3();
        for (size_t qr_idx = 0; qr_idx < query_result.size(); qr_idx++)
        {
            // stop checking if we have found a raycast hit that is closer than all remaining entities
            if ( (0.0f <= closest_distance) && (closest_distance < query_result[qr_idx].distance))
                { break; }

            // only check this result if its a hit against an entity
            if ((NULL != query_result[qr_idx].movable) && (0 == query_result[qr_idx].movable->getMovableType().compare("Entity")))
            {
                // get the entity to check
                Ogre::Entity *pentity = static_cast<Ogre::Entity*>(query_result[qr_idx].movable);

                // mesh data to retrieve
                size_t vertex_count;
                size_t index_count;
                Ogre::Vector3 *vertices;
                unsigned long *indices;

                // get the mesh information
                this->GetMeshInformation(pentity, vertex_count, vertices, index_count, indices,
                                  pentity->getParentNode()->_getDerivedPosition(),
                                  pentity->getParentNode()->_getDerivedOrientation(),
                                  pentity->getParentNode()->_getDerivedScale());

                // test for hitting individual triangles on the mesh
                bool new_closest_found = false;
                for (size_t i = 0; i < index_count; i += 3)
                {
                    // check for a hit against this triangle
                    std::pair<bool, Ogre::Real> hit = Ogre::Math::intersects(Ooray, vertices[indices[i]], vertices[indices[i+1]], vertices[indices[i+2]], true, false);

                    // if it was a hit check if its the closest
                    if (hit.first && ((0.0f > closest_distance) || (hit.second < closest_distance)) )
                    {
                        closest_distance = hit.second;                        // this is the closest so far, save it off
                        new_closest_found = true;
                    }
                }

                // free the verticies and indicies memory
                delete[] vertices;
                delete[] indices;

                // if we found a new closest raycast for this object, update the closest_result before moving on to the next object.
                if (new_closest_found)
                {
                    closest_result = Ooray.getPoint(closest_distance);
                    ClosestActor->Actor = this->GameWorld->Actors->FindActor( pentity->getParentNode() );
                }
            } // \if entity
        } // \if qr_idx

        //Change the closest point into a point relative to the Actor
        if (ClosestActor->Actor != NULL)
            { ClosestActor->Vector = closest_result - ClosestActor->Actor->GetLocation(); }
        return ClosestActor;
    }


    // Private Members
    void WorldQueryTool::GetMeshInformation( Ogre::Entity *entity,
                                size_t &vertex_count,
                                Ogre::Vector3* &vertices,
                                size_t &index_count,
                                unsigned long* &indices,
                                const Ogre::Vector3 &position,
                                const Ogre::Quaternion &orient,
                                const Ogre::Vector3 &scale)
    {
        bool added_shared = false;
        size_t current_offset = 0;
        size_t shared_offset = 0;
        size_t next_offset = 0;
        size_t index_offset = 0;
        vertex_count = index_count = 0;

        Ogre::MeshPtr mesh = entity->getMesh();


        bool useSoftwareBlendingVertices = entity->hasSkeleton();

        if (useSoftwareBlendingVertices)
        {                                   //10,000th line of code
            entity->_updateAnimation();
        }

        // Calculate how many vertices and indices we're going to need
        for (unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
        {
            Ogre::SubMesh* submesh = mesh->getSubMesh( i );

            // We only need to add the shared vertices once
            if(submesh->useSharedVertices)
            {
                if( !added_shared )
                {
                    vertex_count += mesh->sharedVertexData->vertexCount;
                    added_shared = true;
                }
            }
            else
            {
                vertex_count += submesh->vertexData->vertexCount;
            }

            // Add the indices
            index_count += submesh->indexData->indexCount;
        }


        // Allocate space for the vertices and indices
        vertices = new Ogre::Vector3[vertex_count];
        indices = new unsigned long[index_count];

        added_shared = false;

        // Run through the submeshes again, adding the data into the arrays
        for ( unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
        {
            Ogre::SubMesh* submesh = mesh->getSubMesh(i);

            //Ogre::VertexData* vertex_data = submesh->useSharedVertices ? mesh->sharedVertexData : submesh->vertexData;
            Ogre::VertexData* vertex_data;

            //When there is animation:
            if(useSoftwareBlendingVertices)
            {
                //    #ifdef BUILD_AGAINST_AZATHOTH
                //             vertex_data = submesh->useSharedVertices ? entity->_getSharedBlendedVertexData() : entity->getSubEntity(i)->_getBlendedVertexData();
                //    #else
                vertex_data = submesh->useSharedVertices ? entity->_getSkelAnimVertexData() : entity->getSubEntity(i)->_getSkelAnimVertexData();
                //    #endif
            }else{
                vertex_data = submesh->useSharedVertices ? mesh->sharedVertexData : submesh->vertexData;
            }

            if((!submesh->useSharedVertices)||(submesh->useSharedVertices && !added_shared))
            {
                if(submesh->useSharedVertices)
                {
                    added_shared = true;
                    shared_offset = current_offset;
                }

                const Ogre::VertexElement* posElem = vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);
                Ogre::HardwareVertexBufferSharedPtr vbuf = vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());
                unsigned char* vertex = static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));

                // There is _no_ baseVertexPointerToElement() which takes an Ogre::Real or a double
                //  as second argument. So make it float, to avoid trouble when Ogre::Real will
                //  be comiled/typedefed as double:
                //      Ogre::Real* pReal;
                float* pReal;

                for( size_t j = 0; j < vertex_data->vertexCount; ++j, vertex += vbuf->getVertexSize())
                {
                    posElem->baseVertexPointerToElement(vertex, &pReal);
                    Ogre::Vector3 pt(pReal[0], pReal[1], pReal[2]);
                    vertices[current_offset + j] = (orient * (pt * scale)) + position;
                }

                vbuf->unlock();
                next_offset += vertex_data->vertexCount;
            }


            Ogre::IndexData* index_data = submesh->indexData;
            size_t numTris = index_data->indexCount / 3;
            Ogre::HardwareIndexBufferSharedPtr ibuf = index_data->indexBuffer;

            bool use32bitindexes = (ibuf->getType() == Ogre::HardwareIndexBuffer::IT_32BIT);

            unsigned long*  pLong = static_cast<unsigned long*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
            unsigned short* pShort = reinterpret_cast<unsigned short*>(pLong);


            size_t offset = (submesh->useSharedVertices)? shared_offset : current_offset;
            size_t index_start = index_data->indexStart;
            size_t last_index = numTris*3 + index_start;

            if (use32bitindexes)
            {
                for (size_t k = index_start; k < last_index; ++k)
                    { indices[index_offset++] = pLong[k] + static_cast<unsigned long>( offset ); }
            }else{
                for (size_t k = index_start; k < last_index; ++k)
                    { indices[ index_offset++ ] = static_cast<unsigned long>( pShort[k] ) + static_cast<unsigned long>( offset ); }
            }

            ibuf->unlock();
            current_offset = next_offset;
        }
    }
}



#endif
