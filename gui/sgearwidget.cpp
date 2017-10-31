#include "sgearwidget.h"

SGearWidget::SGearWidget(QWidget* parent, const char* name, WFlags fl)
        : UiGearWidget(parent, name, fl)
{
    mObject = 0;
    mNeedsRedraw = true;
}

void SGearWidget::setObject(SObject * obj)
{
    mObject = dynamic_cast<SGear *> (obj);

    if(mObject == 0)
    {
        std::cout << "FATAL ERROR: cannot convert SObject* to SGear*" << std::endl;
        return;
    }

    // set properties...
    reset();
}

void SGearWidget::reset()
{
    QString str;

    mNameLineEdit->setText(mObject->getName());
    
    if(mObject->getGearType() == SGear::BOXGEAR)
    {
        mTypeComboBox->setCurrentItem(0);
    }
    else {
        mTypeComboBox->setCurrentItem(1);
    }
    
    mRadiusLineEdit->setText(str.setNum(mObject->getRadius(), 'f', 3));
    mToothCountSpinBox->setValue(mObject->getToothNumber());

    osg::Vec3 vec = mObject->getPosition();

    mXPosLineEdit->setText(str.setNum(vec.x(), 'f', 3));
    mYPosLineEdit->setText(str.setNum(vec.y(), 'f', 3));
    mZPosLineEdit->setText(str.setNum(vec.z(), 'f', 3));
    
    vec = mObject->getAngVelocity();

    mVelocityLineEdit->setText(str.setNum(vec.z(), 'f', 3));
}

void SGearWidget::save()
{
    if(mObject.valid())
    {
        osg::Vec3 vec;

        mObject->setName(mNameLineEdit->text());
        
        if(mTypeComboBox->currentItem() == 0)
        {
            mObject->setGearType(SGear::BOXGEAR);
        }
        else
        {
            mObject->setGearType(SGear::PRISMGEAR);
        }
                
        mObject->setRadius(mRadiusLineEdit->text().toFloat());
        mObject->setToothNumber(mToothCountSpinBox->value());
        
        vec[0] = mXPosLineEdit->text().toFloat();
        vec[1] = mYPosLineEdit->text().toFloat();
        vec[2] = mZPosLineEdit->text().toFloat();
        
        mObject->setPosition(vec);

        vec[0] = 0;
        vec[1] = 0;
        vec[2] = mVelocityLineEdit->text().toFloat();
        mObject->setAngVelocity(vec);
        
        mObject->createEditorObject();
        reset();
    }
}



