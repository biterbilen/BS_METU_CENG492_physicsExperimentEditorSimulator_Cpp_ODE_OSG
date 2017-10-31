#include "spoolwidget.h"


SPoolWidget::SPoolWidget(QWidget* parent, const char* name, WFlags fl)
        : UiPoolWidget(parent, name, fl)
{
    mObject = 0;
    mNeedsRedraw = true;
}

void SPoolWidget::setObject(SObject * obj)
{
    mObject = dynamic_cast<SPool *> (obj);

    if(mObject == 0)
    {
        std::cout << "FATAL ERROR: cannot convert SObject* to SPool*" << std::endl;
        return;
    }

    // set properties...
    reset();
}

void SPoolWidget::reset()
{
    QString str;

    mNameLineEdit->setText(mObject->getName());
    
    mDepthLineEdit->setText(str.setNum(mObject->getDepth(), 'f', 3));
    mDensityLineEdit->setText(str.setNum(mObject->getLiquidDensity(), 'f', 3));

    osg::Vec3 vec = mObject->getDimensions();

    mXDimLineEdit->setText(str.setNum(vec.x(), 'f', 3));
    mYDimLineEdit->setText(str.setNum(vec.y(), 'f', 3));
    mZDimLineEdit->setText(str.setNum(vec.z(), 'f', 3));

    vec = mObject->getPosition();

    mXPosLineEdit->setText(str.setNum(vec.x(), 'f', 3));
    mYPosLineEdit->setText(str.setNum(vec.y(), 'f', 3));
    mZPosLineEdit->setText(str.setNum(vec.z(), 'f', 3));
}

void SPoolWidget::save()
{
    if(mObject.valid())
    {
        osg::Vec3 vec;

        mObject->setName(mNameLineEdit->text());
        
        mObject->setDepth(mDepthLineEdit->text().toFloat());
        mObject->setLiquidDensity(mDensityLineEdit->text().toFloat());

        vec[0] = mXDimLineEdit->text().toFloat();
        vec[1] = mYDimLineEdit->text().toFloat();
        vec[2] = mZDimLineEdit->text().toFloat();
        
        mObject->setDimensions(vec);
        mObject->createEditorObject();

        vec[0] = mXPosLineEdit->text().toFloat();
        vec[1] = mYPosLineEdit->text().toFloat();
        vec[2] = mZPosLineEdit->text().toFloat();
        
        mObject->setPosition(vec);
    }
}



