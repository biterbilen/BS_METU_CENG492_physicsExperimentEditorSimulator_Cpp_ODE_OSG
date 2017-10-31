#include "drawingareaview.h"
#include <drawingareatool.h>

void DrawingAreaView::mouseMoved(float x, float y, CameraView * camView) {
    DrawingAreaTool *tool=dynamic_cast<DrawingAreaTool*>(activeTool.get());
    tool->mouseMoved(x, y, camView);
}

void DrawingAreaView::mousePressed(float x, float y, CameraView * camView) {
    DrawingAreaTool *tool=dynamic_cast<DrawingAreaTool*>(activeTool.get());
    tool->mousePressed(x, y, camView);
}

void DrawingAreaView::mouseReleased(float x, float y, CameraView * camView) {
    DrawingAreaTool *tool=dynamic_cast<DrawingAreaTool*>(activeTool.get());
    tool->mouseReleased(x, y, camView);
}

void DrawingAreaView::enable() {}

void DrawingAreaView::setEditor(Editor *e) {
    SMainWindow * mainWindow=dynamic_cast<SMainWindow*>(e);
    mainWindow->registerDrawingAreaView(this);
}
