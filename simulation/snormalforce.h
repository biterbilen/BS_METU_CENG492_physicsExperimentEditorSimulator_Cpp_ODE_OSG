#ifndef _S_NORMALFORCE_H
#define _S_NORMALFORCE_H 1

#include <sforce.h>

/**
*/
class SNormalForce : public SForce{
public:
    SNormalForce();
    SNormalForce(osg::Vec3 f);

    void createSimulationForce();
    
    string getCounter();
    void setCounter(string);
protected:
    ~SNormalForce() {}
    
     static string mCounter;

};

#endif
