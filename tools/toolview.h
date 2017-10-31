#ifndef _TOOL_VIEW_H_
#define _TOOL_VIEW_H_ 1

#include <tool.h>
#include <osg/Referenced>

class Editor;

class ToolView: public osg::Referenced {
protected:
    osg::ref_ptr<Tool> activeTool;
public:
    virtual void enable()=0;
    virtual ToolView *clone()=0;
    virtual void setActiveTool(Tool *tool) { activeTool=tool; }
    virtual void setEditor(Editor *) {}
};

#endif
