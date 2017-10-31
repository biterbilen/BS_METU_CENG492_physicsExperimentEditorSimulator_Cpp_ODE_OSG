#include "srelforceatrelpos.h"
#include "seditorcallback.h"

#include <osg/ShapeDrawable>
#include <osg/Material>

string SRelForceAtRelPos::mCounter = "1";

SRelForceAtRelPos::SRelForceAtRelPos() : SForce(osg::Vec3(0, 0, 0))
{
    mType = SForce::RELFORCEATRELPOS;
    mPosition = osg::Vec3(0, 0, 0);
    mName = "RelativeForceAtRelativePosition" + mCounter;
    mCounter = incrementString(mCounter, mCounter.length()-1);
}

SRelForceAtRelPos::SRelForceAtRelPos(osg::Vec3 f, osg::Vec3 p) : SForce(f)
{
    mType = SForce::RELFORCEATRELPOS;
    mPosition = p;
        mName = "RelativeForceAtRelativePosition" + mCounter;
    mCounter = incrementString(mCounter, mCounter.length()-1);
}

void SRelForceAtRelPos::createSimulationForce()
{
    if(mObject->isDynamic())
        dBodyAddRelForceAtRelPos(mObject->getBodyID(), mForce.x()   , mForce.y()   , mForce.z(),
                                                       mPosition.x(), mPosition.y(), mPosition.z());    
}

void SRelForceAtRelPos::setPosition(osg::Vec3 pos)
{
    mPosition = pos; 
}

osg::Vec3 SRelForceAtRelPos::getPosition() const
{
    return mPosition;
}

string SRelForceAtRelPos::getCounter(){
    return mCounter;
}
void SRelForceAtRelPos::setCounter(string s){
    mCounter = s;    
}

