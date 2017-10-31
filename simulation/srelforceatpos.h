#ifndef _S_RELFORCEATPOS_H
#define _S_RELFORCEATPOS_H 1

#include <sforce.h>

/**
@author Sourcerer
*/
class SRelForceAtPos : public SForce
{
public:
    SRelForceAtPos();
    SRelForceAtPos(osg::Vec3 f, osg::Vec3 p);   
    void createSimulationForce(/*dBodyID bodyid, dSpaceID space*/);
    
    void setPosition(osg::Vec3 pos);
    osg::Vec3 getPosition() const;

    string getCounter();
    void setCounter(string);
protected:
    ~SRelForceAtPos() {}
    osg::Vec3 mPosition;
    
    static string mCounter;
    
};

#endif
