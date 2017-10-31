#ifndef _QT_ACTIONADAPTER_h
#define _QT_ACTIONADAPTER_h 1

#include <osgGA/GUIActionAdapter>

/**
 * QtActionAdapter - a class to adapt osgGA-specified actions into feedback for our app.
 * QtActionAdapter allows window-system-agnostic osgGA classes to tell
 * the window system what to do
 */

class QtActionAdapter : public osgGA::GUIActionAdapter
{
public:

    QtActionAdapter():mNeedRedraw(false),
                      mNeedContinuousUpdate(false),
                      mNeedWarpPointer(false),
                      mX(-1),mY(-1) {};

    virtual ~QtActionAdapter() {}

    virtual void requestRedraw()
    {
        mNeedRedraw=true;
    }

    virtual void requestContinuousUpdate(bool needed=true)
    {
        mNeedContinuousUpdate=needed;
    }

    virtual void requestWarpPointer(float x,float y)
    {
        mNeedWarpPointer=true;
        mX=x;
        mY=y;
    }

    virtual void clearWarpRequest()
    {
        mNeedWarpPointer = false;
    }

    bool getNeedRedraw() const {return mNeedRedraw;}
    bool getNeedContinuousUpdate() const	{return mNeedContinuousUpdate;}
    bool getNeedWarpPointer(float &x, float &y) const
    {
        if(mNeedWarpPointer)
        {
            x=mX;
            y=mY;
            return mNeedWarpPointer;
        }
        return false;
    }

private:

    bool mNeedRedraw;
    bool mNeedContinuousUpdate;
    bool mNeedWarpPointer;
    float mX, mY;
};

#endif

