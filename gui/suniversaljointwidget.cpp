#include "suniversaljointwidget.h"

#include <qlineedit.h>
#include <qstring.h>
#include <qlabel.h>

#include "sobject.h"

SUniversalJointWidget::SUniversalJointWidget( QWidget* parent, const char* name, WFlags fl )
        : UiUniversalJointWidget( parent, name, fl )
{
    mJoint = 0;
    mNeedsRedraw = true;
}

void SUniversalJointWidget::setObject( SJoint * obj )
{
    mJoint = dynamic_cast<SUniversal *> ( obj );

    if ( mJoint == 0 )
    {
        std::cout << "FATAL ERROR: cannot convert SJoint* to SUniversal*" << std::endl;
        return ;
    }

    // set properties...
    reset();
}

void SUniversalJointWidget::reset()
{
    QString str;

    mNameLineEdit->setText( mJoint->getName() );
    mObj1NameTextLabel->setText( mJoint->getBody1() ->getName() );
    mObj2NameTextLabel->setText( mJoint->getBody2() ->getName() );

    osg::Vec3 vec = mJoint->getAnchor();

    mAnchorXLineEdit->setText( str.setNum( vec.x(), 'f', 3 ) );
    mAnchorYLineEdit->setText( str.setNum( vec.y(), 'f', 3 ) );
    mAnchorZLineEdit->setText( str.setNum( vec.z(), 'f', 3 ) );
    
    vec = mJoint->getAxis1();

    mAxis1XLineEdit->setText( str.setNum( vec.x(), 'f', 3 ) );
    mAxis1YLineEdit->setText( str.setNum( vec.y(), 'f', 3 ) );
    mAxis1ZLineEdit->setText( str.setNum( vec.z(), 'f', 3 ) );
    
    vec = mJoint->getAxis2();

    mAxis2XLineEdit->setText( str.setNum( vec.x(), 'f', 3 ) );
    mAxis2YLineEdit->setText( str.setNum( vec.y(), 'f', 3 ) );
    mAxis2ZLineEdit->setText( str.setNum( vec.z(), 'f', 3 ) );

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
    
     if ( mJoint->isLoStopSetAxis2() )
    {
        mAxis2LowStopLineEdit->setText( str.setNum( mJoint->getLoStopAxis2(), 'f', 3 ) );
    }
    if ( mJoint->isHiStopSetAxis2() )
    {
        mAxis2HighStopLineEdit->setText( str.setNum( mJoint->getHiStopAxis2(), 'f', 3 ) );
    }
    if ( mJoint->isVelSetAxis2() )
    {
        mAxis2VelocityLineEdit->setText( str.setNum( mJoint->getVelAxis2(), 'f', 3 ) );
    }
    if ( mJoint->isFMaxSetAxis2() )
    {
        mAxis2FMaxLineEdit->setText( str.setNum( mJoint->getFMaxAxis2(), 'f', 3 ) );
    }
}

void SUniversalJointWidget::save()
{
    if ( mJoint.valid() )
    {
        osg::Vec3 vec;

        mJoint->setName( mNameLineEdit->text() );

        vec[ 0 ] = mAnchorXLineEdit->text().toFloat();
        vec[ 1 ] = mAnchorYLineEdit->text().toFloat();
        vec[ 2 ] = mAnchorZLineEdit->text().toFloat();

        mJoint->setAnchor( vec );
        
        vec[ 0 ] = mAxis1XLineEdit->text().toFloat();
        vec[ 1 ] = mAxis1YLineEdit->text().toFloat();
        vec[ 2 ] = mAxis1ZLineEdit->text().toFloat();

        mJoint->setAxis1( vec );
        
        vec[ 0 ] = mAxis2XLineEdit->text().toFloat();
        vec[ 1 ] = mAxis2YLineEdit->text().toFloat();
        vec[ 2 ] = mAxis2ZLineEdit->text().toFloat();

        mJoint->setAxis2( vec );

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

         if ( mAxis2LowStopLineEdit->text() )
        {
            mJoint->setLoStopAxis2(mAxis2LowStopLineEdit->text().toFloat());
        }
        if ( mAxis2HighStopLineEdit->text() )
        {
            mJoint->setHiStopAxis2(mAxis2HighStopLineEdit->text().toFloat());
        }
        if ( mAxis2VelocityLineEdit->text() )
        {
            mJoint->setVelAxis2(mAxis2VelocityLineEdit->text().toFloat());
        }
        if ( mAxis2FMaxLineEdit->text() )
        {
            mJoint->setFMaxAxis2(mAxis2FMaxLineEdit->text().toFloat());
        }
        
        mJoint->createEditorJoint();
        reset();
    }
}


