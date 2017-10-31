#include "srelforce.h"

string SRelForce::mCounter = "1";

SRelForce::SRelForce() : SForce(osg::Vec3(0, 0, 0))
{
    mType = SForce::RELFORCE;
    mName = "RelativeForce" + mCounter;
    mCounter = incrementString(mCounter, mCounter.length()-1);
}
    
SRelForce::SRelForce(osg::Vec3 f) : SForce(f)
{
    mType = SForce::RELFORCE;
    mName = "RelativeForce" + mCounter;
    mCounter = incrementString(mCounter, mCounter.length()-1);
}
    
void SRelForce::createSimulationForce()
{
    if(mObject->isDynamic())
        dBodyAddRelForce(mObject->getBodyID(), mForce.x(), mForce.y(), mForce.z());    
}

string SRelForce::getCounter(){
    return mCounter;
}
void SRelForce::setCounter(string s){
    mCounter = s;
}

