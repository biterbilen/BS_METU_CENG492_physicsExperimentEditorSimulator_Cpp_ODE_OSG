#ifndef _S_RELATIVE_FORCE_AT_REL_POS_WIDGET_H
#define _S_RELATIVE_FORCE_AT_REL_POS_WIDGET_H 1

#include <qlineedit.h>
#include <qstring.h>

#include "sguiproperties.h"
#include "uiRelativeForceAtRelPosWidget.h"
#include "srelforceatrelpos.h"

class SRelativeForceAtRelPosWidget : public UiRelativeForceAtRelPosWidget, public SGuiProperties 
{
    public:
        SRelativeForceAtRelPosWidget(QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
        ~SRelativeForceAtRelPosWidget() {}
    
    void setForce(SForce * obj);

    void save();
    void reset();

    signals:
        void nameChanged();

    private:
        osg::ref_ptr<SRelForceAtRelPos> mForce;
};

#endif

