#include "sforceatpos.h"

#include <osg/ShapeDrawable>
#include <osg/Material>

string SForceAtPos::mCounter = "1";

SForceAtPos::SForceAtPos() : SForce(osg::Vec3(0, 0, 0))
{
    mType = SForce::FORCEATPOS;
    mPosition = osg::Vec3(0, 0, 0);
    mName = "ForceAtPosition" + mCounter;
    mCounter = incrementString(mCounter, mCounter.length()-1);
}

SForceAtPos::SForceAtPos(osg::Vec3 f, osg::Vec3 p) : SForce(f)
{
    mType = SForce::FORCEATPOS;
    mPosition = p;
    mName = "ForceAtPosition" + mCounter;
    mCounter = incrementString(mCounter, mCounter.length()-1);  
}

void SForceAtPos::createSimulationForce()
{
    if(mObject->isDynamic())
        dBodyAddForceAtPos(mObject->getBodyID(), mForce.x()   , mForce.y()   , mForce.z(),
                                                 mPosition.x(), mPosition.y(), mPosition.z());    
}

void SForceAtPos::setPosition(osg::Vec3 pos)
{
    mPosition = pos; 
}

osg::Vec3 SForceAtPos::getPosition() const
{
    return mPosition;
}

string SForceAtPos::getCounter(){
    return mCounter;
}
void SForceAtPos::setCounter(string s){
    mCounter = s;
}

