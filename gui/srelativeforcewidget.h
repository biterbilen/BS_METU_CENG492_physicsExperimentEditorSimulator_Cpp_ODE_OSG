#ifndef _S_RELATIVE_FORCE_WIDGET_H
#define _S_RELATIVE_FORCE_WIDGET_H 1

#include <qlineedit.h>
#include <qstring.h>

#include "sguiproperties.h"
#include "uiRelativeForceWidget.h"
#include "srelforce.h"

class SRelativeForceWidget : public UiRelativeForceWidget, public SGuiProperties 
{
    public:
        SRelativeForceWidget(QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
        ~SRelativeForceWidget() {}
    
    void setForce(SForce * obj);

    void save();
    void reset();

    signals:
        void nameChanged();

    private:
        osg::ref_ptr<SRelForce> mForce;
};

#endif

