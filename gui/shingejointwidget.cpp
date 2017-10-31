#include "shingejointwidget.h"

#include <qlineedit.h>
#include <qstring.h>
#include <qlabel.h>

#include "sobject.h"

SHingeJointWidget::SHingeJointWidget( QWidget* parent, const char* name, WFlags fl )
        : UiHingeJointWidget( parent, name, fl )
{
    mJoint = 0;
    mNeedsRedraw = true;
}

void SHingeJointWidget::setObject( SJoint * obj )
{
    mJoint = dynamic_cast<SHinge *> ( obj );

    if ( mJoint == 0 )
    {
        std::cout << "FATAL ERROR: cannot convert SJoint* to SHinge*" << std::endl;
        return ;
    }

    // set properties...
    reset();
}

void SHingeJointWidget::reset()
{
    QString str;

    mNameLineEdit->setText( mJoint->getName() );
    mObj1NameTextLabel->setText( mJoint->getBody1() ->getName() );
    mObj2NameTextLabel->setText( mJoint->getBody2() ->getName() );

    osg::Vec3 vec = mJoint->getAnchor();

    mAnchorXLineEdit->setText( str.setNum( vec.x(), 'f', 3 ) );
    mAnchorYLineEdit->setText( str.setNum( vec.y(), 'f', 3 ) );
    mAnchorZLineEdit->setText( str.setNum( vec.z(), 'f', 3 ) );
    
    vec = mJoint->getAxis();

    mAxisXLineEdit->setText( str.setNum( vec.x(), 'f', 3 ) );
    mAxisYLineEdit->setText( str.setNum( vec.y(), 'f', 3 ) );
    mAxisZLineEdit->setText( str.setNum( vec.z(), 'f', 3 ) );

    if ( mJoint->isLoStopSet() )
    {
        mAxis1LowStopLineEdit->setText( str.setNum( mJoint->getLoStop(), 'f', 3 ) );
    }
    if ( mJoint->isHiStopSet() )
    {
        mAxis1HighStopLineEdit->setText( str.setNum( mJoint->getHiStop(), 'f', 3 ) );
    }
    if ( mJoint->isVelSet() )
    {
        mAxis1VelocityLineEdit->setText( str.setNum( mJoint->getVel(), 'f', 3 ) );
    }
    if ( mJoint->isFMaxSet() )
    {
        mAxis1FMaxLineEdit->setText( str.setNum( mJoint->getFMax(), 'f', 3 ) );
    }
}

void SHingeJointWidget::save()
{
    if ( mJoint.valid() )
    {
        osg::Vec3 vec;

        mJoint->setName( mNameLineEdit->text() );

        vec[ 0 ] = mAnchorXLineEdit->text().toFloat();
        vec[ 1 ] = mAnchorYLineEdit->text().toFloat();
        vec[ 2 ] = mAnchorZLineEdit->text().toFloat();

        mJoint->setAnchor( vec );
        
        vec[ 0 ] = mAxisXLineEdit->text().toFloat();
        vec[ 1 ] = mAxisYLineEdit->text().toFloat();
        vec[ 2 ] = mAxisZLineEdit->text().toFloat();

        mJoint->setAxis( vec );

        if ( mAxis1LowStopLineEdit->text() )
        {
            mJoint->setLoStop(mAxis1LowStopLineEdit->text().toFloat());
        }
        if ( mAxis1HighStopLineEdit->text() )
        {
            mJoint->setHiStop(mAxis1HighStopLineEdit->text().toFloat());
        }
        if ( mAxis1VelocityLineEdit->text() )
        {
            mJoint->setVel(mAxis1VelocityLineEdit->text().toFloat());
        }
        if ( mAxis1FMaxLineEdit->text() )
        {
            mJoint->setFMax(mAxis1FMaxLineEdit->text().toFloat());
        }
        
        mJoint->createEditorJoint();
        reset();
    }
}


