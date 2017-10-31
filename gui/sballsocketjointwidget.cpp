#include "sballsocketjointwidget.h"

#include <qlineedit.h>
#include <qstring.h>
#include <qlabel.h>

#include "sobject.h"

SBallSocketJointWidget::SBallSocketJointWidget( QWidget* parent, const char* name, WFlags fl )
        : UiBallSocketJointWidget( parent, name, fl )
{
    mJoint = 0;
    mNeedsRedraw = true;
}

void SBallSocketJointWidget::setObject( SJoint * obj )
{
    mJoint = dynamic_cast<SBallSocket *> ( obj );

    if ( mJoint == 0 )
    {
        std::cout << "FATAL ERROR: cannot convert SJoint* to SBallSocket*" << std::endl;
        return ;
    }

    // set properties...
    reset();
}

void SBallSocketJointWidget::reset()
{
    QString str;

    mNameLineEdit->setText( mJoint->getName() );
    mObj1NameTextLabel->setText( mJoint->getBody1() ->getName() );
    mObj2NameTextLabel->setText( mJoint->getBody2() ->getName() );

    osg::Vec3 vec = mJoint->getAnchor();

    mAnchorXLineEdit->setText( str.setNum( vec.x(), 'f', 3 ) );
    mAnchorYLineEdit->setText( str.setNum( vec.y(), 'f', 3 ) );
    mAnchorZLineEdit->setText( str.setNum( vec.z(), 'f', 3 ) );
}

void SBallSocketJointWidget::save()
{
    if ( mJoint.valid() )
    {
        osg::Vec3 vec;

        mJoint->setName( mNameLineEdit->text() );

        vec[ 0 ] = mAnchorXLineEdit->text().toFloat();
        vec[ 1 ] = mAnchorYLineEdit->text().toFloat();
        vec[ 2 ] = mAnchorZLineEdit->text().toFloat();

        mJoint->setAnchor( vec );
        
        mJoint->createEditorJoint();

        reset();
    }
}


