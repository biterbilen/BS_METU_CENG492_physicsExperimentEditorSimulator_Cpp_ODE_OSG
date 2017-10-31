#include "toolzoom.h"
#include <editor.h>

#include "sosggeode.h"

#define FACTOR 0.1

void ToolZoom::mouseMoved(float x, float y, CameraView * camView)
{
    if (clicked)
    {
        y1=y;
        float zoom = camView->getZoom() + (y1-y0) * FACTOR;
        
        if(zoom >= 0.01 && zoom <= 1)
            camView->setZoom( zoom );

        y0=y1;
    }
}

void ToolZoom::mousePressed(float x, float y, CameraView * camView)
{
    clicked=true;
    y1=y0=y;
}

void ToolZoom::mouseReleased(float x, float y, CameraView * camView)
{
    clicked=false;
    y0=y1;
}

