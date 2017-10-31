#ifndef __TOOL_NEW_H__
#define __TOOL_NEW_H__

#include "tool.h"
#include <history.h>

class ToolNew: public Tool {
public:
    ToolNew(): Tool("new") {}
    virtual void init() {
	editor->getSceneGraph()->replaceScenegraph(new osg::Group());
    }
    virtual Tool* clone() { return new ToolNew(); }

};

#endif
