#ifndef _S_OSG_GEODE_H
#define _S_OSG_GEODE_H 1

#include <osg/Geode>
#include "sobject.h"

/**
 * An OSG Geode class that contains a pointer to an SObject
 */
class SOsgGeode : public osg::Geode {

public:
    SOsgGeode();
        
    void setSObject(SObject * obj);
    SObject * getSObject();
    
protected:
    ~SOsgGeode();
      
private:
    osg::ref_ptr<SObject> mObj;
};

#endif

