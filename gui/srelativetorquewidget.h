#ifndef _S_RELATIVE_TORQUE_WIDGET_H
#define _S_RELATIVE_TORQUE_WIDGET_H 1

#include <qlineedit.h>
#include <qstring.h>

#include "sguiproperties.h"
#include "uiRelativeTorqueWidget.h"
#include "sreltorque.h"

class SRelativeTorqueWidget : public UiRelativeTorqueWidget, public SGuiProperties 
{
    public:
        SRelativeTorqueWidget(QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
        ~SRelativeTorqueWidget() {}
    
    void setForce(SForce * obj);

    void save();
    void reset();

    signals:
        void nameChanged();

    private:
        osg::ref_ptr<SRelTorque> mForce;
};

#endif

