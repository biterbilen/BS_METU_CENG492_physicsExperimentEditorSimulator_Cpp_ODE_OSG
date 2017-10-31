#ifndef _S_SIM_CALLBACK_H
#define _S_SIM_CALLBACK_H 1

#include <osg/Vec3>
#include <osg/Quat>
#include <osg/PositionAttitudeTransform>
#include <osg/NodeVisitor>
#include <osg/NodeCallback>

#include <ode/ode.h>

using namespace osg;

class SSimCallback : public osg::NodeCallback
{
public:
    SSimCallback(dGeomID gid)
            : geom(gid)
    {}

    virtual void operator () (osg::Node* node, osg::NodeVisitor* nv)
    {
        dQuaternion myquat;
        dReal* vec;

        // get orientation of the geom
        dGeomGetQuaternion(geom, myquat);
        osg::Quat rot(myquat[1],myquat[2],myquat[3],myquat[0]);

        // get position of the geom
        vec = const_cast<dReal*>(dGeomGetPosition(geom));
        osg::Vec3 pos(vec[0],vec[1],vec[2]);
        
        osg::PositionAttitudeTransform *pat = dynamic_cast<osg::PositionAttitudeTransform *>(node);
        if( pat != 0L )
        {
            pat->setPosition(pos);
            pat->setAttitude(rot);
        }

        traverse(node,nv);
    }

    dGeomID geom;
};

#endif
