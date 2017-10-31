#ifndef _DRAWING_AREA_VIEW_H
#define _DRAWING_AREA_VIEW_H 1

#include "toolview.h"
#include "smainwindow.h"

class SMainWindow;

class DrawingAreaView: public ToolView {
public:
    void mouseMoved(float x, float y, CameraView * camView);
    void mousePressed(float x, float y, CameraView * camView);
    void mouseReleased(float x, float y, CameraView * camView);
    void enable();
    virtual void setEditor(Editor *e);
    ToolView *clone() { return new DrawingAreaView(); }
};

#endif
