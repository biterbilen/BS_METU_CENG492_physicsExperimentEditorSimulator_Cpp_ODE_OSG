#include "toolrotate.h"
#include <editor.h>
#include <comrotate.h>

#include <qmessagebox.h>

#include "sosggeode.h"


#define PI 3.141592654
#define FACTOR 2*PI

void ToolRotate::selected(osg::Node *node)
{

    SOsgGeode * p = dynamic_cast<SOsgGeode*>(node);
    if (p == 0)
    {
        std::cout << "toolRotate: Cannot convert osg::Node* to SOsgGeode*" << std::endl;
    }
    else {
        mObject = p->getSObject();
    }
}

void ToolRotate::mouseMoved(float x, float y, CameraView *)
{
    if (mObject.valid())
    {
        if (clicked)
        {
            y1=y;
            mAngle = (y1-y0)*FACTOR;
            osg::Quat q;
            q.makeRotate(mAngle, mRotAxis);

            if(mObject->getObjectType() == SObject::CONMIRROR ||
                mObject->getObjectType() == SObject::INCLINEDPLANE ||
                mObject->getObjectType() == SObject::CONVEXLENS ||
                mObject->getObjectType() == SObject::GEAR)
            {
                osg::Vec3 v = mRotAxis;
                v.normalize();
                  if(v != osg::Vec3(0,0,1))
                      return;
            }
            
            if(mOriginalRot.zeroRotation())
            {
                mObject->setOrientation(q);
            }
            else {
                osg::Quat quat = mOriginalRot * q;
                mObject->setOrientation(quat);
            }
        }
        else
        {
            y0=y;
        }
    }
}

void ToolRotate::mousePressed(float, float, CameraView * camView)
{
      if(mObject->getObjectType() == SObject::POOL)
     {
               QMessageBox::warning(0, "3DPhySims", "Cannot rotate this object!\n\n", "Ok");
               return;
    }
                
    osg::Matrix matrix = camView->getSceneView()->getViewMatrix();
    // We must remove the translation from modelview matrix
    matrix(3,0)=0.0f;
    matrix(3,1)=0.0f;
    matrix(3,2)=0.0f;
    osg::Matrix transform = osg::Matrix::inverse(matrix);
    mRotAxis=osg::Vec3(0,0,1)*transform;
    
    if (mObject.valid())
    {
        clicked=true;
        mOriginalRot= mObject->getOrientation();
        y1=y0;
    }
}

void ToolRotate::mouseReleased(float, float, CameraView *)
{
      if( mObject->getObjectType() == SObject::CONMIRROR ||
                mObject->getObjectType() == SObject::INCLINEDPLANE ||
               mObject->getObjectType() == SObject::CONVEXLENS ||
              mObject->getObjectType() == SObject::GEAR)
     {
          osg::Vec3 v = mRotAxis;
           v.normalize();
           if(v != osg::Vec3(0,0,1))
               return;
    }

    /* Build a command and apply it! */
    if (mObject.valid() && mAngle != 0)
    {
        mObject->setOrientation(mOriginalRot);
        
        mAngle = (y1-y0)*FACTOR;
        osg::ref_ptr<ComRotate> cmd=new ComRotate(mObject.get(), mRotAxis, mAngle);
        cmd->execute();
        clicked=false;
        y0=y1;
    }
}

