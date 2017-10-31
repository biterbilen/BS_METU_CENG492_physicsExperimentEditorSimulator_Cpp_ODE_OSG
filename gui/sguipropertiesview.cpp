#include "sguipropertiesview.h"
#include "sguipropertiesviewcontent.h"

#include "sconmirrorwidget.h"
#include "sconvexlenswidget.h"
#include "sflatmirrorwidget.h"
#include "slightsourcewidget.h"
#include "sgearwidget.h"
#include "spoolwidget.h"
#include "sboxwidget.h"
#include "sspherewidget.h"
#include "strimeshwidget.h"
#include "scappedcylinderwidget.h"
#include "sinclinedplanewidget.h"
#include "senvwidget.h"

#include "sballsocketjointwidget.h"
#include "shingejointwidget.h"
#include "shinge2jointwidget.h"
#include "ssliderjointwidget.h"
#include "suniversaljointwidget.h"

#include "svelocitywidget.h"
#include "snormaltorquewidget.h"
#include "snormalforcewidget.h"
#include "snormalforceatposwidget.h"
#include "snormalforceatrelposwidget.h"
#include "srelativetorquewidget.h"
#include "srelativeforcewidget.h"
#include "srelativeforceatposwidget.h"
#include "srelativeforceatrelposwidget.h"

#include <qlabel.h>

SGuiPropertiesView::SGuiPropertiesView(QWidget* parent, const char* name, WFlags fl)
        : UiPropertiesViewWidget(parent,name,fl)
{
    mProperties = 0;
    mTreeViewSelectedItem = 0;
    
    QWidget * a = new QWidget(this);
    mContent->setContent(a);
    a->show();
}

SGuiPropertiesView::~SGuiPropertiesView()
{}

void SGuiPropertiesView::treeViewSelectionChanged(QListViewItem * item)
{
    mTreeViewSelectedItem = dynamic_cast<SGuiTreeViewItem *>(item);
    if(mTreeViewSelectedItem != 0L)
    {
        switch(mTreeViewSelectedItem->getItemType())
        {
        case SGuiTreeViewItem::SOBJECT:
            {
                SObject * obj = 0;
                propertiesShow(mTreeViewSelectedItem->getItem(obj));
            }
            break;
        case SGuiTreeViewItem::ENVIRONMENT:
            {
                SEnvironment * obj = 0;
                propertiesShow(mTreeViewSelectedItem->getItem(obj));
            }
            break;
        case SGuiTreeViewItem::FORCE:
            {
                SForce * obj = 0;
                propertiesShow(mTreeViewSelectedItem->getItem(obj));
            }
            break;
        case SGuiTreeViewItem::JOINT:
            {
                SJoint * obj = 0;
                propertiesShow(mTreeViewSelectedItem->getItem(obj));
            }
            break;
        case SGuiTreeViewItem::VELOCITY:
            {
                SObject * obj = 0;
                obj = mTreeViewSelectedItem->getItem(obj);
                
            	SVelocityWidget * wid = new SVelocityWidget(this);
	        wid->setObject(obj);

         	mContentsTextLabel->setText("Velocity Properties");
	        mProperties = wid;
           
	        // Set the widget in the scrollview
                mContent->setContent(wid);
                wid->show();
            }
            break;
        default: break;
        }
    }
}

void SGuiPropertiesView::propertiesShow(SObject * obj)
{
    QWidget * widget = 0;

    switch(obj->getObjectType())
    {
    case SObject::BOX:
        {
            widget = new SBoxWidget(this);
            ((SBoxWidget*)widget)->setObject(obj);

            mContentsTextLabel->setText("Box Properties");
            mProperties = (SBoxWidget*)widget;
        }
        break;
    case SObject::SPHERE:
        {
            widget = new SSphereWidget(this);
            ((SSphereWidget *)widget)->setObject(obj);

            mContentsTextLabel->setText("Sphere Properties");
            mProperties = (SSphereWidget *)widget;
        }
        break;
    case SObject::CAPPEDCYLINDER:
        {
            widget = new SCappedCylinderWidget(this);
            ((SCappedCylinderWidget *)widget)->setObject(obj);

            mContentsTextLabel->setText("Capped Cylinder Properties");
            mProperties = (SCappedCylinderWidget *)widget;
        }
        break;
    case SObject::POOL:
        {
            widget = new SPoolWidget(this);
            ((SPoolWidget *)widget)->setObject(obj);

            mContentsTextLabel->setText("Pool Properties");
            mProperties = (SPoolWidget *)widget;
        }
        break;
    case SObject::GEAR:
        {
            widget = new SGearWidget(this);
            ((SGearWidget *)widget)->setObject(obj);

            mContentsTextLabel->setText("Gear Properties");
            mProperties = (SGearWidget *)widget;
        }
        break;
    case SObject::INCLINEDPLANE:
        {
            widget = new SInclinedPlaneWidget(this);
            ((SInclinedPlaneWidget *)widget)->setObject(obj);

            mContentsTextLabel->setText("Inclined Plane Properties");
            mProperties = (SInclinedPlaneWidget *)widget;
        }
        break;
    case SObject::TRIMESH:
        {
            widget = new STrimeshWidget(this);
            ((STrimeshWidget *)widget)->setObject(obj);

            mContentsTextLabel->setText("Trimesh Properties");
            mProperties = (STrimeshWidget *)widget;
        }
        break;
    case SObject::LIGHT:
        {
            widget = new SLightSourceWidget(this);
            ((SLightSourceWidget *)widget)->setObject(obj);

            mContentsTextLabel->setText("Light Source Properties");
            mProperties = (SLightSourceWidget *)widget;
        }
        break;
    case SObject::FLATMIRROR:
        {
            widget = new SFlatMirrorWidget(this);
            ((SFlatMirrorWidget *)widget)->setObject(obj);

            mContentsTextLabel->setText("Flat Mirror Properties");
            mProperties = (SFlatMirrorWidget *)widget;
        }
        break;
    case SObject::CONMIRROR:
        {
            widget = new SConMirrorWidget(this);
            ((SConMirrorWidget *)widget)->setObject(obj);

            mContentsTextLabel->setText("Con Mirror Properties");
            mProperties = (SConMirrorWidget *)widget;
        }
        break;
    case SObject::CONVEXLENS:
        {
            widget = new SConvexLensWidget(this);
            ((SConvexLensWidget *)widget)->setObject(obj);

            mContentsTextLabel->setText("Convex Lens Properties");
            mProperties = (SConvexLensWidget *)widget;
        }
        break;
    default:
        std::cout << "PropertiesView: Cannot identify S0bject type" << std::endl;
        return;
        break;
    }
    
     // Set the widget in the scrollview
     mContent->setContent(widget);
     widget->show();
}

void SGuiPropertiesView::propertiesShow(SJoint * joint)
{
   QWidget * widget = 0;

    switch(joint->getJointType())
    {
    case SJoint::BALLSOCKET:
        {
            std::cout << "sguipropertiesview: Ball socket properties showing" << std::endl;
            widget = new SBallSocketJointWidget(this);
            ((SBallSocketJointWidget*)widget)->setObject(joint);

            mContentsTextLabel->setText("Joint Properties");
            mProperties = (SBallSocketJointWidget*)widget;
        }
        break;
    case SJoint::HINGE:
        {
            std::cout << "sguipropertiesview: hinge properties showing" << std::endl;
            widget = new SHingeJointWidget(this);
            ((SHingeJointWidget*)widget)->setObject(joint);

            mContentsTextLabel->setText("Joint Properties");
            mProperties = (SHingeJointWidget*)widget;
        }
        break;
    case SJoint::HINGE2:
        {
            widget = new SHinge2JointWidget(this);
            ((SHinge2JointWidget*)widget)->setObject(joint);

            mContentsTextLabel->setText("Joint Properties");
            mProperties = (SHinge2JointWidget*)widget;
        }
        break;
   case SJoint::SLIDER:
        {
            std::cout << "sguipropertiesview: slider properties showing" << std::endl;
            widget = new SSliderJointWidget(this);
            ((SSliderJointWidget*)widget)->setObject(joint);

            mContentsTextLabel->setText("Joint Properties");
            mProperties = (SSliderJointWidget*)widget;
        }
        break;
    case SJoint::UNIVERSAL:
        {
            widget = new SUniversalJointWidget(this);
            ((SUniversalJointWidget*)widget)->setObject(joint);

            mContentsTextLabel->setText("Joint Properties");
            mProperties = (SUniversalJointWidget*)widget;
        }
        break;
    default:
        std::cout << "PropertiesView: Cannot identify SJoint type" << std::endl;
        return;
        break;
    }
    
     // Set the widget in the scrollview
     mContent->setContent(widget);
     widget->show();    
}

void SGuiPropertiesView::propertiesShow(SForce * force)
{
    QWidget * widget = NULL;

    switch(force->getType())
    {
    case SForce::NORMALFORCE:
        {
            widget = new SNormalForceWidget(this);
            ((SNormalForceWidget *)widget)->setForce(force);

             mProperties = ((SNormalForceWidget *)widget);
        }
        break;
    case SForce::FORCEATPOS:
        {
            widget = new SNormalForceAtPosWidget(this);
            ((SNormalForceAtPosWidget *)widget)->setForce(force);

             mProperties = ((SNormalForceAtPosWidget *)widget);
        }
        break;
    case SForce::FORCEATRELPOS:
        {
            widget = new SNormalForceAtRelPosWidget(this);
            ((SNormalForceAtRelPosWidget *)widget)->setForce(force);

             mProperties = ((SNormalForceAtRelPosWidget *)widget);
        }
        break;
    case SForce::RELFORCE:
        {
            widget = new SRelativeForceWidget(this);
            ((SRelativeForceWidget *)widget)->setForce(force);

             mProperties = ((SRelativeForceWidget *)widget);
        }
        break;
    case SForce::RELFORCEATPOS:
        {
            widget = new SRelativeForceAtPosWidget(this);
            ((SRelativeForceAtPosWidget *)widget)->setForce(force);

             mProperties = ((SRelativeForceAtPosWidget *)widget);
        }
        break;
    case SForce::RELFORCEATRELPOS:
        {
            widget = new SRelativeForceAtRelPosWidget(this);
            ((SRelativeForceAtRelPosWidget *)widget)->setForce(force);

             mProperties = ((SRelativeForceAtRelPosWidget *)widget);
        }
        break;
    case SForce::TORQUE:
        {
            widget = new SNormalTorqueWidget(this);
            ((SNormalTorqueWidget *)widget)->setForce(force);

             mProperties = ((SNormalTorqueWidget *)widget);
        }
        break;
    case SForce::RELTORQUE:
        {
            widget = new SRelativeTorqueWidget(this);
            ((SRelativeTorqueWidget *)widget)->setForce(force);

             mProperties = ((SRelativeTorqueWidget *)widget);
        }
        break;
    default:
        std::cout << "PropertiesView: Cannot identify SForce type" << std::endl;
        return;
        break;
    }

    mContentsTextLabel->setText("Force Properties");
    // Set the widget in the scrollview
    mContent->setContent(widget);
    widget->show();
}

void SGuiPropertiesView::propertiesShow(SEnvironment * env)
{
    SEnvWidget * envWidget = new SEnvWidget(this);
    envWidget->setObject(env);

    // Set the widget in the scrollview
    mContent->setContent(envWidget);
    envWidget->show();

    mContentsTextLabel->setText("Environment Properties");
    mProperties = envWidget;
}

void SGuiPropertiesView::updateProperties()
{
    if(mProperties != NULL)
        mProperties->reset();
}

void SGuiPropertiesView::applyButtonClicked()
{
    emit applyClicked();
}

void SGuiPropertiesView::cancelButtonClicked()
{
    emit cancelClicked();
}

