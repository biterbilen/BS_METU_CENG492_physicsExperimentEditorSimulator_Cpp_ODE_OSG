#include "sconvexlenswidget.h"

#include <qcombobox.h>
#include <qlineedit.h>
#include <qstring.h>
#include <qlabel.h>

SConvexLensWidget::SConvexLensWidget(QWidget* parent, const char* name, WFlags fl)
        : UiConvexLensWidget(parent, name, fl)
{
    mObject = 0;
    mNeedsRedraw = true;
}

void SConvexLensWidget::setObject(SObject * obj)
{
    mObject = dynamic_cast<SConvexLens *> (obj);

    if(mObject == 0)
    {
        std::cout << "FATAL ERROR: cannot convert SObject* to SConvexLens*" << std::endl;
        return;
    }

    // set properties...
    reset();
}

void SConvexLensWidget::reset()
{
    QString str;

    mNameLineEdit->setText(mObject->getName());
    mRefractionLineEdit->setText(str.setNum(mObject->getRefractionIndex(), 'f', 3));
//     mRadiusLineEdit->setText(str.setNum(mObject->getRadius(), 'f', 3));
    mThicknessLineEdit->setText(str.setNum(mObject->getThickness(), 'f', 3));

    osg::Vec3 vec = mObject->getPosition();

    mXPosLineEdit->setText(str.setNum(vec.x(), 'f', 3));
    mYPosLineEdit->setText(str.setNum(vec.y(), 'f', 3));
    mZPosLineEdit->setText(str.setNum(vec.z(), 'f', 3));
}

void SConvexLensWidget::save()
{
    if(mObject.valid())
    {
        osg::Vec3 vec;

        mObject->setName(mNameLineEdit->text());

        mObject->setRefractionIndex(mRefractionLineEdit->text().toFloat());
//         mObject->setRadius(mRadiusLineEdit->text().toFloat());
        mObject->setThickness(mThicknessLineEdit->text().toFloat());

        vec[0] = mXPosLineEdit->text().toFloat();
        vec[1] = mYPosLineEdit->text().toFloat();
        vec[2] = mZPosLineEdit->text().toFloat();

        mObject->setPosition(vec);
        
        mObject->createEditorObject();
        reset();
    }
}



