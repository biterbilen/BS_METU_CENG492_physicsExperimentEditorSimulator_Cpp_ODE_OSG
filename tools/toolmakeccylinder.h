#ifndef TOOL_MAKE_CCYLINDER_H
#define TOOL_MAKE_CCYLINDER_H 1

#include "toolmakeshape.h"
#include "scappedcylinder.h"

class ToolMakeCCylinder: public ToolMakeShape {
public:
    ToolMakeCCylinder(): ToolMakeShape("make-ccylinder") {}
    virtual SObject *makeShape() { return new SCappedCylinder(0.5f, 0.5f, getPos()); }
    virtual Tool* clone() { return new ToolMakeCCylinder(); }
};

#endif
