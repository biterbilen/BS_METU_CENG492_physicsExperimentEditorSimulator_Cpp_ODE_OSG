#ifndef _S_FORCEATRELPOS_H
#define _S_FORCEATRELPOS_H 1

#include <sforce.h>

/**
@author Sourcerer
*/
class SForceAtRelPos : public SForce
{
public:
    SForceAtRelPos();
    SForceAtRelPos(osg::Vec3 f, osg::Vec3 p);

    void createSimulationForce(/*dBodyID bodyid, dSpaceID space*/);
    
    void setPosition(osg::Vec3 pos);
    osg::Vec3 getPosition() const;
    string getCounter();
    void setCounter(string);
    
protected:
    ~SForceAtRelPos() {}
    osg::Vec3 mPosition;
    
    static string mCounter;


};

#endif
