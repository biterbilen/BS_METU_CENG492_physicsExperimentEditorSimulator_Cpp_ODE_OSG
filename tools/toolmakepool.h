#ifndef __TOOL_MAKE_POOL_H__
#define __TOOL_MAKE_POOL_H__

#include "toolmakeshape.h"
#include "spool.h"

class ToolMakePool: public ToolMakeShape {
public:
    ToolMakePool(): ToolMakeShape("make-pool") {}
    virtual SObject *makeShape() { return new SPool(getPos(), osg::Vec3(3, 3, 3)); }
    virtual Tool* clone() { return new ToolMakePool(); }
};

#endif
