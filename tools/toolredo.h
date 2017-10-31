#ifndef __TOOL_REDO_H__
#define __TOOL_REDO_H__

#include "tool.h"
#include "history.h"

class ToolRedo: public Tool {
public:
    ToolRedo(): Tool("redo") {}
    virtual void init() {
	    History::redo();
    }

    virtual Tool* clone() { return new ToolRedo(); }
};

#endif
