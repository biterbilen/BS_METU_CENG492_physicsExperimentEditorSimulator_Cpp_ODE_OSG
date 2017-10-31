#ifndef _S_BOX_WIDGET_H
#define _S_BOX_WIDGET_H 1

#include "sguiproperties.h"
#include "uiBoxWidget.h"
#include "sbox.h"

class SBoxWidget : public UiBoxWidget, public SGuiProperties 
{
    //Q_OBJECT
    public:
        SBoxWidget(QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
        ~SBoxWidget() {}
    
    void setObject(SObject * obj);
    
    void save();
    void reset();

    signals:
        void nameChanged();
        
    private:
        osg::ref_ptr<SBox> mObject;
};

#endif

