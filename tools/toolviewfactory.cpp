//#include <config.h>
#include "toolviewfactory.h"

ToolViewFactory *ToolViewFactory::_instance=new ToolViewFactory();

ToolViewFactory::ToolViewFactory() { }

void ToolViewFactory::registerView(Tool *tool, ToolView *view) {
    views[tool->getName()]=view;
}

ToolView* ToolViewFactory::getView(Tool *tool) {
    if (views.find(tool->getName()) != views.end()) {
        return views[tool->getName()]->clone();
    } else {
        return 0;
    }
}
