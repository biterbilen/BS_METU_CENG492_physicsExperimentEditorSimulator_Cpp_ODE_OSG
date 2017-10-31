#include "storque.h"

string STorque::mCounter = "1";

STorque::STorque() : SForce(osg::Vec3(0, 0, 0))
{
    mType = SForce::TORQUE;
    
    mName = "NormalTorque" + mCounter;
    mCounter = incrementString(mCounter, mCounter.length()-1);
}

STorque::STorque(osg::Vec3 f) : SForce(f)
{
    mType = SForce::TORQUE;
    
    mName = "NormalTorque" + mCounter;
    mCounter = incrementString(mCounter, mCounter.length()-1);
}

void STorque::createSimulationForce()
{
    if(mObject->isDynamic())
        dBodyAddTorque(mObject->getBodyID(), mForce.x(), mForce.y(), mForce.z());    
}

string STorque::getCounter(){
    return mCounter;
}
void STorque::setCounter(string s){
    mCounter = s;
}

