#ifndef __VIEW_H__
#define __VIEW_H__

#include <osgUtil/SceneView>
#include <osg/Referenced>
#include <osg/MatrixTransform>
#include <osg/PolygonMode>
#include <osg/ShadeModel>
#include <osgText/Text>
#include <osg/Projection>

class CameraView : public osg::Referenced
{
public:
    typedef enum {
        FRONT,
        BACK,
        TOP,
        BOTTOM,
        LEFT,
        RIGHT,
        USER
    } ViewType;

    typedef enum {
        ORTHOGONAL,
        PERSPECTIVE
    } ProjectionType;

    typedef enum {
        Y_UP,
        Z_UP
    } AxisSystem;

private:
    osg::ref_ptr<osgUtil::SceneView> sceneView;
    
    /** This is vector of gridNode for this view */
    osg::ref_ptr<osg::MatrixTransform> mGridNode;
    
    osg::ref_ptr<osg::MatrixTransform> mAxisNode;
    
    /** A global state set for to change things like solid/wireframe, smooth/flat... */
    osg::ref_ptr<osg::StateSet> mStateSet;

    osg::ref_ptr<osg::Projection> mHud;
    osg::ref_ptr<osgText::Text> mHudViewText;
    
    float zoom;
    // User camera, moved like a trackball
    osg::Vec3 center_perspective;
    osg::Vec3 orbit_angle_perspective;

    ProjectionType projection;

    ViewType mViewType;
    AxisSystem axis_system;
    osg::Matrix viewMatrix;
    float aspectRatio;
    
    osg::Node * makeHUD();
public:
    CameraView();

    /// Establishes the orthogonal or perspective mode and calls applytrackball
    void applyViewType();

    /// Establishes the aspect ratio of the window
    void setAspectRatio(float ratio) { this->aspectRatio = ratio; }

    /// Establishes the grid node so that this view can update the grid depending on the active view
    void setGridNode(osg::MatrixTransform *gridnode);

    /// Changes the zoom. In orthogonal mode is the scaling, in perspective mode is the FOV
    void setZoom(float zoom) { this->zoom=zoom; applyViewType(); }

    /// Obtains the current zoom bein applied
    float getZoom() { return zoom; }

    /// Sets the data being manipulated by the osg sceneview that has this camera
    void setData(osg::Group *node);

    /// Changes background color. But gray should be cool enough ;)
    void setBackgroundColor(osg::Vec4 v);
    
    /** Configuring the model */
    void setPolygonMode(osg::PolygonMode::Mode m);
    void setShadeModel(osg::ShadeModel::Mode m);

    /// Changes the trackball position
    void setPerspective(osg::Vec3 eye, osg::Vec3 center, osg::Vec3 up);

    /// Selects one of the default trackball sets
    void setViewType(ViewType vt);

    /// Returns the current view type of the camera
    ViewType getViewType() { return mViewType; }

    /// Obtains the scene view associated to this camera
    osgUtil::SceneView* getSceneView() { return sceneView.get(); }

    /// Rotates the trackball
    void rotateCamera(float h_angle, float p_angle);

    /// Moves the trackball center with an offset
    void moveCamera(float x_offset, float y_offset, float z_offset);

    /// Sets the absolute trackball center
    void setCenter(float x_center, float y_center, float z_center);
    
    osg::Vec3 getCenter() { return center_perspective; }

    /// Applies the trackball configuration
    void applyTrackball();

    /// Changes between orthogonal and perspective projection types
    void setProjectionType(ProjectionType p) { projection=p;applyViewType(); }

    /// Returns the view matrix of the camera
    osg::Matrix getViewMatrix() const { return viewMatrix; }
};

#endif
