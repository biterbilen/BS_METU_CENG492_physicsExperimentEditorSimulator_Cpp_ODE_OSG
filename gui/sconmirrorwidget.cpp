#include "sconmirrorwidget.h"

#include <qcombobox.h>
#include <qlineedit.h>
#include <qstring.h>
#include <qlabel.h>

#define RAD2DEG(x) ( (x)*180/3.14159265356 )

SConMirrorWidget::SConMirrorWidget(QWidget* parent, const char* name, WFlags fl)
        : UiConMirrorWidget(parent, name, fl)
{
    mObject = 0;
    mNeedsRedraw = true;
}

void SConMirrorWidget::setObject(SObject * obj)
{
    mObject = dynamic_cast<SConMirror *> (obj);

    if(mObject == 0)
    {
        std::cout << "FATAL ERROR: cannot convert SObject* to SConMirror*" << std::endl;
        return;
    }

    // set properties...
    reset();
}

void SConMirrorWidget::reset()
{
    QString str;

    mNameLineEdit->setText(mObject->getName());
    
    mFocalLengthLineEdit->setText(str.setNum(mObject->getFocalLength(), 'f', 3));
    mAzRangeLineEdit->setText(str.setNum(RAD2DEG(mObject->getAzRange()), 'f', 3));
    mElevRangeLineEdit->setText(str.setNum(RAD2DEG(mObject->getElevRange()), 'f', 3));

    osg::Vec3 vec = mObject->getPosition();

    mXPosLineEdit->setText(str.setNum(vec.x(), 'f', 3));
    mYPosLineEdit->setText(str.setNum(vec.y(), 'f', 3));
    mZPosLineEdit->setText(str.setNum(vec.z(), 'f', 3));
}

void SConMirrorWidget::save()
{
    if(mObject.valid())
    {
        osg::Vec3 vec;

        mObject->setName(mNameLineEdit->text());

        mObject->setFocalLength(mFocalLengthLineEdit->text().toFloat());
        mObject->setAzRange(osg::DegreesToRadians(mAzRangeLineEdit->text().toFloat()));
        mObject->setElevRange(osg::DegreesToRadians(mElevRangeLineEdit->text().toFloat()));

        vec[0] = mXPosLineEdit->text().toFloat();
        vec[1] = mYPosLineEdit->text().toFloat();
        vec[2] = mZPosLineEdit->text().toFloat();

        mObject->setPosition(vec);
        
        mObject->createEditorObject();
        reset();
    }
}



