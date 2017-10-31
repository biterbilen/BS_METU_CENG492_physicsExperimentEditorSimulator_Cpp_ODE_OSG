#ifndef _S_CON_MIRROR_WIDGET_H
#define _S_CON_MIRROR_WIDGET_H 1

#include "sguiproperties.h"
#include "uiConMirrorWidget.h"
#include "sconmirror.h"

class SConMirrorWidget : public UiConMirrorWidget, public SGuiProperties 
{
    //Q_OBJECT
    public:
        SConMirrorWidget(QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
        ~SConMirrorWidget() {}
    
    void setObject(SObject * obj);
    
    void save();
    void reset();

    signals:
        void nameChanged();
        
    private:
        osg::ref_ptr<SConMirror> mObject;
};

#endif

