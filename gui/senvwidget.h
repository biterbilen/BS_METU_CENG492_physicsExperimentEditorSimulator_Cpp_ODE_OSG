#ifndef _S_ENV_WIDGET_H
#define _S_ENV_WIDGET_H 1

#include "sguiproperties.h"
#include "uiEnvironmentWidget.h"
#include "senvironment.h"

class SEnvWidget : public UiEnvironmentWidget, public SGuiProperties 
{
    //Q_OBJECT
    public:
        SEnvWidget(QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
        ~SEnvWidget() {}
    
    void setObject(SEnvironment * obj);
    
    void save();
    void reset();

    protected slots:
        void solverChanged(int);
        
    private:
        osg::ref_ptr<SEnvironment> mObject;
};

#endif

