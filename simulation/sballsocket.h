#ifndef _BALL_SOCKET_H
#define _BALL_SOCKET_H 1

#include <string>
#include <ode/ode.h>
#include <osg/Vec3>
#include <osg/ShapeDrawable>
#include <osg/Material>

#include "sjoint.h"

/**
 * @class SBallSocket
 * @brief Ode's ball and socket type joint
 */
class SBallSocket : public SJoint
{
public:
	SBallSocket();

	void setType();
	void setAnchor(osg::Vec3 a);	    
    osg::Vec3 getAnchor();
    
    void createSimulationJoint(dWorldID world, dJointGroupID jointgroup);    
    osg::Node * createVisualJoint();
    
    void createEditorJoint();
        
    string getCounter();
    void setCounter(string);

protected:
 	~SBallSocket() {}

	osg::Vec3 mAnchor; // anchor position    
    static string mCounter;
};

#endif
