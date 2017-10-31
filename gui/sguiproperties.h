#ifndef _S_GUI_PROPERTIES_H
#define _S_GUI_PROPERTIES_H 1

#include "sobject.h"
#include "sforce.h"
#include "sjoint.h"
#include "senvironment.h"

class SGuiProperties
{
    public:
        SGuiProperties();
        virtual ~SGuiProperties();
        
        virtual void setObject(SObject *) {}
        virtual void setJoint(SJoint *) {}
        virtual void setForce(SForce *) {}
        virtual void setEnvironment(SEnvironment *) {}
        
        virtual void save() {}
        virtual void reset() {}

        bool needsRedraw() { return mNeedsRedraw; }
        bool isChanged() { return mChanged; }
    
    protected:
        bool mNeedsRedraw;
        bool mChanged;
};

#endif

