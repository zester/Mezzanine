#ifndef _leveltriggers_cpp
#define _leveltriggers_cpp

#include "leveltriggers.h"

BNS_Fan::BNS_Fan(const String& name, Physics::HingeConstraint* Motor, ActorRigid* Button, ActorRigid* Fan, FieldOfForce* Wind, ParticleEffect* WindClutter)
    : WorldTrigger(name)
{
    this->Button = Button;
    this->Wind = Wind;
    this->Fan = Fan;
    this->Motor = Motor;
    this->WindClutter = WindClutter;
}

BNS_Fan::~BNS_Fan()
{
}

bool BNS_Fan::ConditionsAreMet()
{
    if(-107.5 > Button->GetLocation().Y)// -123 to -107, due to the 15 units of spring
    {
        Motor->EnableMotor(true,20.0,9000.0);
        return true;
    }else{
        Motor->EnableMotor(false,0.0,0.0);
        WindClutter->RemoveFromWorld();
        Wind->SetFieldStrength(0);
        return false;
    }
}

void BNS_Fan::ApplyTrigger()
{
    Real RotationSpeed = Fan->GetPhysicsSettings()->GetAngularVelocity().Y;
    // We want the max force applied to be ~120.0.  The rotation speed is on a scale from 0 to 12.
    // So we have to multiply the rotation by ~12 to get the proper force we want to apply for the motor.
    Real FieldStrength = (-RotationSpeed) * 12.0;
    Wind->SetFieldStrength(FieldStrength);
    if(FieldStrength > 60.0) WindClutter->AddToWorld();

    /*std::stringstream stream;
    stream << ;
    Entresol::GetSingletonPtr()->LogString(stream.str());
    Entresol::GetSingletonPtr()->DoMainLoopLogging();// */
}

Roll_Roll::Roll_Roll(const String& name, std::vector<Physics::HingeConstraint*>& TheRollers)
    : WorldTrigger(name)
{
    Rollers.swap(TheRollers);
    for( Whole X = 0 ; X < Rollers.size() ; ++X )
        Rollers[X]->GetActorA()->GetPhysicsSettings()->SetActivationState(Mezzanine::Physics::AS_DisableDeactivation);
}

Roll_Roll::~Roll_Roll()
{
}

void Roll_Roll::Rotate(Physics::HingeConstraint* RollerMotor)
{
    Real DeltaTime = Entresol::GetSingletonPtr()->GetLastFrameTimeMilliseconds();
    Real DeltaAngle = 1.25 * DeltaTime;
    Real CurrentAngle = RollerMotor->GetHingeAngle();
    Real ActualAngle = DeltaAngle + ( CurrentAngle>0 ? CurrentAngle : -CurrentAngle );
    RollerMotor->SetMotorTarget(-ActualAngle,DeltaTime);
}

bool Roll_Roll::ConditionsAreMet()
{
    return true;
}

void Roll_Roll::ApplyTrigger()
{
    for( Whole X = 0 ; X < Rollers.size() ; ++X )
        Rotate(Rollers[X]);
}

#endif
