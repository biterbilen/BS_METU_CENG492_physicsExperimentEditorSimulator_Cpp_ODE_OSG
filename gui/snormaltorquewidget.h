#ifndef _S_NORMAL_TORQUE_WIDGET_H
#define _S_NORMAL_TORQUE_WIDGET_H 1

#include <qlineedit.h>
#include <qstring.h>

#include "sguiproperties.h"
#include "uiNormalTorqueWidget.h"
#include "storque.h"

class SNormalTorqueWidget : public UiNormalTorqueWidget, public SGuiProperties 
{
    public:
        SNormalTorqueWidget(QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
        ~SNormalTorqueWidget() {}
    
    void setForce(SForce * obj);

    void save();
    void reset();

    signals:
        void nameChanged();

    private:
        osg::ref_ptr<STorque> mForce;
};

#endif

