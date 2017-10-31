#include "slinvelocity.h"

SLinVelocity::SLinVelocity() : SVelocity(osg::Vec3(0, 0, 0))
{
    mType = SVelocity::LINEAR;
}

SLinVelocity::SLinVelocity(osg::Vec3 f) : SVelocity(f)
{
    mType = SVelocity::LINEAR;
}

void SLinVelocity::createSimulationVelocity(dBodyID bodyID)
{
    if(bodyID != 0)
        dBodySetLinearVel(bodyID, mVelocity.x(), mVelocity.y(), mVelocity.z());
}

