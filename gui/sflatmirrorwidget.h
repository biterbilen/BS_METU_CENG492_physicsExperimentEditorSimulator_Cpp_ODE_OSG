#ifndef _S_FLAT_MIRROR_WIDGET_H
#define _S_FLAT_MIRROR_WIDGET_H 1

#include "sguiproperties.h"
#include "uiFlatMirrorWidget.h"
#include "sflatmirror.h"

class SFlatMirrorWidget : public UiFlatMirrorWidget, public SGuiProperties 
{
    //Q_OBJECT
    public:
        SFlatMirrorWidget(QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
        ~SFlatMirrorWidget() {}
    
    void setObject(SObject * obj);
    
    void save();
    void reset();

    signals:
        void nameChanged();
        
    private:
        osg::ref_ptr<SFlatMirror> mObject;
};

#endif

