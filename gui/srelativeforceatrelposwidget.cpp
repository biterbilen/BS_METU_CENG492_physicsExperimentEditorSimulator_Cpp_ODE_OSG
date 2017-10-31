#include "srelativeforceatrelposwidget.h"

SRelativeForceAtRelPosWidget::SRelativeForceAtRelPosWidget(QWidget* parent, const char* name, WFlags fl)
        : UiRelativeForceAtRelPosWidget(parent, name, fl)
{
    mForce = 0;
    mNeedsRedraw = true;
}

void SRelativeForceAtRelPosWidget::setForce(SForce * obj)
{
    mForce = dynamic_cast<SRelForceAtRelPos*> (obj);

    if(mForce == 0)
    {
        std::cout << "FATAL ERROR: cannot convert SForce* to SRelForceAtRelPos*" << std::endl;
        return;
    }

    // set properties...
    reset();
}

void SRelativeForceAtRelPosWidget::reset()
{
    QString str;

    mNameLineEdit->setText(mForce->getName());

    osg::Vec3 vec = mForce->getForce();

    mForceVectorXLineEdit->setText(str.setNum(vec.x(), 'f'));
    mForceVectorYLineEdit->setText(str.setNum(vec.y(), 'f'));
    mForceVectorZLineEdit->setText(str.setNum(vec.z(), 'f'));
    
    vec = mForce->getPosition();

    mForceAppPointXLineEdit->setText(str.setNum(vec.x(), 'f'));
    mForceAppPointYLineEdit->setText(str.setNum(vec.y(), 'f'));
    mForceAppPointZLineEdit->setText(str.setNum(vec.z(), 'f'));
    
    mStartTimeLineEdit->setText(str.setNum(mForce->getStart(), 'f'));
    mDurationLineEdit->setText(str.setNum(mForce->getDuration(), 'f'));
}

void SRelativeForceAtRelPosWidget::save()
{
    if(mForce.valid())
    {
        osg::Vec3 vec;

        vec[0] = mForceVectorXLineEdit->text().toFloat();
        vec[1] = mForceVectorYLineEdit->text().toFloat();
        vec[2] = mForceVectorZLineEdit->text().toFloat();
        
        mForce->setForce(vec);
        
        vec[0] = mForceAppPointXLineEdit->text().toFloat();
        vec[1] = mForceAppPointYLineEdit->text().toFloat();
        vec[2] = mForceAppPointZLineEdit->text().toFloat();
        
        mForce->setPosition(vec);
        
        float start = mStartTimeLineEdit->text().toFloat();
        float duration = mDurationLineEdit->text().toFloat();
        
        mForce->setStart(start);
        mForce->setDuration(duration);
        
        mForce->createEditorForce();
        
        reset();
    }
}

