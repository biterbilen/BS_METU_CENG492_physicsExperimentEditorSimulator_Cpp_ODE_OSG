#ifndef _HINGE_H
#define _HINGE_H 1

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
// 
using namespace std;
using namespace osg;

/**
 * @class SHinge
 * @brief Ode's hinge type joint
 */
class SHinge : public SJoint
{
public:
	SHinge();
    
	void setType();
	void setAnchor(osg::Vec3 a);
	void setAxis(osg::Vec3 a);
	osg::Vec3 getAnchor();
	osg::Vec3 getAxis();
 
    void createSimulationJoint(dWorldID worldid, dJointGroupID jointgroupid); 
    osg::Node * createVisualJoint();
    
    void createEditorJoint();
    
    string getCounter();
    void setCounter(string);
    
protected:
	~SHinge() {}
 
     static string mCounter;
 	osg::Vec3 mAnchor;	// anchor position
	osg::Vec3 mAxis; 	// initial orientation
};

#endif
