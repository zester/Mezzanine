#ifndef _startingarea_cpp
#define _startingarea_cpp

#include "startingarea.h"
#include "physgame.h"

StartingArea::StartingArea(const String &name, Vector3 Location) : AreaEffect(name, Location)
{
    Grav.X = 0;
    Grav.Y = 0;
    Grav.Z = 0;
}

StartingArea::~StartingArea()
{

}

void StartingArea::ApplyEffect()
{
    ActorBase* Act = NULL;
    PhysicsManager* PhysMan = TheWorld->GetPhysicsManager();
    if(!AddedActors.empty())
    {
        for( std::vector<ActorBase*>::iterator it = AddedActors.begin() ; it != AddedActors.end() ; it++ )
        {
            Act = (*it);
            Act->GetPhysicsSettings()->DisableCollisionResponse();
            PhysMan->SetIndividualGravity(Act, Grav);
        }
    }
    if(!RemovedActors.empty())
    {
        for( std::vector<ActorBase*>::iterator it = RemovedActors.begin() ; it != RemovedActors.end() ; it++ )
        {
            Act = (*it);
            Act->GetPhysicsSettings()->EnableCollisionResponse();
            PhysMan->SetIndividualGravity(Act, PhysMan->GetGravity());
        }
    }
}

bool StartingArea::IsEmpty()
{
    return OverlappingActors.empty();
}

#endif
