#ifndef __TOOL_BOX_H__
#define __TOOL_BOX_H__

#include "tool.h"
#include <map>
#include <string>

class ToolBox {
    typedef std::map<std::string, osg::ref_ptr<Tool> > ToolMap;
    ToolMap tools;
public:
    void registerTool(Tool *tool);
    Tool *getTool(std::string name);
    static ToolBox *instance() { return _instance; }
private:
    ToolBox();
    static ToolBox *_instance;
};

#endif
