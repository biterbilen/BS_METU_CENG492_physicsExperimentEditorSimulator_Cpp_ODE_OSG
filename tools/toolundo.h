#ifndef __TOOL_UNDO_H__
#define __TOOL_UNDO_H__

#include "tool.h"
#include "history.h"

class ToolUndo: public Tool {
public:
    ToolUndo(): Tool("undo") {}
    virtual void init() {
	History::undo();
    }
    virtual Tool* clone() { return new ToolUndo(); }

};

#endif
