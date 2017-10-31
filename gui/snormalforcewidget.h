#ifndef _S_NORMAL_FORCE_WIDGET_H
#define _S_NORMAL_FORCE_WIDGET_H 1

#include <qlineedit.h>
#include <qstring.h>

#include "sguiproperties.h"
#include "uiNormalForceWidget.h"
#include "snormalforce.h"

class SNormalForceWidget : public UiNormalForceWidget, public SGuiProperties 
{
    public:
        SNormalForceWidget(QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
        ~SNormalForceWidget() {}
    
    void setForce(SForce * obj);

    void save();
    void reset();

    signals:
        void nameChanged();

    private:
        osg::ref_ptr<SNormalForce> mForce;
};

#endif

