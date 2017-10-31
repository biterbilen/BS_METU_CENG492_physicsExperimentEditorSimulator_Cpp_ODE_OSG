#ifndef _S_GEAR_WIDGET_H
#define _S_GEAR_WIDGET_H 1

#include <qlineedit.h>
#include <qspinbox.h>
#include <qcombobox.h>
#include <qstring.h>

#include "sguiproperties.h"
#include "uiGearWidget.h"
#include "sgear.h"

class SGearWidget : public UiGearWidget, public SGuiProperties 
{
    //Q_OBJECT
    public:
        SGearWidget(QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
        ~SGearWidget() {}
    
    void setObject(SObject * obj);
    
    void save();
    void reset();

    signals:
        void nameChanged();
        
    private:
        osg::ref_ptr<SGear> mObject;
};

#endif

