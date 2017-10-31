#include "slightsourcewidget.h"

#include <qcombobox.h>
#include <qlineedit.h>
#include <qstring.h>
#include <qlabel.h>

SLightSourceWidget::SLightSourceWidget(QWidget* parent, const char* name, WFlags fl)
        : UiLightSourceWidget(parent, name, fl)
{
    mObject = 0;
    mNeedsRedraw = true;
}

void SLightSourceWidget::setObject(SObject * obj)
{
    mObject = dynamic_cast<SLight *> (obj);

    if(mObject == 0)
    {
        std::cout << "FATAL ERROR: cannot convert SObject* to SLight*" << std::endl;
        return;
    }

    // set properties...
    reset();
}

void SLightSourceWidget::reset()
{
    QString str;

    mNameLineEdit->setText(mObject->getName());

    osg::Vec3 vec = mObject->getPosition();

    mXPosLineEdit->setText(str.setNum(vec.x(), 'f', 3));
    mYPosLineEdit->setText(str.setNum(vec.y(), 'f', 3));
    mZPosLineEdit->setText(str.setNum(vec.z(), 'f', 3));
}

void SLightSourceWidget::save()
{
    if(mObject.valid())
    {
        osg::Vec3 vec;

        mObject->setName(mNameLineEdit->text());

        vec[0] = mXPosLineEdit->text().toFloat();
        vec[1] = mYPosLineEdit->text().toFloat();
        vec[2] = mZPosLineEdit->text().toFloat();

        mObject->setPosition(vec);

        mObject->createEditorObject();
        reset();
    }
}



