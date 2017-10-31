#ifndef _TOOL_MAKE_FLAT_MIRROR_H__
#define _TOOL_MAKE_FLAT_MIRROR_H__

#include "toolmakeshape.h"
#include "sflatmirror.h"

class ToolMakeFlatMirror: public ToolMakeShape {
public:
    ToolMakeFlatMirror(): ToolMakeShape("make-flat-mirror") {}
    virtual SObject *makeShape() { return new SFlatMirror(getPos(), 3, 4); }
    virtual Tool* clone() { return new ToolMakeFlatMirror(); }
};

#endif

