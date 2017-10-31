#ifndef _S_SLIDER_JOINT_EDITOR_CALLBACK_H
#define _S_SLIDER_JOINT_EDITOR_CALLBACK_H 1

#include <osg/Vec3>
#include <osg/Quat>
#include <osg/MatrixTransform>
#include <osg/NodeVisitor>
#include <osg/NodeCallback>

#include "sslider.h"

class SSliderJointEditorCallback : public osg::NodeCallback
{
public:
    SSliderJointEditorCallback(SSlider * j, int type)
    {
        mJoint = j;
        mType = type;
    }

    virtual void operator () (osg::Node* node, osg::NodeVisitor* nv)
    {           
        osg::Vec3 geomPos1;              
        osg::Vec3 geomPos2;
        osg::Vec3 axis;        
        osg::Vec3 diff;

        geomPos1 = mJoint->getBody1()->getPosition();        
        geomPos2 = mJoint->getBody2()->getPosition();
        diff = (geomPos2 - geomPos1);
        axis = (geomPos2 - geomPos1);
        axis.normalize();
        mJoint->setAxis(axis);
        
        osg::Matrix m;
        m.makeScale(osg::Vec3(1, 1, diff.length() / 2));
        osg::Matrix m2;
        m2.makeRotate(osg::Vec3(0, 0, 1), axis);
       
        osg::Matrix m3;
        if(mType == 0)
        {
            m3.makeTranslate(geomPos1+diff * 1/4);
        }
        else {
            m3.makeTranslate(geomPos1+diff * 3/4);
        }

        osg::MatrixTransform *pat = dynamic_cast<osg::MatrixTransform *>(node);
        if( pat != 0L )
            pat->setMatrix(m * m2 * m3);

        traverse(node,nv);
    }

private:

    osg::ref_ptr<SSlider> mJoint;
    int mType;
};

#endif
