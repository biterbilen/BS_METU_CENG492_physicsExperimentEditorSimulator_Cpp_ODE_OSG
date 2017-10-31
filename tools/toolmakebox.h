#ifndef __TOOL_MAKE_BOX_H__
#define __TOOL_MAKE_BOX_H__

#include "toolmakeshape.h"
#include "sbox.h"

class ToolMakeBox: public ToolMakeShape {
public:
    ToolMakeBox(): ToolMakeShape("make-box") {}
    virtual SObject *makeShape() { return new SBox(1.0f, getPos()); }
    virtual Tool* clone() { return new ToolMakeBox(); }
};

#endif
