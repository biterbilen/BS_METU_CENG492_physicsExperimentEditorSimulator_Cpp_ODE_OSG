#ifndef _S_ANG_VELOCITY_H
#define _S_ANG_VELOCITY_H 1

#include <svelocity.h>

/**@class SAngVelocity
 *@brief class for angular velocity
 */
class SAngVelocity : public SVelocity
{
public:
    SAngVelocity();
    SAngVelocity(osg::Vec3 f);
    
    void createSimulationVelocity(dBodyID);
protected:
    ~SAngVelocity() {}
};

#endif

