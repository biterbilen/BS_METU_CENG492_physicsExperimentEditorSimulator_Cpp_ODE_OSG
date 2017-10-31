#ifndef __COM_TRANSLATE_H__
#define __COM_TRANSLATE_H__

#include "command.h"
#include "sobject.h"

class ComTranslate: public Command {
    osg::ref_ptr<SObject> affected;
    osg::Vec3 original;
    osg::Vec3 target;
public:
    ComTranslate(SObject * obj, osg::Vec3 newPos);
    virtual ~ComTranslate() {}
    virtual void realExecute();
    virtual void undo();
};

#endif
