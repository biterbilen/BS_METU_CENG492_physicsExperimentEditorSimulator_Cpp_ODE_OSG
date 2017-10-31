#include "svelocity.h"

SVelocity::SVelocity(osg::Vec3 f)
{
    mVelocity = f;
}

SVelocity::SVelocity()
{
    mVelocity = osg::Vec3(0, 0, 0);
}

SVelocity::VelocityTypes SVelocity::getType() const
{
    return mType;
} 

void SVelocity::setVelocity(osg::Vec3 f)
{
    mVelocity = f; 
}

osg::Vec3 SVelocity::getVelocity() const
{
    return mVelocity;
}
