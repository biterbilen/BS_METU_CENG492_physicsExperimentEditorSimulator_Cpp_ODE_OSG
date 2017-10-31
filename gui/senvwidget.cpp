#include "senvwidget.h"

#include <qlineedit.h>
#include <qstring.h>
#include <qcombobox.h>

SEnvWidget::SEnvWidget(QWidget* parent, const char* name, WFlags fl)
        : UiEnvironmentWidget(parent, name, fl)
{
    mObject = 0;
    mNeedsRedraw = false;
}

void SEnvWidget::setObject(SEnvironment * obj)
{
    mObject = obj;

    // set properties...
    reset();
}

void SEnvWidget::reset()
{
    QString str;

    switch(mObject->getSolverType())
    {
    case SEnvironment::QUICKSTEP:
            mSolverComboBox->setCurrentItem(0);;
            mIterationsLineEdit->setEnabled(true);
        break;
    case SEnvironment::FASTSTEP:
            mSolverComboBox->setCurrentItem(1);;
            mIterationsLineEdit->setEnabled(true);
        break;
    case SEnvironment::NORMAL:
            mSolverComboBox->setCurrentItem(2);;
            mIterationsLineEdit->setEnabled(false);
        break;
    }

    mGravityLineEdit->setText(str.setNum(mObject->getGravity().z(), 'f', 3));
    mMuLineEdit->setText(str.setNum(mObject->getMu(), 'f', 3));

    mIterationsLineEdit->setText(str.setNum(mObject->getIterations()));
    mCFMLineEdit->setText(str.setNum(mObject->getCFM(), 'f', 6));
    mERPLineEdit->setText(str.setNum(mObject->getERP(), 'f', 6));
    mStepSizeLineEdit->setText(str.setNum(mObject->getStepSize(), 'f', 6));
}

void SEnvWidget::save()
{
    if(mObject.valid())
    {
        float tmp;

        tmp = mGravityLineEdit->text().toFloat();
        mObject->setGravity(osg::Vec3(0, 0, tmp));
        
        tmp = mMuLineEdit->text().toFloat();
        mObject->setMu(tmp);

        int iter = mIterationsLineEdit->text().toInt();
        mObject->setIterations(iter);

        tmp = mCFMLineEdit->text().toFloat();
        mObject->setCFM(tmp);

        tmp = mERPLineEdit->text().toFloat();
        mObject->setERP(tmp);

        tmp = mStepSizeLineEdit->text().toFloat();
        mObject->setStepSize(tmp);

        switch(mSolverComboBox->currentItem())
        {
        case 0:
            mObject->setSolverType(SEnvironment::QUICKSTEP);
            break;
        case 1:
            mObject->setSolverType(SEnvironment::FASTSTEP);
            break;
        case 2:
            mObject->setSolverType(SEnvironment::NORMAL);
            break;
        }
    }
}

void SEnvWidget::solverChanged(int index)
{
    switch(index)
    {
    case 2: /** Normal WorldStep */
            mIterationsLineEdit->setEnabled(false);
        break;
    default:
            mIterationsLineEdit->setEnabled(true);
        break;
    }
}

