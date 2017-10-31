#ifndef _S_EDITOR_CALLBACK_H
#define _S_EDITOR_CALLBACK_H 1

#include <osg/Vec3>
#include <osg/Quat>
#include <osg/PositionAttitudeTransform>
#include <osg/NodeVisitor>
#include <osg/NodeCallback>

#include "sobject.h"

using namespace osg;

class SEditorCallback : public osg::NodeCallback
{
public:
    SEditorCallback(SObject * o)
    {
        obj = o;
    }

    virtual void operator () (osg::Node* node, osg::NodeVisitor* nv)
    {
        osg::PositionAttitudeTransform *pat = dynamic_cast<osg::PositionAttitudeTransform *>(node);
        if( pat != 0L )
        {
            pat->setPosition(obj->getPosition());
            pat->setAttitude(obj->getOrientation());
        }

        traverse(node, nv);
    }

    SObject * getObject() { return obj.get(); }

private:
    osg::ref_ptr<SObject> obj;
};

#endif

