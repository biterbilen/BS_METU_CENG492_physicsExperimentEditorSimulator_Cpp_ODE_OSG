#include "scappedcylinder.h"
#include "sosggeode.h"
#include "seditorcallback.h"

string SCappedCylinder::mCounter = "1";

SCappedCylinder::SCappedCylinder(SObject::Types t) : SObject(t)
{
    mObjectType = SObject::CAPPEDCYLINDER;
    mName = "CappedCylinder" + mCounter;
    mCounter = incrementString(mCounter, mCounter.length()-1);
}

SCappedCylinder::SCappedCylinder(float radius, float height, osg::Vec3 pos) : SObject(SObject::DYNAMIC)
{
    mObjectType = SObject::CAPPEDCYLINDER;
    mPosition = pos;
    mOrientation = osg::Quat(0, osg::Vec3(0, 0, 1));
    mRadius = radius;
    mHeight = height;

    setMass(1);
    
    mName = "CappedCylinder" + mCounter;
    mCounter = incrementString(mCounter, mCounter.length()-1);
}

string SCappedCylinder::getCounter()
{
    return mCounter;
}

void SCappedCylinder::setCounter(string counter)
{
    mCounter = counter;
}

float SCappedCylinder::getRadius() const
{
    return mRadius;
}

void SCappedCylinder::setRadius(float r)
{
    mRadius = r;
    
}

void SCappedCylinder::setHeight(float h)
{
    mHeight = h;
    
}

float SCappedCylinder::getHeight() const
{
    return mHeight;
}

void SCappedCylinder::setMass(float m)
{
    // Fill the mMass structure with corresponding mass information
    
//void dMassSetCappedCylinderTotal (dMass *, dReal total_mass, int direction,
//                         dReal radius, dReal length);  
    // FIXME DIRECTION SHOULD BE REGULATED ACCORDING TO THE ORIENTATION BUT HOW? direction (1=x, 2=y, 3=z). ???????????
               
    // FIXME mass should be updated when the radius or height is changed
    dMassSetCappedCylinderTotal(&mMass, m, 3, mRadius, mHeight);
    
}

float SCappedCylinder::getVolume()
{
    return (M_PI * mRadius * mRadius ) * (mRadius + mHeight);
}
    
float SCappedCylinder::getDensity()
{
    return (this->getMass() / this->getVolume());
}

void SCappedCylinder::createSimObject(dWorldID worldid, dSpaceID spaceid)
{
    // ODE related stuff
    if (this->isDynamic()) 
    {
        mBodyID = dBodyCreate(worldid);
        dBodySetPosition(mBodyID, mPosition.x(), mPosition.y(), mPosition.z());
        dBodySetMass(mBodyID, &mMass);

        mGeomID = dCreateCCylinder(spaceid, mRadius, mHeight);
        dGeomSetBody(mGeomID, mBodyID);
    }
    else 
    {
        mBodyID = 0;
        mGeomID = dCreateCCylinder(spaceid, mRadius, mHeight);
        dGeomSetPosition(mGeomID, mPosition.x(), mPosition.y(), mPosition.z());
        dGeomSetBody(mGeomID, 0);      
    }
      
    dQuaternion quat;
    quat[0] = mOrientation[3];
    quat[1] = mOrientation[0];
    quat[2] = mOrientation[1];
    quat[3] = mOrientation[2];
    dGeomSetQuaternion(mGeomID, quat);
    
    if(mBobby)
        createBobbies();
}

void SCappedCylinder::createEditorObject() 
{
    init();
    
    /** Remove previously created child nodes of the transform node */
    while(mPosAttTransNode->getNumChildren() > 0)
    {
        mPosAttTransNode->removeChild(mPosAttTransNode->getChild(0));
    }
        
    /** Below we create the new child nodes and add them to the transform node */
    
    // OSG related stuff
    osg::TessellationHints * hints = new osg::TessellationHints;
    hints->setDetailRatio(0.5f);

    // Set up a StateSet to make the capped cylinder colro
    osg::StateSet * cappedCylinderState = new osg::StateSet();
    osg::Material * redMaterial = new osg::Material();
    osg::Vec4 color( 0.5f, 0.5f, 1.0f, 1.0f );
    redMaterial->setDiffuse( osg::Material::FRONT_AND_BACK, color );
    cappedCylinderState->setAttribute( redMaterial );

    /** composite shape object with two spheres and a cylinder */ 
    osg::CompositeShape * compositeCappedCylinder;
    compositeCappedCylinder = new osg::CompositeShape();
    compositeCappedCylinder->addChild(new osg::Cylinder(osg::Vec3(0, 0, 0), mRadius, mHeight));
    compositeCappedCylinder->addChild(new osg::Sphere(osg::Vec3(0, 0, -mHeight/2 ), mRadius));
    compositeCappedCylinder->addChild(new osg::Sphere(osg::Vec3(0, 0, mHeight/2), mRadius));
    
    osg::ShapeDrawable * cappedCylinderDraw;
    cappedCylinderDraw = new osg::ShapeDrawable(compositeCappedCylinder, hints);
    cappedCylinderDraw->setStateSet( cappedCylinderState );
    
    // Create a Geometry Node, and add the drawable to that node
    SOsgGeode * geode = new SOsgGeode();
    geode->addDrawable(cappedCylinderDraw);
    geode->setSObject(this);

    /** Add the geom into the transform node of this object. */
    mPosAttTransNode->addChild(geode);
    
    SEditorCallback * editorCB = new SEditorCallback(this);
    mPosAttTransNode->setUpdateCallback(editorCB);
}

osg::Node * SCappedCylinder::createVisualObject()
{
    // OSG related stuff
    osg::ref_ptr<osg::TessellationHints> hints = new osg::TessellationHints;
    hints->setDetailRatio(0.5f);

    // Set up a StateSet to make the CCylinder blue
    osg::ref_ptr<osg::StateSet> cappedCylinderState = new osg::StateSet();
    osg::ref_ptr<osg::Material> blueMaterial = new osg::Material();
    osg::Vec4 blue( 0.5f, 0.5f, 1.0f, 1.0f );
    blueMaterial->setDiffuse( osg::Material::FRONT_AND_BACK, blue );
    cappedCylinderState->setAttribute( blueMaterial.get() );
    
    /** composite shape object with two spheres and a cylinder */ 
    osg::ref_ptr<osg::CompositeShape> compositeCappedCylinder;
    compositeCappedCylinder = new osg::CompositeShape();
    compositeCappedCylinder->addChild(new osg::Cylinder(osg::Vec3(0, 0, 0), mRadius, mHeight));
    compositeCappedCylinder->addChild(new osg::Sphere(osg::Vec3(0, 0, -mHeight/2.0 ), mRadius));
    compositeCappedCylinder->addChild(new osg::Sphere(osg::Vec3(0, 0, mHeight/2.0), mRadius));
    
    /** Create a drawable shape with the above composite object */
    osg::ref_ptr<osg:: ShapeDrawable> cappedCylinderDraw;
    cappedCylinderDraw = new osg::ShapeDrawable(compositeCappedCylinder.get(), hints.get());
    cappedCylinderDraw->setStateSet( cappedCylinderState.get() );
    
    // Create a Geometry Group, and add the drawable to that node
    osg::ref_ptr<osg::Geode> geode = new osg::Geode();
    geode->addDrawable(cappedCylinderDraw.get());
   
    /** 
    * Create a Transform Node, so that we can set up a NodeCallback which will
    * transform and rotate the child node in each drawing of the scene
    */
    osg::ref_ptr<osg::Transform> transNode = new osg::PositionAttitudeTransform();
    transNode->addChild(geode.get());

    osg::ref_ptr<SSimCallback> odeCallBack = new SSimCallback(mGeomID);
    transNode->setUpdateCallback( odeCallBack.get() );
   
    return transNode.take();
}

bool SCappedCylinder::isEven(int n)
{
    if(n % 2 == 0)
        return true;
    else 
        return false;    
}

vector<osg::Vec4> SCappedCylinder::calculatePositionRadius(float r, float h)
{
    vector<osg::Vec4> result;
    
    int n = 1 + (int) floorf((3.0 * h) / (4.0 * r));    
    float diff = h - (n - 1) * 4.0 * r /3.0; 
    
//     cout<<n<<" "<<diff<<endl;
    
    if(isEven(n))
    {        
        if(diff != 0)
        {
            float radius = pow(3*r*r*diff/4.0, 1.0/3.0);
            result.push_back(osg::Vec4(0, 0, 0, radius));    
        }        
        n /= 2;        
    }
    else // n is odd
    {    
        diff /= 2.0;
        if(diff != 0)
        {
            float radius = pow(3*r*r*diff/4.0, 1.0/3.0);
            result.push_back(osg::Vec4(0, 0, -r - diff/2.0, radius));    
            result.push_back(osg::Vec4(0, 0,  r + diff/2.0, radius));    
        }        
        result.push_back(osg::Vec4(0, 0, 0, r));
        n -= 1;
        n /= 2;    
    }
    for(int i = 0 ; i < n ; i++)
    {
        result.push_back(osg::Vec4(0, 0, -h/2.0 + i * 4.0 * r / 3.0, r));
        result.push_back(osg::Vec4(0, 0,  h/2.0 - i * 4.0 * r / 3.0, r));
    }
    
    return result;
}

void SCappedCylinder::createBobbies()
{
    vector<osg::Vec4> relative = calculatePositionRadius(mRadius, mHeight);
    float v = 0;
    for(unsigned int i = 0 ; i <  relative.size() ; i++)
    {
        float radius = (relative[i]).w();
        SBobby* bobby = new SBobby();
        bobby->setBody(this);        
        bobby->setPosition(osg::Vec3((relative[i]).x(), (relative[i]).y(), (relative[i]).z()));
        bobby->setRadius(radius);                                
        bobby->setAttachedBodyID(mBodyID);
        mBobbies.push_back(bobby);     
        
        v += bobby->getTotalVolume();
    }
}
