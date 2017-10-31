#ifndef _TOOL_ZOOM_H
#define _TOOL_ZOOM_H 1

#include "drawingareatool.h"
#include "editor.h"
#include "cameraview.h"


class ToolZoom: public DrawingAreaTool {
    bool clicked;
    float y0;
    float y1;
public:
    ToolZoom(): DrawingAreaTool("zoom") { clicked=0; }
    virtual void mouseMoved(float x, float y, CameraView * camView);
    virtual void mousePressed(float x, float y, CameraView * camView);
    virtual void mouseReleased(float x, float y, CameraView * camView);
    virtual void init() { clicked=0; }
    virtual Tool* clone() { return new ToolZoom(); }

};

#endif

