#ifndef _gamebase_cpp
#define _gamebase_cpp
///////////////////////////////////////////////////////////////////////////////
// Gamewide Logic misc Features go here
///////////////////////////////////////////////////////////////////////////////

#include "gamebase.h"       //Game Include
#include <physgame.h>       //Physgame include
#include <sstream>          //STL includes

using namespace phys;

//Create the World Globally! and set it to hold some actors
World TheWorld( Vector3(-10000.0,-10000.0,-10000.0), Vector3(10000.0,10000.0,10000.0), 30);

const Plane PlaneOfPlay( Vector3(2.0,1.0,-5.0), Vector3(1.0,2.0,-5.0), Vector3(1.0,1.0,-5.0));

Generic6DofConstraint* Dragger=NULL;

int main(int argc, char **argv)
{
    // Set the Title
    TheWorld.SetWindowName("Catch!... The Game!");
    TheWorld.SetTargetFrameRate(40);

    //Give the world functions to run before and after input and physics
    TheWorld.CallBacks->SetPreInput(&PreInput);
    TheWorld.CallBacks->SetPostInput(&PostInput);
    TheWorld.CallBacks->SetPrePhysics(&PrePhysics);
    TheWorld.CallBacks->SetPostPhysics(&PostPhysics);

    //give the World our function to execute after renderingg
    TheWorld.CallBacks->SetPostRender(&PostRender);

    //Set the Make the RenderWindow and load system stuff
	TheWorld.GameInit(false);

    //Set up polling for the letter Q and middle mouse button, and the mouse X and Y locations
    TheWorld.Events->AddPollingCheck( MetaCode(0, 1, MetaCode::KEY_q) );
    TheWorld.Events->AddPollingCheck( MetaCode(0, 3, MetaCode::MOUSEBUTTON) );
    TheWorld.Events->AddPollingCheck( MetaCode(0, 0, MetaCode::MOUSEABSOLUTEHORIZONTAL) );

    //Actually Load the game stuff
    LoadContent();

    //Configure the wireframe Drawer
    TheWorld.Physics->SetDebugPhysicsWireCount(2);
    TheWorld.Physics->SetDebugPhysicsRendering(0);

    //Setup some camera tricks
    String CameraNode = TheWorld.Cameras->CreateOrbitingNode( Vector3(0,0,0), Vector3(0.0,200.0,750.0) );
    TheWorld.Cameras->AttachCameraToNode(CameraNode);

	//Start the Main Loop
	TheWorld.MainLoop();

	return 0;
}

bool PostRender()
{
	//Lets set a variable for the time
	static Whole gametime = 0;

	TheWorld.Log(String("---------- Starting PosterRender CallBack -------------"));
    TheWorld.Log(String("Current Game Time "));

	//getting a message from the event manager)
	EventRenderTime* CurrentTime = TheWorld.Events->PopNextRenderTimeEvent();

    Whole LastFrame = CurrentTime->getMilliSecondsSinceLastFrame();

    // Is CurrentTime a valid event?
    while(0 != CurrentTime)
    {
        TheWorld.Log(gametime);
        gametime+=CurrentTime->getMilliSecondsSinceLastFrame();

        delete CurrentTime;
        CurrentTime = TheWorld.Events->GetNextRenderTimeEvent();
    }

    //Play around with the title bar
    std::stringstream timestream;
    timestream << "Catch!... " << gametime;
    TheWorld.SetWindowName( timestream.str() );

    ActorBase* Act1 = TheWorld.Actors->FindActor("RobotWayUpFrontLeft");
    ActorBase* Act2 = TheWorld.Actors->FindActor("RobotWayUpFrontRight");
    if (Act1->IsAnimated())
    {
        Act1->AdvanceAnimation((Real)0.001 * LastFrame);
    }

    if (Act2->IsAnimated())
    {
        Act2->AdvanceAnimation((Real)0.0001 * LastFrame);
    }

    if (1000<gametime && 1040>gametime)
    {
        Sound* Welcome = NULL;
        Welcome = TheWorld.Sounds->GetSoundByName("Welcome");
        if(Welcome)
        {
            Welcome->Play2d(false);
        }
    }

    // Turn on the Wireframe
    if (30000<gametime)
        { TheWorld.Physics->SetDebugPhysicsRendering(1); }

    //IF the game has gone on for 150 or more seconds close it.
	if (150000<gametime || (TheWorld.Events->GetNextQuitEvent()!=0) )
        { return false; }

    return true;
}

bool PrePhysics()
{
    TheWorld.Log("Object Locations");
    //Replace this with something that uses the actor container and logs the location of everything
    TheWorld.Log(TheWorld.Actors->FindActor("MetalSphere2")->GetLocation());
    return true;
}

bool PostPhysics()
{
    //ActorSoft* ActS = static_cast< ActorSoft* > (TheWorld.Actors->FindActor("MetalSphere2"));
    //ActS->UpdateSoftBody();
    //ActorSoft* ActS2 = static_cast< ActorSoft* > (TheWorld.Actors->FindActor("Robot9"));
    //ActS2->UpdateSoftBody();
    return true;
}

bool PreInput()
{
    return true;
}

bool PostInput()
{
    //User Input through a WorldQueryTool
    static WorldQueryTool Queryer(&TheWorld);

    Queryer.GatherEvents();
    TheWorld.Log("Mouse location From WorldQueryTool X/Y");
    TheWorld.Log(Queryer.GetMouseX());
    TheWorld.Log(Queryer.GetMouseY());

/*    if(320>Queryer.GetMouseX() && Queryer.IsMouseButtonPushed(3))
        {TheWorld.Cameras->IncrementYOrbit(-0.01, TheWorld.Cameras->GetNodeAttachedToCamera() );}

    if(320<Queryer.GetMouseX() && Queryer.IsMouseButtonPushed(3))
        {TheWorld.Cameras->IncrementYOrbit(0.01, TheWorld.Cameras->GetNodeAttachedToCamera() );}*/

    if( Queryer.IsKeyboardButtonPushed(MetaCode::KEY_LEFT) )
        { TheWorld.Cameras->IncrementYOrbit(-0.01, TheWorld.Cameras->GetNodeAttachedToCamera() ); }

    if( Queryer.IsKeyboardButtonPushed(MetaCode::KEY_RIGHT) )
        { TheWorld.Cameras->IncrementYOrbit(0.01, TheWorld.Cameras->GetNodeAttachedToCamera() ); }

    if( Queryer.IsKeyboardButtonPushed(MetaCode::KEY_UP) )
        { TheWorld.Cameras->ZoomCamera( -12.0 ); }

    if( Queryer.IsKeyboardButtonPushed(MetaCode::KEY_DOWN) )
        { TheWorld.Cameras->ZoomCamera( 12.0 ); }

    if( Queryer.IsKeyboardButtonPushed(MetaCode::KEY_SPACE) )
        { TheWorld.Cameras->ResetZoom(); }

    if( Queryer.IsKeyboardButtonPushed(MetaCode::KEY_m) )
        {
            Sound* Theme = TheWorld.Sounds->GetSoundByName("Theme2");
            if(!Theme->IsPlaying())
            {
                Theme->Play2d(false);
            }
        }

    // Make a declaration for a static constrain so it survives the function lifetime
    // static *constraint MouseDragger = 0;

    if( Queryer.IsMouseButtonPushed(1) )
    {
        TheWorld.Log("Begin Mouse Dragging");
        Ray *MouseRay = Queryer.GetMouseRay();
        TheWorld.Log("MouseRay");
        TheWorld.Log(*MouseRay);
        TheWorld.Log("PlaneOfPlay");
        TheWorld.Log(PlaneOfPlay);


        Vector3WActor *ClickOnActor = Queryer.GetFirstActorOnRayByPolygon( *MouseRay );

        bool firstframe=false;
        if (0 == ClickOnActor || 0 == ClickOnActor->Actor)
        {
            TheWorld.Log("No Actor Clicked on");
        }else{
            TheWorld.Log("Actor Clicked on");
            TheWorld.Log(*ClickOnActor);
            if(!(ClickOnActor->Actor->IsStaticOrKinematic()))
            {
                if(!Dragger)
                {
                    if(ClickOnActor->Actor->GetType()==ActorBase::Actorrigid)
                    {
                        Vector3 LocalPivot = ClickOnActor->Actor->GetLocation();
                        LocalPivot.Inverse();
                        Vector3 Clickloc = ClickOnActor->Vector;
                        LocalPivot = LocalPivot * Clickloc;
                        ActorRigid* rigid = static_cast<ActorRigid*>(ClickOnActor->Actor);
                        Dragger = new Generic6DofConstraint(rigid, LocalPivot, Quaternion(0,0,0,0), false);
                        Dragger->SetLinearLowerLimit(Vector3(0.f,0.f,0.f));
                        Dragger->SetLinearUpperLimit(Vector3(0.f,0.f,0.f));
                        Dragger->SetAngularLowerLimit(Vector3(0.f,0.f,0.f));
                        Dragger->SetAngularUpperLimit(Vector3(0.f,0.f,0.f));
                        TheWorld.Physics->AddConstraint(Dragger);
                        Dragger->SetParam(4,0.8,0);
                        Dragger->SetParam(4,0.8,1);
                        Dragger->SetParam(4,0.8,2);
                        Dragger->SetParam(4,0.8,3);
                        Dragger->SetParam(4,0.8,4);
                        Dragger->SetParam(4,0.8,5);
                        Dragger->SetParam(2,0.1,0);
                        Dragger->SetParam(2,0.1,1);
                        Dragger->SetParam(2,0.1,2);
                        Dragger->SetParam(2,0.1,3);
                        Dragger->SetParam(2,0.1,4);
                        Dragger->SetParam(2,0.1,5);
                        firstframe=true;
                    }else{
                        TheWorld.Log("Actor is not an ActorRigid.  Aborting.");
                    }
                }
            }else{
                TheWorld.Log("Actor is Static or Kinematic.  Aborting.");
            }
        }
        Vector3 *DragTo = Queryer.RayPlaneIntersection(*MouseRay, PlaneOfPlay);
        if (0 == DragTo)
        {
            TheWorld.Log("PlaneOfPlay Not Clicked on");
        }else{
            if(Dragger && !firstframe)
            {
                TheWorld.Log("Dragged To");
                TheWorld.Log(*DragTo);
                Dragger->SetOffsetALocation(*DragTo);
            }
        }

        if ( DragTo )
            { delete DragTo; }

        if ( MouseRay )
            { delete MouseRay; }

    }else{
        if(Dragger)
        {
            TheWorld.Physics->RemoveConstraint(Dragger);
            delete Dragger;
            Dragger=NULL;
        }
    }

    // using the Raw Event Manager, and deleting the events
    if( !CheckForEsc() )
        return false;
    return true;
}

///////////////////
//Non-Callbacks
bool CheckForEsc()
{
    //this will either set the pointer to 0 or return a valid pointer to work with.
    EventUserInput* OneInput = TheWorld.Events->PopNextUserInputEvent();

    //We check each Event
    while(0 != OneInput)
    {
        TheWorld.Log("Input Events Processed");

        //we check each MetaCode in each Event
        for (int c=0; c<OneInput->GetMetaCodeCount(); c++ )
        {
            TheWorld.Log(OneInput->GetMetaCode(c));
            //Is the key we just pushed ESCAPE
            if(MetaCode::KEY_ESCAPE == OneInput->GetMetaCode(c).GetCode())
            {
                return false;
            }
        }

        delete OneInput;
        OneInput = TheWorld.Events->PopNextUserInputEvent();
    }

    return true;
}

void LoadContent()
{
    ActorRigid *object1, *object2, *object3, *object4;
    ActorRigid *object5, *object6;
    ActorRigid *object7;
    //Ogre Setup Code
    String groupname ("Group1");
    String filerobot ("robot.mesh");
    String robotprefix ("Robot");

    Real mass=5.0;
    TheWorld.AddResourceLocation(crossplatform::GetDataDirectory(), "FileSystem", groupname, false);
    TheWorld.DeclareResource(filerobot, "Mesh", groupname);
    //TheWorld.DeclareResource("Examples.material", "Material", groupname);
    //TheWorld.DeclareResource("Plane.material", "Material", groupname);
    //TheWorld.DeclareResource("Sphere_Wood.material", "Material", groupname);
    //TheWorld.DeclareResource("Sphere_Metal.material", "Material", groupname);
    TheWorld.InitResourceGroup(groupname);

    // Now Lets make some bowling pins
    Real PinSpacing=75.0;           //This is how far apart we want the pins
    for(unsigned int c=0; c<4; c++)     //make the back row
    {
        std::stringstream namestream;
        namestream << robotprefix << c;
        TheWorld.Actors->AddActor( new ActorRigid (mass,namestream.str(),filerobot,groupname,&TheWorld) );
        TheWorld.Actors->LastActorAdded()->CreateShapeFromMeshDynamic(4);
        TheWorld.Actors->LastActorAdded()->SetInitLocation(Vector3( (-2.0*PinSpacing)+(c*PinSpacing), -100.0, 0));
    }

    for(unsigned int c=0; c<3; c++)     //the row with three pins
    {
        std::stringstream namestream;
        namestream << robotprefix << (c+4);
        TheWorld.Actors->AddActor( new ActorRigid (mass,namestream.str(),filerobot,groupname,&TheWorld) );
        TheWorld.Actors->LastActorAdded()->CreateShapeFromMeshDynamic(3);
        TheWorld.Actors->LastActorAdded()->SetInitLocation(Vector3( (-1.5*PinSpacing)+(c*PinSpacing), -66.0, -PinSpacing));
    }

    for(unsigned int c=0; c<2; c++)     //the row with 2 pins
    {
        std::stringstream namestream;
        namestream << robotprefix << (c+7);
        TheWorld.Actors->AddActor( new ActorRigid (mass,namestream.str(),filerobot,groupname,&TheWorld) );
        TheWorld.Actors->LastActorAdded()->CreateShapeFromMeshDynamic(2);
        TheWorld.Actors->LastActorAdded()->SetInitLocation(Vector3( (-PinSpacing)+(c*PinSpacing), -33.0, -PinSpacing*2));
    }

    std::stringstream namestream;           //make the front pin
    namestream << robotprefix << 9;
    TheWorld.Actors->AddActor( new ActorRigid (mass,namestream.str(),filerobot,groupname,&TheWorld) );
    TheWorld.Actors->LastActorAdded()->CreateShapeFromMeshDynamic(1);
    TheWorld.Actors->LastActorAdded()->SetInitLocation(Vector3( (-0.5*PinSpacing), 0.0, -PinSpacing*3));
    //ActorSoft* Act9 = static_cast < ActorSoft* > (TheWorld.Actors->LastActorAdded());
    //Act9->SetInitLocation(Vector3( (-0.5*PinSpacing), 0.0, -PinSpacing*3));

    object5 = new ActorRigid (0,"Plane","Plane.mesh",groupname,&TheWorld);
    object5->CreateShapeFromMeshStatic();
    object5->SetInitLocation(Vector3(0.0,-100,-300.0));

    object6 = new ActorRigid (0,"Ramp","Plane.mesh",groupname,&TheWorld);
    object6->CreateShapeFromMeshStatic();
    object6->SetInitLocation(Vector3(00.0,300.0,-1100.0));
    object6->SetInitOrientation(Quaternion(0.5, 0.0, 0.0, -0.25));

    object1 = new ActorRigid (mass,"RobotWayUpFrontRight",filerobot,groupname,&TheWorld);
    object1->CreateShapeFromMeshDynamic(1);
    object1->SetInitLocation(Vector3(400,70,100));
    object1->SetInitOrientation(Quaternion(0.5, 0.5, 0.0, 0.9));
    object1->SetAnimation("Idle", true);
    object1->EnableAnimation(true);

    object2 = new ActorRigid (150.0f,"WoodSphere","Sphere_Wood.mesh",groupname,&TheWorld);
    object2->CreateSphereShapeFromMesh();
    object2->SetActorScaling(Vector3(0.5,0.5,0.5));
    object2->SetInitLocation(Vector3(-140.0,2800.0,-1150.0));

    object3 = new ActorRigid (200.0f,"MetalSphere","Sphere_Metal.mesh",groupname,&TheWorld); Schedule :
    object3->CreateSphereShapeFromMesh();
    object3->SetActorScaling(Vector3(0.7,0.7,0.7));
    object3->SetInitLocation(Vector3(150.0,1800.0,-1300.0));

    object4 = new ActorRigid (mass,"RobotWayUpFrontLeft",filerobot,groupname,&TheWorld);
    object4->CreateShapeFromMeshDynamic(4);
    object4->SetInitLocation(Vector3(-400,10, 100));
    object4->SetInitOrientation(Quaternion(0.5, 0.5, 0.0, 0.9));
    object4->SetAnimation("Idle", true);
    object4->EnableAnimation(true);

    object7 = new ActorRigid (200.0f,"MetalSphere2","Sphere_Metal.mesh",groupname,&TheWorld);
    object7->CreateSphereShapeFromMesh();
    object7->SetActorScaling(Vector3(0.3,0.3,0.3));
    object7->SetInitLocation(Vector3(10.0,25000.0,-1300.0));

    //Final Steps
    Vector3 grav;
    grav.X=0.0;
    grav.Y=-10000.0;
    grav.Z=0.0;

    TheWorld.Actors->AddActor(object1);
    TheWorld.Actors->AddActor(object2);
    TheWorld.Actors->AddActor(object3);
    TheWorld.Actors->AddActor(object4);
    TheWorld.Actors->AddActor(object5);
    TheWorld.Actors->AddActor(object6);
    TheWorld.Actors->AddActor(object7);

    Sound *sound1, *music1, *music2;
    TheWorld.Sounds->CreateSoundSet("Announcer");
    sound1 = TheWorld.Sounds->CreateSound("Welcome", "data/common/sounds/welcomefun-1.ogg", false);
    TheWorld.Sounds->AddSoundToSoundSet("Announcer", sound1);

    TheWorld.Sounds->CreateSoundSet("SoundTrack");
    music1 = TheWorld.Sounds->CreateSound("Theme1", "data/common/music/cAudioTheme1.ogg", true);
    TheWorld.Sounds->AddSoundToSoundSet("SoundTrack", music1);
    music2 = TheWorld.Sounds->CreateSound("Theme2", "data/common/music/cAudioTheme2.ogg", true);
    TheWorld.Sounds->AddSoundToSoundSet("SoundTrack", music2);

    TheWorld.Log("Actor Count");
    TheWorld.Log( TheWorld.Actors->GetActorCount() );

    TheWorld.Physics->SetGravity(grav);
    TheWorld.Physics->SetSoftGravity(grav);
}
#endif
