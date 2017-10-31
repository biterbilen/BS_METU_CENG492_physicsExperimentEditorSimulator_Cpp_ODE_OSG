#ifndef __TOOL_MAKE_SHAPE_H__
#define __TOOL_MAKE_SHAPE_H__

#include <osg/Geode>
#include <osg/PositionAttitudeTransform>
#include <osg/ShapeDrawable>
#include "drawingareatool.h"
#include "comaddchild.h"
#include "sobject.h"

/**
 * Abstract class for all the shape makers. Subclasses of this tool have
 * only to define a constructor and copy method as always, and
 * a makeShape() function to make the concrete shape.
 * A getPos() method is provided to subclases in order to know where in the
 * space the user want to put the shape.
 */
class ToolMakeShape: public DrawingAreaTool {
    osg::Vec3 pos;
protected:
    /// API for the subclasses to know where they may be put
    osg::Vec3 getPos() { return pos; }
public:
    ToolMakeShape(char *name): DrawingAreaTool(name) {}
    
    virtual void mouseMoved(float x, float y, CameraView * camView);
    virtual void mouseReleased(float x, float y, CameraView * camView);

    /// Template method to make the actual shape
    virtual SObject * makeShape()=0;
};


#endif
