#ifndef __TOOL_MAKE_GEAR_H__
#define __TOOL_MAKE_GEAR_H__

#include "toolmakeshape.h"
#include "sgear.h"

class ToolMakeGear: public ToolMakeShape {
public:
    ToolMakeGear(): ToolMakeShape("make-gear") {}
    virtual SObject *makeShape() { return new SGear(1.0f, 8, getPos()); }
    virtual Tool* clone() { return new ToolMakeGear(); }
};

#endif
