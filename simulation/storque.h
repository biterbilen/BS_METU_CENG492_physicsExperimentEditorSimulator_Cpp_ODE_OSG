#ifndef _S_TORQUE_H
#define _S_TORQUE_H 1

#include <sforce.h>

/**
*/
class STorque : public SForce
{
public:
    STorque();
    STorque(osg::Vec3 f);
    
    void createSimulationForce();
    string getCounter();
    void setCounter(string);
    
    
protected:
    ~STorque() {}
    static string mCounter;

};

#endif
