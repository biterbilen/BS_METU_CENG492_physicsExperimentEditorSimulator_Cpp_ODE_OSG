#include "svelocitywidget.h"

#include <qlineedit.h>
#include <qstring.h>

SVelocityWidget::SVelocityWidget(QWidget* parent, const char* name, WFlags fl)
        : UiVelocityWidget(parent, name, fl)
{
    mObject = 0;
    mNeedsRedraw = false;
}

void SVelocityWidget::setObject(SObject * obj)
{
    mObject = obj;

    // set properties...
    reset();
}

void SVelocityWidget::reset()
{
    QString str;

    osg::Vec3 vec = mObject->getLinVelocity();

    mLinVelocityXLineEdit->setText(str.setNum(vec.x(), 'f'));
    mLinVelocityYLineEdit->setText(str.setNum(vec.y(), 'f'));
    mLinVelocityZLineEdit->setText(str.setNum(vec.z(), 'f'));
    
    vec = mObject->getAngVelocity();
    
    mAngVelocityXLineEdit->setText(str.setNum(vec.x(), 'f'));
    mAngVelocityYLineEdit->setText(str.setNum(vec.y(), 'f'));
    mAngVelocityZLineEdit->setText(str.setNum(vec.z(), 'f'));
}

void SVelocityWidget::save()
{
    if(mObject.valid())
    {
        osg::Vec3 vec;
        
        vec[0] = mLinVelocityXLineEdit->text().toFloat();
        vec[1] = mLinVelocityYLineEdit->text().toFloat();
        vec[2] = mLinVelocityZLineEdit->text().toFloat();

        mObject->setLinVelocity(vec);
        
        vec[0] = mAngVelocityXLineEdit->text().toFloat();
        vec[1] = mAngVelocityYLineEdit->text().toFloat();
        vec[2] = mAngVelocityZLineEdit->text().toFloat();

        mObject->setAngVelocity(vec);
    }
}



