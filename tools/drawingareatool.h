#ifndef __DRAWING_AREA_TOOL_H__
#define __DRAWING_AREA_TOOL_H__

#include "tool.h"

class DrawingAreaTool: public Tool {
public:
    virtual void mouseMoved(float, float, CameraView *) {}
    virtual void mousePressed(float, float, CameraView *) {}
    virtual void mouseReleased(float, float, CameraView *) {}
protected:
    DrawingAreaTool(char *name): Tool(name) {}
};

#endif
