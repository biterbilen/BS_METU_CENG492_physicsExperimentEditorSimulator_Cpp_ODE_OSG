#ifndef _S_RELTORQUE_H
#define _S_RELTORQUE_H 1

#include <sforce.h>

/**
*/
class SRelTorque : public SForce
{
public:
    SRelTorque();
    SRelTorque(osg::Vec3 f);

    void createSimulationForce();
    string getCounter();
    void setCounter(string);

protected:
    ~SRelTorque() {}
       static string mCounter;
};

#endif
