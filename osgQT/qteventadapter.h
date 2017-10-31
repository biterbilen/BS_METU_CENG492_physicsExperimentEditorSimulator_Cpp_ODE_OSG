#ifndef _QT_EVENT_ADAPTER_H
#define _QT_EVENT_ADAPTER_H 1

#include <osgGA/GUIEventAdapter>
#include <osg/Timer>
#include <qevent.h>

/// QtEventAdapter allows window-system-agnostic osgGA classes to access Qt events

/**
 * QtEventAdapter - a class to adapt Qt events into abstract GUI events for
 * osgGA classes.
 */

class QtEventAdapter : public osgGA::GUIEventAdapter
{

public:

    QtEventAdapter();
    virtual ~QtEventAdapter() {}

    // Overriden from GUIEventAdapter...

    /** Get the EventType of the GUI event.*/
    virtual EventType getEventType() const { return eventType_; }

    /** key pressed, return -1 if inapropriate for this event. */
    virtual int getKey() const { return key_; }

    /** button pressed/released, return -1 if inappropriate for this event.*/
    virtual int getButton() const { return button_; }

    /** window minimum x. */
    virtual float getXmin() const { return xMin_; }

    /** window maximum x. */
    virtual float getXmax() const { return xMax_; }

    /** window minimum y. */
    virtual float getYmin() const { return yMin_; }

    /** window maximum y. */
    virtual float getYmax() const { return yMax_; }

    /** current mouse x position.*/
    virtual float getX() const { return mX_; }

    /** current mouse y position.*/
    virtual float getY() const { return mY_; }

    /** current mouse button state */
    virtual unsigned int getButtonMask() const { return buttonMask_; }

    /** current mod key state */
    virtual unsigned int getModKeyMask() const { return modKeyMask_; }

    /** time in seconds of event. */
    virtual double time() const { return timestamp_; }


    // Adapt methods...

    /** adapts a mouse press */
    void adaptMousePressEvent(const QMouseEvent *);

    /** adapts a mouse release */
    void adaptMouseReleaseEvent(const QMouseEvent *);

    /** adapts a mouse move */
    void adaptMouseMoveEvent(const QMouseEvent *);

    /** adapts a key press */
    void adaptKeyPressEvent(const QKeyEvent *);

    /** adapts a key press */
    void adaptKeyReleaseEvent(const QKeyEvent *);

    /** method for adapting resize events. */
    void adaptResize(int Xmin, int Ymin, int Xmax, int Ymax);

    /** method for adapting frame events, i.e. idle/display callback.
     * this was like that in GTK verison of this viewer*/
    //void adaptFrame(int mouseX, int mouseY);
    
    /*TODO check back QtViewer::app function*/
    void adaptFrame(/*ReferenceTime*/);

protected:

    EventType eventType_;
    int key_;
    int button_;
    float xMin_,xMax_;
    float yMin_,yMax_;
    float mX_;
    float mY_;
    unsigned int buttonMask_;
	unsigned int modKeyMask_;
    double timestamp_;

    // This should be easier than this:
    static osg::Timer timer_;
    static const osg::Timer_t initialTick_;

    /** Utility methods to avoid lots of duplicate code */
    void setMouseEventDetails(const QMouseEvent*);
};


#endif
