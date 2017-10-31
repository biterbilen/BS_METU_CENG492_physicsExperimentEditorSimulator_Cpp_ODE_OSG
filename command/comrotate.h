#ifndef __COM_ROTATE_H__
#define __COM_ROTATE_H__

#include "command.h"
#include "sobject.h"

class ComRotate: public Command {
    osg::ref_ptr<SObject> affected;
    osg::Quat original;
    osg::Quat target;
public:
    ComRotate(SObject *node, osg::Vec3 quat_vec, float angle);
    virtual ~ComRotate() {}
    virtual void realExecute();
    virtual void undo();
};

#endif
