#ifndef __TOOL_ROTATE_H__
#define __TOOL_ROTATE_H__

#include "drawingareatool.h"
#include "sobject.h"


class ToolRotate: public DrawingAreaTool {
    osg::ref_ptr<SObject> mObject;
    bool clicked;
	osg::Quat mOriginalRot;
    osg::Vec3 mRotAxis;
    float mAngle;
    float y0;
    float y1;
public:
    ToolRotate(): DrawingAreaTool("rotate") { clicked=0; }
    virtual void selected(osg::Node *node);
    virtual void mouseMoved(float, float, CameraView *);
    virtual void mousePressed(float, float, CameraView *);
    virtual void mouseReleased(float, float, CameraView *);
    virtual void init() { clicked=0; }
    virtual Tool* clone() { return new ToolRotate(); }

};

#endif
