#include "sforceatrelpos.h"

#include <osg/ShapeDrawable>
#include <osg/Material>

string SForceAtRelPos::mCounter = "1";

SForceAtRelPos::SForceAtRelPos() : SForce(osg::Vec3(0, 0, 0))
{
    mType = SForce::FORCEATRELPOS;
    mPosition = osg::Vec3(0, 0, 0);
    mName = "ForceatRelativePosition" + mCounter;
    mCounter = incrementString(mCounter, mCounter.length()-1);
}

SForceAtRelPos::SForceAtRelPos(osg::Vec3 f, osg::Vec3 p) : SForce(f)
{
    mType = SForce::FORCEATRELPOS;
    mPosition = p;
    mName = "ForceatRelativePosition" + mCounter;
    mCounter = incrementString(mCounter, mCounter.length() - 1);
}


void SForceAtRelPos::createSimulationForce(/*dBodyID bodyid, dSpaceID space*/)
{
    if(mObject->isDynamic())
        dBodyAddForceAtRelPos(mObject->getBodyID(), mForce.x()   , mForce.y()   , mForce.z(),
                                                    mPosition.x(), mPosition.y(), mPosition.z());        
}

void SForceAtRelPos::setPosition(osg::Vec3 pos)
{
    mPosition = pos;    
}

osg::Vec3 SForceAtRelPos::getPosition() const
{
    return mPosition;
}

string SForceAtRelPos::getCounter(){
    return mCounter;
}
void SForceAtRelPos::setCounter(string s){
    mCounter = s;
}

