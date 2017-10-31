#include "sflatmirrorwidget.h"

#include <qcombobox.h>
#include <qlineedit.h>
#include <qstring.h>
#include <qlabel.h>

SFlatMirrorWidget::SFlatMirrorWidget(QWidget* parent, const char* name, WFlags fl)
        : UiFlatMirrorWidget(parent, name, fl)
{
    mObject = 0;
    mNeedsRedraw = true;
}

void SFlatMirrorWidget::setObject(SObject * obj)
{
    mObject = dynamic_cast<SFlatMirror *> (obj);

    if(mObject == 0)
    {
        std::cout << "FATAL ERROR: cannot convert SObject* to SFlatMirror*" << std::endl;
        return;
    }

    // set properties...
    reset();
}

void SFlatMirrorWidget::reset()
{
    QString str;

    mNameLineEdit->setText(mObject->getName());

    mWidthLineEdit->setText(str.setNum(mObject->getWidth(), 'f', 3));
    mLengthLineEdit->setText(str.setNum(mObject->getLength(), 'f', 3));

    osg::Vec3 vec = mObject->getPosition();

    mXPosLineEdit->setText(str.setNum(vec.x(), 'f', 3));
    mYPosLineEdit->setText(str.setNum(vec.y(), 'f', 3));
    mZPosLineEdit->setText(str.setNum(vec.z(), 'f', 3));
}

void SFlatMirrorWidget::save()
{
    if(mObject.valid())
    {
        osg::Vec3 vec;

        mObject->setName(mNameLineEdit->text());

        mObject->setWidth(mWidthLineEdit->text().toFloat());
        mObject->setLength(mLengthLineEdit->text().toFloat());

        vec[0] = mXPosLineEdit->text().toFloat();
        vec[1] = mYPosLineEdit->text().toFloat();
        vec[2] = mZPosLineEdit->text().toFloat();

        mObject->setPosition(vec);
        
        mObject->createEditorObject();
        reset();
    }
}



