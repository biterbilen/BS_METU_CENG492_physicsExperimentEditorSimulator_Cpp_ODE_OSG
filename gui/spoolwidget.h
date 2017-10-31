#ifndef _S_POOL_WIDGET_H
#define _S_POOL_WIDGET_H 1

#include <qlineedit.h>
#include <qstring.h>

#include "sguiproperties.h"
#include "uiPoolWidget.h"
#include "spool.h"

class SPoolWidget : public UiPoolWidget, public SGuiProperties 
{
    //Q_OBJECT
    public:
        SPoolWidget(QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
        ~SPoolWidget() {}
    
    void setObject(SObject * obj);
    
    void save();
    void reset();

    signals:
        void nameChanged();
        
    private:
        osg::ref_ptr<SPool> mObject;
};

#endif

