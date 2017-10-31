#include "toolbox.h"

ToolBox::ToolBox() { }

ToolBox *ToolBox::_instance=new ToolBox();

void ToolBox::registerTool(Tool *tool) {
    tools[tool->getName()]=tool;
}

Tool *ToolBox::getTool(std::string name) {
    if (tools.find(name) != tools.end()) {
        return tools[name]->clone();
    } else {
        return 0;
    }
}
