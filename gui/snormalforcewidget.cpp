#include "snormalforcewidget.h"

SNormalForceWidget::SNormalForceWidget(QWidget* parent, const char* name, WFlags fl)
        : UiNormalForceWidget(parent, name, fl)
{
    mForce = 0;
    mNeedsRedraw = true;
}

void SNormalForceWidget::setForce(SForce * obj)
{
    mForce = dynamic_cast<SNormalForce*> (obj);

    if(mForce == 0)
    {
        std::cout << "FATAL ERROR: cannot convert SForce* to SNormalForce*" << std::endl;
        return;
    }

    // set properties...
    reset();
}

void SNormalForceWidget::reset()
{
    QString str;

    mNameLineEdit->setText(mForce->getName());

    osg::Vec3 vec = mForce->getForce();

    mForceVectorXLineEdit->setText(str.setNum(vec.x(), 'f'));
    mForceVectorYLineEdit->setText(str.setNum(vec.y(), 'f'));
    mForceVectorZLineEdit->setText(str.setNum(vec.z(), 'f'));
    
    mStartTimeLineEdit->setText(str.setNum(mForce->getStart(), 'f'));
    mDurationLineEdit->setText(str.setNum(mForce->getDuration(), 'f'));
}

void SNormalForceWidget::save()
{
    if(mForce.valid())
    {
        osg::Vec3 vec;

        vec[0] = mForceVectorXLineEdit->text().toFloat();
        vec[1] = mForceVectorYLineEdit->text().toFloat();
        vec[2] = mForceVectorZLineEdit->text().toFloat();
        
        mForce->setForce(vec);
        
        float start = mStartTimeLineEdit->text().toFloat();
        float duration = mDurationLineEdit->text().toFloat();
        
        mForce->setStart(start);
        mForce->setDuration(duration);
        
        mForce->createEditorForce();
        
        reset();
    }
}

