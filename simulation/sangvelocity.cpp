#include "sangvelocity.h"

SAngVelocity::SAngVelocity() : SVelocity(osg::Vec3(0, 0, 0))
{
    mType = SVelocity::ANGULAR;
}

SAngVelocity::SAngVelocity(osg::Vec3 f) : SVelocity(f)
{
    mType = SVelocity::ANGULAR;
}

void SAngVelocity::createSimulationVelocity(dBodyID bodyID)
{
    if(bodyID != 0)
        dBodySetAngularVel(bodyID, mVelocity.x(), mVelocity.y(), mVelocity.z());
}

