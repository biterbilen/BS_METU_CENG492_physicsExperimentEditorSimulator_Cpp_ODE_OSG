#ifndef _S_LIN_VELOCITY_H
#define _S_LIN_VELOCITY_H 1

#include "svelocity.h"

/**@class SLinVelocity
 *@brief class for linear velocity
 */
class SLinVelocity : public SVelocity {
public:
    SLinVelocity();
    SLinVelocity(osg::Vec3 f);
        
    void createSimulationVelocity(dBodyID);
protected:
    ~SLinVelocity() {}
};

#endif
