#include "sboxwidget.h"

#include <qcombobox.h>
#include <qlineedit.h>
#include <qstring.h>
#include <qlabel.h>

SBoxWidget::SBoxWidget(QWidget* parent, const char* name, WFlags fl)
        : UiBoxWidget(parent, name, fl)
{
    mObject = 0;
    mNeedsRedraw = true;
}

void SBoxWidget::setObject(SObject * obj)
{
    mObject = dynamic_cast<SBox *> (obj);

    if(mObject == 0)
    {
        std::cout << "FATAL ERROR: cannot convert SObject* to SBox*" << std::endl;
        return;
    }

    // set properties...
    reset();
}

void SBoxWidget::reset()
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

    osg::Vec3 vec = mObject->getDimensions();

    mXDimLineEdit->setText(str.setNum(vec.x(), 'f', 3));
    mYDimLineEdit->setText(str.setNum(vec.y(), 'f', 3));
    mZDimLineEdit->setText(str.setNum(vec.z(), 'f', 3));

    vec = mObject->getPosition();

    mXPosLineEdit->setText(str.setNum(vec.x(), 'f', 3));
    mYPosLineEdit->setText(str.setNum(vec.y(), 'f', 3));
    mZPosLineEdit->setText(str.setNum(vec.z(), 'f', 3));
}

void SBoxWidget::save()
{
    if(mObject.valid())
    {
        osg::Vec3 vec;

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

        vec[0] = mXDimLineEdit->text().toFloat();
        vec[1] = mYDimLineEdit->text().toFloat();
        vec[2] = mZDimLineEdit->text().toFloat();

        mObject->setDimensions(vec);

        vec[0] = mXPosLineEdit->text().toFloat();
        vec[1] = mYPosLineEdit->text().toFloat();
        vec[2] = mZPosLineEdit->text().toFloat();

        mObject->setPosition(vec);
        
        mObject->createEditorObject();
        
        list<SJoint*>::iterator j;
        for(j = mObject->getJointBegin() ; j != mObject->getJointEnd() ; j++)
        {
            (*j)->createEditorJoint();
        }
        
        reset();
    }
}



