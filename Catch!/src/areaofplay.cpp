#ifndef _areaofplay_cpp
#define _areaofplay_cpp

#include "areaofplay.h"
#include <physgame.h>

AreaOfPlay::AreaOfPlay(const String &name, Vector3 Location) : AreaEffect(name, Location)
{

}

AreaOfPlay::~AreaOfPlay()
{

}

void AreaOfPlay::ApplyEffect()
{

}

bool AreaOfPlay::IsInside(ActorBase* Actor)
{
    for( std::list<ActorBase*>::iterator it = OverlappingActors.begin() ; it != OverlappingActors.end() ; it++ )
    {
        if( Actor == (*it) )
        {
            return true;
        }
    }
}

#endif