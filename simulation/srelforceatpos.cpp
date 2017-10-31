#include "srelforceatpos.h"


string SRelForceAtPos::mCounter = "1"; 

SRelForceAtPos::SRelForceAtPos() : SForce(osg::Vec3(0, 0, 0))
{
    mType = SForce::RELFORCEATPOS;
    mPosition = osg::Vec3(0, 0, 0);
    
    mName = "RelativeForceatPosition" + mCounter;
    mCounter = incrementString(mCounter, mCounter.length()-1);
}

SRelForceAtPos::SRelForceAtPos(osg::Vec3 f, osg::Vec3 p) : SForce(f)
{
    mType = SForce::RELFORCEATPOS;
    mPosition = p;
    
    mName =  "RelativeForceatPosition" + mCounter;
    mCounter = incrementString(mCounter, mCounter.length()-1);
}

void SRelForceAtPos::createSimulationForce(/*dBodyID bodyid, dSpaceID space*/)
{
    if(mObject->isDynamic())
        dBodyAddRelForceAtPos(mObject->getBodyID(), mForce.x()   , mForce.y()   , mForce.z(),
                                                    mPosition.x(), mPosition.y(), mPosition.z());    
}

void SRelForceAtPos::setPosition(osg::Vec3 pos)
{
    mPosition = pos;    
}

osg::Vec3 SRelForceAtPos::getPosition() const
{
    return mPosition;
}

string SRelForceAtPos::getCounter()
{
    return mCounter;
}

void SRelForceAtPos::setCounter(string s)
{
    mCounter = s;
}
