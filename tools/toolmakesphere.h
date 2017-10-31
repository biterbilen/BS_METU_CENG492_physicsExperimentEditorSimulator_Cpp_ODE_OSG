#ifndef __TOOL_MAKE_SPHERE_H__
#define __TOOL_MAKE_SPHERE_H__

#include "toolmakeshape.h"
#include "ssphere.h"

class ToolMakeSphere: public ToolMakeShape {
public:
    ToolMakeSphere(): ToolMakeShape("make-sphere") {}
    virtual SObject *makeShape() { return new SSphere(0.5f, getPos()); }
    virtual Tool* clone() { return new ToolMakeSphere(); }
};

#endif
