#ifndef _TOOL_MAKE_CONVEX_LENS_H__
#define _TOOL_MAKE_CONVEX_LENS_H__

#include "toolmakeshape.h"
#include "sconvexlens.h"

class ToolMakeConvexLens: public ToolMakeShape {
public:
    ToolMakeConvexLens(): ToolMakeShape("make-convex-lens") {}
    virtual SObject *makeShape() { return new SConvexLens(getPos(), 1, 0.2); }
    virtual Tool* clone() { return new ToolMakeConvexLens(); }
};

#endif

