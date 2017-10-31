#ifndef _SLIDER_H
#define _SLIDER_H 1

#include <string>

#include <osg/Vec3>
#include <osg/Quat>
#include <osg/PositionAttitudeTransform>
#include <osg/NodeVisitor>
#include <osg/NodeCallback>
#include <osg/ShapeDrawable>
#include <osg/Material>

#include <ode/ode.h>

#include "sjoint.h"

/**
 * @class SSlider
 * @brief Ode's slider type joint
 */
class SSlider:public SJoint
{
public:
	SSlider();

	void setAxis(osg::Vec3 a);	   
    osg::Vec3 getAxis();
    
    void createSimulationJoint(dWorldID worldid, dJointGroupID jointgroupid); 
    osg::Node * createVisualJoint();
    
     void createEditorJoint();
    
    string getCounter();
    void setCounter(string);
    
protected:
	~SSlider() {}
 
 	osg::Vec3 mAxis;		// axis direction
    static string mCounter;
};

#endif
