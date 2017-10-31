#ifndef _S_NORMAL_FORCE_AT_REL_POS_WIDGET_H
#define _S_NORMAL_FORCE_AT_REL_POS_WIDGET_H 1

#include <qlineedit.h>
#include <qstring.h>

#include "sguiproperties.h"
#include "uiNormalForceAtRelPosWidget.h"
#include "sforceatrelpos.h"

class SNormalForceAtRelPosWidget : public UiNormalForceAtRelPosWidget, public SGuiProperties 
{
    public:
        SNormalForceAtRelPosWidget(QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
        ~SNormalForceAtRelPosWidget() {}

    void setForce(SForce * obj);

    void save();
    void reset();

    signals:
        void nameChanged();

    private:
        osg::ref_ptr<SForceAtRelPos> mForce;
};

#endif

