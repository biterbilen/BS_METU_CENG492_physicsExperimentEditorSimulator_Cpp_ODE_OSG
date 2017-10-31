#ifndef _S_NORMAL_FORCE_AT_POS_WIDGET_H
#define _S_NORMAL_FORCE_AT_POS_WIDGET_H 1

#include <qlineedit.h>
#include <qstring.h>

#include "sguiproperties.h"
#include "uiNormalForceAtPosWidget.h"
#include "sforceatpos.h"

class SNormalForceAtPosWidget : public UiNormalForceAtPosWidget, public SGuiProperties 
{
    public:
        SNormalForceAtPosWidget(QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
        ~SNormalForceAtPosWidget() {}
    
    void setForce(SForce * obj);

    void save();
    void reset();

    signals:
        void nameChanged();

    private:
        osg::ref_ptr<SForceAtPos> mForce;
};

#endif

