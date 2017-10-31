#ifndef _S_RELFORCE_H
#define _S_RELFORCE_H 1

#include <sforce.h>

/**
@author Sourcerer
*/
class SRelForce : public SForce
{
public:
    SRelForce();
    SRelForce(osg::Vec3 f);
   
    void createSimulationForce();

    string getCounter();
    void setCounter(string);
protected:
    ~SRelForce() {}
    static string mCounter;
};

#endif
