#include "strimeshwidget.h"

#include <qcombobox.h>
#include <qlineedit.h>
#include <qstring.h>
#include <qlabel.h>

STrimeshWidget::STrimeshWidget(QWidget* parent, const char* name, WFlags fl)
        : UiTrimeshWidget(parent, name, fl)
{
    mObject = 0;
    mNeedsRedraw = true;
}

void STrimeshWidget::setObject(SObject * obj)
{
    mObject = dynamic_cast<STrimesh *> (obj);

    if(mObject == 0)
    {
        std::cout << "FATAL ERROR: cannot convert SObject* to STrimesh*" << std::endl;
        return;
    }

    // set properties...
    reset();
}

void STrimeshWidget::reset()
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

    mMassLineEdit->setText(str.setNum(mObject->getMass(), 'f', 3));

    osg::Vec3 vec = mObject->getPosition();

    mXPosLineEdit->setText(str.setNum(vec.x(), 'f', 3));
    mYPosLineEdit->setText(str.setNum(vec.y(), 'f', 3));
    mZPosLineEdit->setText(str.setNum(vec.z(), 'f', 3));
}

void STrimeshWidget::save()
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

        mObject->setMass(mMassLineEdit->text().toFloat());
        
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



