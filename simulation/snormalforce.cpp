#include "snormalforce.h"

#include <osg/ShapeDrawable>
#include <osg/Material>

string SNormalForce::mCounter = "1";

SNormalForce::SNormalForce() : SForce(osg::Vec3(0, 0, 0))
{
    mType = SForce::NORMALFORCE;
    mName = "NormalForce" + mCounter;
    mCounter = incrementString(mCounter, mCounter.length()-1);    

}

SNormalForce::SNormalForce(osg::Vec3 f) : SForce(f)
{
    mType = SForce::NORMALFORCE;
    mName = "NormalForce" + mCounter;
    mCounter = incrementString(mCounter, mCounter.length()-1);
}


void SNormalForce::createSimulationForce()
{
    if(mObject->isDynamic())    
        dBodyAddForce(mObject->getBodyID(), mForce.x(), mForce.y(), mForce.z());                
}

string SNormalForce::getCounter()
{
    return mCounter;
}

void SNormalForce::setCounter(string counter)
{
    mCounter = counter;
}      


