#include "sinclinedplanewidget.h"

#include <qcombobox.h>
#include <qlineedit.h>
#include <qstring.h>
#include <qlabel.h>

SInclinedPlaneWidget::SInclinedPlaneWidget(QWidget* parent, const char* name, WFlags fl)
        : UiInclinedPlaneWidget(parent, name, fl)
{
    mObject = 0;
    mNeedsRedraw = true;
}

void SInclinedPlaneWidget::setObject(SObject * obj)
{
    mObject = dynamic_cast<SInclinedPlane *> (obj);

    if(mObject == 0)
    {
        std::cout << "FATAL ERROR: cannot convert SObject* to SInclinedPlane*" << std::endl;
        return;
    }

    // set properties...
    reset();
}

void SInclinedPlaneWidget::reset()
{
    QString str;

    mNameLineEdit->setText(mObject->getName());

    mAngleLineEdit->setText(str.setNum(mObject->getAngle(), 'f', 3));
    mHeightLineEdit->setText(str.setNum(mObject->getHeight(), 'f', 3));
    mWidthLineEdit->setText(str.setNum(mObject->getWidth(), 'f', 3));

    osg::Vec3 vec = mObject->getPosition();

    mXPosLineEdit->setText(str.setNum(vec.x(), 'f', 3));
    mYPosLineEdit->setText(str.setNum(vec.y(), 'f', 3));
    mZPosLineEdit->setText(str.setNum(vec.z(), 'f', 3));
}

void SInclinedPlaneWidget::save()
{
    if(mObject.valid())
    {
        osg::Vec3 vec;

        mObject->setName(mNameLineEdit->text());
        
        mObject->setAngle(mAngleLineEdit->text().toFloat());
        mObject->setHeight(mHeightLineEdit->text().toFloat());
        mObject->setWidth(mWidthLineEdit->text().toFloat());

        vec[0] = mXPosLineEdit->text().toFloat();
        vec[1] = mYPosLineEdit->text().toFloat();
        vec[2] = mZPosLineEdit->text().toFloat();

        mObject->setPosition(vec);
        
        mObject->createEditorObject();
        reset();
    }
}

