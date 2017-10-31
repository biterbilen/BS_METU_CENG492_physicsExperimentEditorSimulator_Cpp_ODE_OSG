#ifndef _S_RELFORCEATRELPOS_H
#define _S_RELFORCEATRELPOS_H 1

#include <sforce.h>

/**
@author Sourcerer
*/
class SRelForceAtRelPos : public SForce
{
public:
    SRelForceAtRelPos();
    SRelForceAtRelPos(osg::Vec3 f, osg::Vec3 p);    

    void createSimulationForce(/*dBodyID bodyid, dSpaceID space*/);
    
    void setPosition(osg::Vec3 pos);
    osg::Vec3 getPosition() const;
    
    string getCounter();
    void setCounter(string);
    
protected:
    ~SRelForceAtRelPos() {};
    osg::Vec3 mPosition;
    
    static string mCounter;

};

#endif
