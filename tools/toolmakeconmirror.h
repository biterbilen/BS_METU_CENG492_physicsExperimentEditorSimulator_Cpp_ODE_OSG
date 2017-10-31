#ifndef _TOOL_MAKE_CON_MIRROR_H__
#define _TOOL_MAKE_CON_MIRROR_H__

#include "toolmakeshape.h"
#include "sconmirror.h"

class ToolMakeConMirror: public ToolMakeShape {
public:
    ToolMakeConMirror(): ToolMakeShape("make-con-mirror") {}
    virtual SObject *makeShape() { return new SConMirror(getPos(), 2); }
    virtual Tool* clone() { return new ToolMakeConMirror(); }
};

#endif

