#include "sspherewidget.h"

#include <qcombobox.h>
#include <qlineedit.h>
#include <qstring.h>
#include <qlabel.h>

SSphereWidget::SSphereWidget(QWidget* parent, const char* name, WFlags fl)
            : UiSphereWidget(parent, name, fl), SGuiProperties()
{
    mObject = 0;
    mNeedsRedraw = true;
}

void SSphereWidget::setObject(SObject * obj)
{
    mObject = dynamic_cast<SSphere *> (obj);
    
    if(mObject == 0)
    {
        std::cout << "FATAL ERROR: cannot convert SObject* to SSphere*" << std::endl;
        return;
    }

    // set properties...
    reset();
}

void SSphereWidget::reset()
{
    QString str;
    
    mNameLineEdit->setText(mObject->getName());
        if(mObject->getType() == SObject::DYNAMIC)
    {
        mDynamicComboBox->setCurrentItem(0);
    }
    else
    {
        mDynamicComboBox->setCurrentItem(1);
    }

    if(mObject->hasBobbies())
    {
        mBuoyancyComboBox->setCurrentItem(0);
    }
    else
    {
        mBuoyancyComboBox->setCurrentItem(1);
    }
    
    mMassLineEdit->setText(str.setNum(mObject->getMass(), 'f', 3));
    mVolumeValue->setText(str.setNum(mObject->getVolume(), 'f', 3));
    mDensityValue->setText(str.setNum(mObject->getDensity(), 'f', 3));
    
    mRadiusLineEdit->setText(str.setNum(mObject->getRadius(), 'f', 3));

    osg::Vec3 vec = mObject->getPosition();

    mXPosLineEdit->setText(str.setNum(vec.x(), 'f', 3));
    mYPosLineEdit->setText(str.setNum(vec.y(), 'f', 3));
    mZPosLineEdit->setText(str.setNum(vec.z(), 'f', 3));

//     mXOriLineEdit->setText();
//     mYOriLineEdit->setText();
//     mZOriLineEdit->setText();
}

void SSphereWidget::save()
{
    if(mObject.valid())
    {
        osg::Vec3 vec;
        float radius;
        
        mObject->setName(mNameLineEdit->text());
        
        if(mDynamicComboBox->currentItem() == 0)
        {
            mObject->setDynamic();
        }
        else
        {
            mObject->setStatic();
        }

        if(mBuoyancyComboBox->currentItem() == 0)
        {
            mObject->setBobby(true);
        }
        else
        {
            mObject->setBobby(false);
        }

        mObject->setMass(mMassLineEdit->text().toFloat());
        
        radius = mRadiusLineEdit->text().toFloat();
        mObject->setRadius(radius);

        vec[0] = mXPosLineEdit->text().toFloat();
        vec[1] = mYPosLineEdit->text().toFloat();
        vec[2] = mZPosLineEdit->text().toFloat();
        mObject->setPosition(vec);

        //TODO save orientation
                  
        mObject->createEditorObject();
        
        list<SJoint*>::iterator j;
        for(j = mObject->getJointBegin() ; j != mObject->getJointEnd() ; j++)
        {
            (*j)->createEditorJoint();
        }
    }
}


