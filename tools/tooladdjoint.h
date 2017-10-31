#ifndef _TOOL_ADD_JOINT_H
#define _TOOL_ADD_JOINT_H 1

#include "drawingareatool.h"
#include "comaddjoint.h"
#include "sobject.h"
#include "sosggeode.h"

class ToolAddJoint: public DrawingAreaTool {
protected:
    osg::ref_ptr<SObject> mObj1;
    osg::ref_ptr<SObject> mObj2;
    bool mFirstOneSelected;
    osg::Vec2 mMouse;
public:
    ToolAddJoint(char * name): DrawingAreaTool(name) {}
    virtual void mousePressed(float, float, CameraView *);
    virtual void init();
    /** Template method to make the actual joint */
    virtual SJoint * makeJoint() = 0;
};

#endif

