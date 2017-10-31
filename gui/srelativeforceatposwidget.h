#ifndef _S_RELATIVE_FORCE_AT_POS_WIDGET_H
#define _S_RELATIVE_FORCE_AT_POS_WIDGET_H 1

#include <qlineedit.h>
#include <qstring.h>

#include "sguiproperties.h"
#include "uiRelativeForceAtPosWidget.h"
#include "srelforceatpos.h"

class SRelativeForceAtPosWidget : public UiRelativeForceAtPosWidget, public SGuiProperties 
{
    public:
        SRelativeForceAtPosWidget(QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
        ~SRelativeForceAtPosWidget() {}
    
    void setForce(SForce * obj);

    void save();
    void reset();

    signals:
        void nameChanged();

    private:
        osg::ref_ptr<SRelForceAtPos> mForce;
};

#endif

