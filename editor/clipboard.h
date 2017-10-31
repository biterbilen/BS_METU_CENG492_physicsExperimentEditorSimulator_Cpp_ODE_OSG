#ifndef __CLIPBOARD_H__
#define __CLIPBOARD_H__

#include <osg/Node>

class Clipboard {
private:
    static osg::Node *contents;

public:
    static osg::Node *getContents() { return contents; }
    static void setContents(osg::Node *node) { contents=node; }
};

#endif

