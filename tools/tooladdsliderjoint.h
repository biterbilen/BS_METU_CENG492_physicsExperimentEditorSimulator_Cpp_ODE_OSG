#ifndef _TOOL_ADD_SLIDER_JOINT_H
#define _TOOL_ADD_SLIDER_JOINT_H 1

#include "tooladdjoint.h"
#include "sslider.h"

class ToolAddSliderJoint: public ToolAddJoint {
public:
    ToolAddSliderJoint(): ToolAddJoint("add-slider-joint") {}
    virtual SJoint *makeJoint() { return new SSlider(); }
    virtual Tool* clone() { return new ToolAddSliderJoint(); }
};

#endif

