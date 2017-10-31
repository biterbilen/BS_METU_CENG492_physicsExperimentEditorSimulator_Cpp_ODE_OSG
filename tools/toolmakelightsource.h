#ifndef _TOOL_MAKE_LIGHT_SOURCE_H__
#define _TOOL_MAKE_LIGHT_SOURCE_H__

#include "toolmakeshape.h"
#include "slight.h"

class ToolMakeLightSource: public ToolMakeShape {
public:
    ToolMakeLightSource(): ToolMakeShape("make-light-source") {}
    virtual SObject *makeShape() { return new SLight(getPos()); }
    virtual Tool* clone() { return new ToolMakeLightSource(); }
};

#endif

