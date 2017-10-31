#include "cameraview.h"

#include <osg/Matrix>
#include <osg/Geode>
#include <osg/ShapeDrawable>
#include <osg/Geometry>
#include <osg/LineWidth>

#define ROT_90_RAD 1.570796327
#define SQR(x) ((x)*(x))
#define RAD2DEG(x) ( (x)*180/3.14159265356 )

#define DEFAULT_BACKGROUND_COLOR 0.5,0.5,0.5,1.0

#define CAMERA_DISTANCE 30

#ifndef PI
#  define PI 3.14159265359
#endif

osg::Node * CameraView::makeHUD()
{
    osg::MatrixTransform* modelview_abs = new osg::MatrixTransform;
    modelview_abs->setReferenceFrame(osg::Transform::RELATIVE_TO_ABSOLUTE);
    modelview_abs->setMatrix(osg::Matrix::identity());

    osg::Geode* geode = new osg::Geode();

    std::string timesFont("arial.ttf");

    // turn lighting off for the text and disable depth test to ensure its always ontop.
    osg::StateSet* stateset = geode->getOrCreateStateSet();
    stateset->setMode(GL_LIGHTING,osg::StateAttribute::OFF);

    // Disable depth test, and make sure that the hud is drawn after everything
    // else so that it always appears ontop.
    stateset->setMode(GL_DEPTH_TEST,osg::StateAttribute::OFF);
    stateset->setRenderBinDetails(11, "RenderBin");

    osg::Vec3 position(15.0f, 15.0f,0.0f);

    geode->addDrawable( mHudViewText.get() );
    mHudViewText->setFont(timesFont);
    mHudViewText->setPosition(position);
 
    osg::ShapeDrawable * box;
    box = new osg::ShapeDrawable(new osg::Box(osg::Vec3(15, 15, 0), 10, 0, 0));
    geode->addDrawable(box);
    
    modelview_abs->addChild(geode);
    return modelview_abs;
}

osg::MatrixTransform *makeGrid()
{
    osg::MatrixTransform *transform=new osg::MatrixTransform();
    osg::Geode *geode=new osg::Geode();
    osg::Geometry *geom=new osg::Geometry();
    osg::Vec3Array *vertices=new osg::Vec3Array();
    
    int i;
    float unitSize=1.0f;
    float edgeSize=50.0f;
    osg::Vec4 grid_color=osg::Vec4(0.75f,0.75f,0.75f,1.0f);
    osg::Vec4 axis_color=osg::Vec4(1.0f,1.0f,1.0f,1.0f);
    
    int numLines=(int)(edgeSize/unitSize)/2;
    float radius=edgeSize/2;
    vertices->push_back(osg::Vec3(0.0f,-radius,0.0f));
    vertices->push_back(osg::Vec3(0.0f, radius,0.0f));
    vertices->push_back(osg::Vec3(-radius,0.0f,0.0f));
    vertices->push_back(osg::Vec3( radius,0.0f,0.0f));
    for (i=1;i<numLines;i++)
    {
        vertices->push_back(osg::Vec3(i*unitSize,-radius,0.0f));
        vertices->push_back(osg::Vec3(i*unitSize,radius,0.0f));
        vertices->push_back(osg::Vec3(-i*unitSize,-radius,0.0f));
        vertices->push_back(osg::Vec3(-i*unitSize,radius,0.0f));
        vertices->push_back(osg::Vec3(-radius,i*unitSize,0.0f));
        vertices->push_back(osg::Vec3(radius,i*unitSize,0.0f));
        vertices->push_back(osg::Vec3(-radius,-i*unitSize,0.0f));
        vertices->push_back(osg::Vec3(radius,-i*unitSize,0.0f));
    }
    geom->setVertexArray(vertices);
    osg::Vec4Array* colors = new osg::Vec4Array;
    colors->push_back(axis_color);
    colors->push_back(grid_color);
    geom->setColorArray(colors);
    geom->setColorBinding(osg::Geometry::BIND_PER_PRIMITIVE_SET);

    osg::Vec3Array* normals = new osg::Vec3Array;
    normals->push_back(osg::Vec3(0.0f,-1.0f,0.0f));
    geom->setNormalArray(normals);
    geom->setNormalBinding(osg::Geometry::BIND_OVERALL);

    geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES,0,4));
    geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES,4,vertices->size()-4));
    geode->addDrawable(geom);
    osg::StateSet *set=new osg::StateSet();
    set->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
    geode->setStateSet(set);
    transform->addChild(geode);
    return transform;
}

osg::MatrixTransform *makeAxis(float x, float y, float z)
{
    osg::MatrixTransform *transform=new osg::MatrixTransform();
    osg::Geode *geode=new osg::Geode();
    osg::Geometry *geom=new osg::Geometry();
    osg::Vec3Array *vertices=new osg::Vec3Array();

    vertices->push_back(osg::Vec3(x, y, z));
    vertices->push_back(osg::Vec3(1, y, z));

    vertices->push_back(osg::Vec3(x, y, z));
    vertices->push_back(osg::Vec3(x,1, z));

    vertices->push_back(osg::Vec3(x, y, z));
    vertices->push_back(osg::Vec3(x, y,1));

    geom->setVertexArray(vertices);
    osg::Vec4Array* colors = new osg::Vec4Array;
    colors->push_back(osg::Vec4(1.0f, 0.0f,0.0f,1.0f));
    colors->push_back(osg::Vec4(0.0f,1.0f,0.0f,1.0f));
    colors->push_back(osg::Vec4(0.0f,0.0f,1.0f,1.0f));
    geom->setColorArray(colors);
    geom->setColorBinding(osg::Geometry::BIND_PER_PRIMITIVE);

    geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES,0,2));
    geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES,2,2));
    geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES,4,2));
    geode->addDrawable(geom);
    osg::StateSet *set=new osg::StateSet();
    set->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
    osg::LineWidth *width=new osg::LineWidth();
    width->setWidth(2.0f);
    set->setAttributeAndModes(width, osg::StateAttribute::ON);
    geode->setStateSet(set);
    transform->addChild(geode);
    return transform;
}

CameraView::CameraView()
{
    aspectRatio=1.0f;
    zoom=0.1f;
    
    mHud  = new osg::Projection();
    mHud->setMatrix(osg::Matrix::ortho2D(0, 1024, 0, 768));
    mHudViewText = new  osgText::Text();

    sceneView=new osgUtil::SceneView();
    sceneView->setDefaults();
    sceneView->setBackgroundColor(osg::Vec4(DEFAULT_BACKGROUND_COLOR));
    sceneView->setComputeNearFarMode(osgUtil::CullVisitor::COMPUTE_NEAR_FAR_USING_BOUNDING_VOLUMES);
    sceneView->init();
        
    center_perspective=osg::Vec3(0,0,0);
    projection=CameraView::ORTHOGONAL;
    axis_system=CameraView::Z_UP;
    setViewType(CameraView::TOP);
}

void CameraView::setData(osg::Group *node)
{
    sceneView->setSceneData(node);

    mStateSet = new osg::StateSet();
    node->setStateSet(mStateSet.get());

//    mHud->addChild(makeHUD());
    node->addChild(mHud.get());
    
    /** Make axis node and add to the scene data */
    mAxisNode=makeAxis(0.0f, 0.0f, 0.0f);
    node->addChild(mAxisNode.get());
    
    /** Make grid node and add to the scene data */
    mGridNode = makeGrid();
    node->addChild(mGridNode.get());
}

void CameraView::setBackgroundColor(osg::Vec4 v)
{
    sceneView->setBackgroundColor(v);
}

void CameraView::setPolygonMode(osg::PolygonMode::Mode m)
{
    osg::PolygonMode* polyModeObj = new osg::PolygonMode;
    polyModeObj->setMode(osg::PolygonMode::FRONT_AND_BACK,m);
    mStateSet->setAttribute(polyModeObj);
}

void CameraView::setShadeModel(osg::ShadeModel::Mode m)
{
    osg::StateSet* stateset = mStateSet.get();
    osg::ShadeModel* shademodel = dynamic_cast<osg::ShadeModel*>(stateset->getAttribute(osg::StateAttribute::SHADEMODEL));
    if (!shademodel)
    {
        shademodel = new osg::ShadeModel();
        stateset->setAttribute(shademodel,osg::StateAttribute::OVERRIDE);
    }
    shademodel->setMode(m);
}

void CameraView::applyTrackball()
{
    osg::Matrix mX=osg::Matrix::rotate(orbit_angle_perspective[0],1,0,0);
    osg::Matrix mY=osg::Matrix::rotate(orbit_angle_perspective[1],0,1,0);
    osg::Matrix mZ=osg::Matrix::rotate(orbit_angle_perspective[2],0,0,1);
    osg::Matrix transf=mX*mY*mZ;
    osg::Matrix translate=osg::Matrix::translate(center_perspective);

    osg::Vec3 eye(0,0,CAMERA_DISTANCE);
    eye=eye*transf*translate;

    osg::Vec3 center(0,0,0);
    center=center*transf+eye;

    osg::Vec3 up(0,0,1);
    up=up*transf;
    sceneView->setViewMatrixAsLookAt(eye, center, up);
    viewMatrix = transf*translate;
}

void CameraView::applyViewType()
{
    applyTrackball();
    switch (projection)
    {
    case ORTHOGONAL:
        sceneView->setProjectionMatrixAsOrtho(-1.0f/zoom*aspectRatio,
                                              1.0f/zoom*aspectRatio,
                                              -1.0f/zoom,
                                              1.0f/zoom,
                                              -CAMERA_DISTANCE,
                                              CAMERA_DISTANCE*10);
        break;
    case PERSPECTIVE:
                sceneView->setProjectionMatrixAsPerspective(60/*2*RAD2DEG(atanf(1.5f/zoom/CAMERA_DISTANCE))*/,
                        aspectRatio,
                       -CAMERA_DISTANCE,
                       CAMERA_DISTANCE*10);

//         sceneView->setProjectionMatrixAsFrustum(-1.0f/zoom*aspectRatio,
//                                                 1.0f/zoom*aspectRatio,
//                                                 -1.0f/zoom,
//                                                 1.0f/zoom,
//                                                 -CAMERA_DISTANCE,
//                                                 CAMERA_DISTANCE*10);
        break;
    }
}

void CameraView::rotateCamera(float h_angle, float p_angle)
{
    orbit_angle_perspective+=osg::Vec3(h_angle, p_angle, 0);
    if (mGridNode.valid())
    {
        osg::Matrix matrix;
        mGridNode->setMatrix(matrix);
    }
    applyViewType();
}

void CameraView::moveCamera(float x_offset, float y_offset, float z_offset)
{
    center_perspective+=osg::Vec3(x_offset, y_offset, z_offset);
    applyViewType();
}

void CameraView::setCenter(float x_center, float y_center, float z_center)
{
    center_perspective=osg::Vec3(x_center, y_center, z_center);
    applyViewType();
}


void CameraView::setViewType(ViewType vt)
{
    mViewType = vt;

    switch (vt)
    {
    case TOP:
        {
            orbit_angle_perspective=osg::Vec3(0, 0, 0);
            mHudViewText->setText("Top");
        }
        break;
    case FRONT:
        {
            orbit_angle_perspective=osg::Vec3(PI/2, 0, 0);
            mHudViewText->setText("Front");
        }
        break;
    case BACK:
        {
            orbit_angle_perspective=osg::Vec3(PI/2, 0, PI);
            mHudViewText->setText("Back");
        }
        break;
    case BOTTOM:
        {
            orbit_angle_perspective=osg::Vec3(0, PI, 0);
            mHudViewText->setText("Bottom");
        }
        break;
    case LEFT:
        {
            orbit_angle_perspective=osg::Vec3(PI/2, 0, -PI/2);
            mHudViewText->setText("Left");
        }
        break;
    case RIGHT:
        {
            orbit_angle_perspective=osg::Vec3(PI/2, 0, PI/2);
            mHudViewText->setText("Right");
        }
        break;
    case USER:
        {
            orbit_angle_perspective=osg::Vec3(PI/4, 0, PI/4);
            mHudViewText->setText("Perspective");
        }
        break;
    }

    if (mGridNode.valid())
    {
        osg::Matrix matrix;
        switch (vt)
        {
        case TOP:break;
        case FRONT: matrix.makeRotate(ROT_90_RAD, 1, 0, 0); break;
        case BACK: matrix.makeRotate(ROT_90_RAD,1,0,0);break;
        case BOTTOM:break;
        case LEFT: matrix.makeRotate(ROT_90_RAD, 0, 1, 0);break;
        case RIGHT:matrix.makeRotate(ROT_90_RAD, 0, 1, 0);break;
        case USER: break;
        }
        mGridNode->setMatrix(matrix);
    }
    applyViewType();
}


