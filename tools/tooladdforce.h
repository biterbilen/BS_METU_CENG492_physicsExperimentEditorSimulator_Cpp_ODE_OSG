#ifndef _TOOL_ADD_FORCE_H
#define _TOOL_ADD_FORCE_H 1

#include "drawingareatool.h"
#include "comaddforce.h"
#include "sobject.h"
#include "sosggeode.h"

class ToolAddForce: public DrawingAreaTool {
    osg::ref_ptr<SObject> mObj;
    osg::Vec2 mMouse;
public:
    ToolAddForce(char * name): DrawingAreaTool(name) {}
    virtual void mousePressed(float, float, CameraView *);
    
    /** Template method to make the actual force */
    virtual SForce * makeForce() = 0;
};

#endif

