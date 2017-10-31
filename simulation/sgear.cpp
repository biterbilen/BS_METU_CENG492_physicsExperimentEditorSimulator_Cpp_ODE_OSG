#include "sgear.h"

#define SQRT2    1.4142136
#define GEARTHICKNESS 0.5

string SGear::mCounter = "1";

SGear::SGear(GearTypes g) : SObject(SObject::DYNAMIC)
{
    mObjectType = SObject::GEAR;
    mGearType = g;
    mPosition.set(0,0,0);

    mName = "Gear" + mCounter;
    mCounter = incrementString(mCounter, mCounter.length()-1);
}

SGear::SGear(float r, int t, osg::Vec3 p) : SObject(SObject::DYNAMIC)
{
    mObjectType = SObject::GEAR;
    mGearType = SGear::BOXGEAR;

    mRadius = r;
    mToothNumber = t;
    mPosition = p;

    setMass(1);

    mName = "Gear" + mCounter;
    mCounter = incrementString(mCounter, mCounter.length()-1);
}

void SGear::setRadius(float r)
{
    mRadius = r;
}

string SGear::getCounter()
{
    return mCounter;
}

void SGear::setCounter(string counter)
{
    mCounter = counter;
}

float SGear::getRadius() const
{
    return mRadius;
}

void SGear::setToothNumber(int t)
{
    mToothNumber = t;

}

int SGear::getToothNumber() const
{
    return mToothNumber;
}

SGear::GearTypes SGear::getGearType() const
{
    return mGearType;
}

void SGear::init()
{
    mAngle = 2.0*M_PI/mToothNumber; // 2*M_PI<=>mToothNumber => mAngle*M_PI<=>1
    mInitAngle = 0;

    if (mGearType == BOXGEAR)
    {
        mBoxDim = M_PI*mRadius/mToothNumber; // perimeter / 2
        mBoxCenter.set(mRadius + mBoxDim/4,0,0);
    }
    else if (mGearType == PRISMGEAR)
    {
        mBoxDim = SQRT2*mRadius*sin(mAngle/2);
        mInitAngle = 0.25*M_PI;
        std::cout<<"box dim:" <<mBoxDim << std::endl;
        mBoxCenter.set(sqrt(mRadius*mRadius - mBoxDim*mBoxDim/2),0,0);
    }

    //mBoxCenter.set(mRadius+mBoxDim/4,0,0);
    mBoxCenterOrientationMatrix.makeIdentity();
    mBoxCenterOrientationMatrix.makeRotate(mAngle, osg::Vec3(0,0,1)); // FOR CENTERS

    //mBoxCenter.set(mRadius + mBoxDim/4,0,0);

    mGeoms.clear();
    mGeoms.resize(mToothNumber);
}

void SGear::setMass(float m)
{
    dMassSetBoxTotal(&mMass, m, mRadius, mRadius, GEARTHICKNESS);
}

void SGear::createSimObject(dWorldID world, dSpaceID space)
{
    //ODE related stuff
    mBodyID = dBodyCreate(world);
    dBodySetGravityMode (mBodyID, 0);
    dBodySetPosition(mBodyID, mPosition.x(), mPosition.y(), mPosition.z());
    dBodySetMass(mBodyID, &mMass);

    float tmpBoxDim = SQRT2*mRadius;
    mGeomID = dCreateBox(space, tmpBoxDim, tmpBoxDim, GEARTHICKNESS);
    dGeomSetBody(mGeomID, mBodyID);

    // start accumulating masses for the encapsulated geometries

    osg::Quat quatr;

    vector<dGeomID> g2;
    g2.clear();
    g2.resize(mToothNumber);


    for (int k = 0; k < mToothNumber; k++)
    {
        osg::Quat quatr(mInitAngle-mAngle*k, osg::Vec3(0,0,1));

        mGeoms[k] = dCreateGeomTransform(space);
//         dGeomTransformSetCleanup (mGeoms[k], 1);

        g2[k] = dCreateBox(space, mBoxDim, mBoxDim, GEARTHICKNESS);

        dGeomTransformSetGeom (mGeoms[k],g2[k]);

        // set the transformation
        dGeomSetPosition (g2[k], mBoxCenter.x(),mBoxCenter.y(),mBoxCenter.z());

        dQuaternion quat;
        quat[0] = quatr[3];
        quat[1] = quatr[0];
        quat[2] = quatr[1];
        quat[3] = quatr[2];
        dGeomSetQuaternion(g2[k], quat);

        mBoxCenter = mBoxCenterOrientationMatrix * mBoxCenter;

        dGeomSetBody(mGeoms[k], mBodyID);
    }

    dQuaternion quat;
    quat[0] = mOrientation[3];
    quat[1] = mOrientation[0];
    quat[2] = mOrientation[1];
    quat[3] = mOrientation[2];
    dGeomSetQuaternion(mGeomID, quat);

    dBodyID tmpBodyID = dBodyCreate(world);
    dJointID jointID = dJointCreateHinge2(world, 0);   
    dJointAttach(jointID, mBodyID, tmpBodyID);
    dJointSetHinge2Anchor(jointID, mPosition.x(), mPosition.y(), mPosition.z());
    
     osg::Vec3 axis = osg::Matrix(mOrientation) * osg::Vec3(0, 0, 1);
     osg::Vec3 axis2;
    
    if( (osg::Vec3(1,1,1) ^ axis) == osg::Vec3(0,0,0))
            axis2 =  osg::Vec3(1,1,2) ^ axis;
    else axis2 = osg::Vec3(1,1,1) ^ axis;
    
    dJointSetHinge2Axis1(jointID, axis2.x(), axis2.y(), axis2.z());   
    dJointSetHinge2Axis2(jointID, axis.x(), axis.y(), axis.z());
    
     dJointSetHinge2Param(jointID, dParamLoStop, 0);
     dJointSetHinge2Param(jointID, dParamHiStop, 0);
    
     dJointID sliderID = dJointCreateSlider(world, 0);
    dJointAttach(sliderID, tmpBodyID, 0);
    dJointSetSliderAxis(sliderID, axis.x(), axis.y(), axis.z());
    
     dJointSetSliderParam(sliderID, dParamLoStop, 0);
     dJointSetSliderParam(sliderID, dParamHiStop, 0);
}

osg::Node *SGear::createVisualObject()
{
    // OSG related stuff
    osg::TessellationHints * hints = new osg::TessellationHints;
    //TODO set detail ratio according to radius and toothnumber
    hints->setDetailRatio(1.0f);

    // Set up a StateSet to make the capped cylinder color
    osg::StateSet * gearState = new osg::StateSet();
    osg::Material * redMaterial = new osg::Material();
    osg::Vec4 color( 1.0f, 0.0f, 0.0f, 1.0f );
    redMaterial->setDiffuse( osg::Material::FRONT_AND_BACK, color );
    gearState->setAttribute( redMaterial );

    /*
    *composite shape object with two spheres and a cylinder
    *
    */
    osg::CompositeShape * compositeGear;
    compositeGear = new osg::CompositeShape();
    compositeGear->addChild(new osg::Cylinder(osg::Vec3(0, 0, 0), mRadius, GEARTHICKNESS));

    for (int i = 0; i < mToothNumber; i++)
    {
        osg::Quat quatr(mInitAngle-mAngle*i, osg::Vec3(0,0,1));

        osg::Box *box = new osg::Box(mBoxCenter, mBoxDim, mBoxDim, GEARTHICKNESS);
        box->setRotation(quatr);

        compositeGear->addChild(box);

        mBoxCenter = mBoxCenterOrientationMatrix * mBoxCenter;
    }

    osg:: ShapeDrawable * gearDraw;
    gearDraw = new osg::ShapeDrawable(compositeGear, hints);
    gearDraw->setStateSet( gearState );

    // Create a Geometry Node, and add the drawable to that node
    osg::Geode * geode = new osg::Geode();
    geode->addDrawable(gearDraw);

    osg::Transform * transNode = new osg::PositionAttitudeTransform();
    transNode->addChild(geode);

    SSimCallback * odeCallBack = new SSimCallback(mGeomID);
    transNode->setUpdateCallback( odeCallBack );

    return transNode;
}

void SGear::createEditorObject()
{
    init();
    
    /** Remove previously created child nodes of the transform node */
    while(mPosAttTransNode->getNumChildren() > 0)
    {
        mPosAttTransNode->removeChild(mPosAttTransNode->getChild(0));
    }

    /** Below we create the new child nodes and add them to the transform node */

    osg::TessellationHints * hints = new osg::TessellationHints;
    hints->setDetailRatio(0.5f);

    // Set up a StateSet to make the capped cylinder color
    osg::StateSet * gearState = new osg::StateSet();
    osg::Material * redMaterial = new osg::Material();
    osg::Vec4 color( 1.0f, 0.0f, 0.0f, 1.0f );
    redMaterial->setDiffuse( osg::Material::FRONT_AND_BACK, color );
    gearState->setAttribute( redMaterial );

    /** composite shape object with two spheres and a cylinder */
    osg::CompositeShape * compositeGear;
    compositeGear = new osg::CompositeShape();
    compositeGear->addChild(new osg::Cylinder(osg::Vec3(0, 0, 0), mRadius, GEARTHICKNESS));

    for (int i = 0; i < mToothNumber; i++)
    {
        osg::Quat quatr(mInitAngle-mAngle*i, osg::Vec3(0,0,1));

        osg::Box *box = new osg::Box(mBoxCenter, mBoxDim, mBoxDim, GEARTHICKNESS);
        box->setRotation(quatr);

        compositeGear->addChild(box);

        mBoxCenter = mBoxCenterOrientationMatrix * mBoxCenter;
    }

    osg:: ShapeDrawable * gearDraw;
    gearDraw = new osg::ShapeDrawable(compositeGear, hints);
    gearDraw->setStateSet( gearState );

    // Create a Geometry Node, and add the drawable to that node
    SOsgGeode * geode = new SOsgGeode();
    geode->addDrawable(gearDraw);
    geode->setSObject(this);

    /** Add the geom into the transform node of this object. */
    mPosAttTransNode->addChild(geode);

    SEditorCallback * editorCB = new SEditorCallback(this);
    mPosAttTransNode->setUpdateCallback(editorCB);
}

