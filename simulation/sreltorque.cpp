#include "sreltorque.h"

string SRelTorque::mCounter = "1";

SRelTorque::SRelTorque() : SForce(osg::Vec3(0, 0, 0))
{
    mType = SForce::RELTORQUE;
    mName = "RelativeTorque" + mCounter;
    mCounter = incrementString(mCounter, mCounter.length()-1);
}

SRelTorque::SRelTorque(osg::Vec3 f) : SForce( f)
{
    mType = SForce::RELTORQUE;
    mName = "RelativeTorque" + mCounter;
    mCounter = incrementString(mCounter, mCounter.length()-1);
}

void SRelTorque::createSimulationForce()
{
    if(mObject->isDynamic())
        dBodyAddRelTorque(mObject->getBodyID(), mForce.x(), mForce.y(), mForce.z());    
}

string SRelTorque::getCounter(){
        return mCounter;
}
void SRelTorque::setCounter(string s){
        mCounter = s;
}

