#ifndef _S_ROPE_SIM_CALLBACK_H
#define _S_ROPE_SIM_CALLBACK_H 1

#include <osg/Vec3>
#include <osg/Quat>
#include <osg/PositionAttitudeTransform>
#include <osg/NodeVisitor>
#include <osg/NodeCallback>

#include <ode/ode.h>

using namespace osg;

class SRopeSimCallback : public osg::NodeCallback
{
public:
    SRopeSimCallback(dGeomID gid1, dGeomID gid2)
    {
        mGeomID1 = gid1;
        mGeomID2 = gid2;
    }            

    virtual void operator () (osg::Node* node, osg::NodeVisitor* nv)
    {
//         dQuaternion myquat;
        dReal* vec1;
        dReal* vec2;

        // get orientation of the geom
//         dGeomGetQuaternion(mGeomID, myquat);
//         osg::Quat rot(myquat[1],myquat[2],myquat[3],myquat[0]);

        // get position of the geom
        vec1 = const_cast<dReal*>(dGeomGetPosition(mGeomID1));
        vec2 = const_cast<dReal*>(dGeomGetPosition(mGeomID2));
        osg::Vec3 pos((vec1[0]+vec2[0])/2, (vec1[1]+vec2[1])/2, (vec1[2]+vec2[2])/2);
        
        osg::PositionAttitudeTransform *pat = dynamic_cast<osg::PositionAttitudeTransform *>(node);
        if( pat != 0L )
        {
            pat->setPosition(pos);
//             pat->setAttitude(rot);
        }

        traverse(node,nv);
    }

private:

    dGeomID mGeomID1;
    dGeomID mGeomID2;
};

#endif
