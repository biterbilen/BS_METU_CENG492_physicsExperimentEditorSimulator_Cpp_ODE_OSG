#ifndef _S_FORCEATPOS_H
#define _S_FORCEATPOS_H 1

#include <sforce.h>

/**
@author Sourcerer
*/
class SForceAtPos : public SForce
{
public:
    SForceAtPos();
    SForceAtPos(osg::Vec3 f, osg::Vec3 p);
  
    void setPosition(osg::Vec3 pos);
    osg::Vec3 getPosition() const;
    
    string getCounter();
    void setCounter(string);
    
    void createSimulationForce(/*dBodyID bodyid, dSpaceID space*/);
protected:
    ~SForceAtPos() {}
    osg::Vec3 mPosition;
    
    
    static string mCounter;
    
};

#endif
