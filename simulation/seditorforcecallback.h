#ifndef _S_EDITOR_FORCE_CALLBACK_H
#define _S_EDITOR_FORCE_CALLBACK_H 1

#include <osg/PositionAttitudeTransform>
#include <osg/NodeVisitor>
#include <osg/NodeCallback>
#include "sforce.h"
#include "sobject.h"

class SEditorForceCallback : public osg::NodeCallback
{
public:
    SEditorForceCallback(SForce * f)
    {
        mForce = f;
    }

    virtual void operator () (osg::Node* node, osg::NodeVisitor* nv)
    {
        osg::PositionAttitudeTransform *pat = dynamic_cast<osg::PositionAttitudeTransform *>(node);
        if( pat != 0L )
        {
            switch(mForce->getType())
            {
                case SForce::FORCEATRELPOS:
                    {
                        osg::Matrix m(mForce->getObject()->getOrientation());
                        osg::Vec3 p = osg::Matrix::inverse(m) * mForce->getPosition();
            
                        pat->setPosition(p + mForce->getObject()->getPosition());
                    }
                    break;
                case SForce::NORMALFORCE:
                    {
                        pat->setPosition(mForce->getObject()->getPosition());
                    }
                    break;
                case SForce::FORCEATPOS:
                    {
                        pat->setPosition(mForce->getPosition());
                    }
                    break;
                case SForce::RELFORCE:
                    {            
                        pat->setPosition(mForce->getObject()->getPosition());
                        pat->setAttitude(mForce->getObject()->getOrientation());
                    }
                break;
                case SForce::RELFORCEATPOS:
                    {
                        pat->setPosition(mForce->getPosition());
                        pat->setAttitude(mForce->getObject()->getOrientation());
                    }
                    break;
                case SForce::RELFORCEATRELPOS:
                    {
                        osg::Matrix m(mForce->getObject()->getOrientation());
                        osg::Vec3 p = osg::Matrix::inverse(m) * mForce->getPosition();
            
                        pat->setPosition(p + mForce->getObject()->getPosition());
                        pat->setAttitude(mForce->getObject()->getOrientation());
                    }
                    break;
                case SForce::TORQUE:
                    {
                         pat->setPosition(mForce->getObject()->getPosition());
                    }
                    break;
                case SForce::RELTORQUE:
                    {                                 
                        pat->setPosition(mForce->getObject()->getPosition());
                        pat->setAttitude(mForce->getObject()->getOrientation());
                    }
                    break;
                default: 
                    break;
            }
        }

        traverse(node, nv);
    }

private:
    osg::ref_ptr<SForce> mForce;
};

#endif

