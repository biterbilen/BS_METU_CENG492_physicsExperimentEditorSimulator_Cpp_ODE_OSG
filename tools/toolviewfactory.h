#ifndef __TOOL_VIEW_FACTORY__
#define __TOOL_VIEW_FACTORY__

//#include <config.h>
#include "toolview.h"
#include <string>
#include <map>

class ToolViewFactory
{
    typedef std::map<std::string, osg::ref_ptr<ToolView> > ToolViewMap;
    ToolViewMap views;
public:
    void registerView(Tool *tool, ToolView *view);
    ToolView* getView(Tool *tool);
    static ToolViewFactory *instance() { return _instance; }
private:
    static ToolViewFactory *_instance;
    ToolViewFactory();
};

#endif

