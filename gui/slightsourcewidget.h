#ifndef _S_LIGHT_SOURCE_WIDGET_H
#define _S_LIGHT_SOURCE_WIDGET_H 1

#include "sguiproperties.h"
#include "uiLightSourceWidget.h"
#include "slight.h"

class SLightSourceWidget : public UiLightSourceWidget, public SGuiProperties 
{
    //Q_OBJECT
    public:
        SLightSourceWidget(QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
        ~SLightSourceWidget() {}
    
    void setObject(SObject * obj);
    
    void save();
    void reset();

    signals:
        void nameChanged();
        
    private:
        osg::ref_ptr<SLight> mObject;
};

#endif

