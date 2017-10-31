#include "sconmirror.h"

#include "seditorcallback.h"

//TODO set controls for az and elev range qualities one of them should be max 2*pi while the other is max pi
string SConMirror::mCounter = "1";

SConMirror::SConMirror() : SObject(SObject::STATIC)
{    
    mObjectType = SObject::CONMIRROR;
    mPosition.set(0,0,20);
    mFocalLength = 20;   
    mRadius = 2 * mFocalLength;
    mAzRange = M_PI*0.25;
    mElevRange = M_PI*0.25; 
    mDirection.set(1,0,0);
    mName = "ConMirror" + mCounter;
    mCounter = incrementString(mCounter, mCounter.length()-1);
    mDensity = 60;
    mCatBits = 1;    
    mColBits = 2;
}

SConMirror::SConMirror(osg::Vec3 pos, float foc) : SObject(SObject::STATIC)
{
    mObjectType = SObject::CONMIRROR;
    mPosition = pos;
    mFocalLength = foc;
    mRadius = 2 * mFocalLength;
    mAzRange = M_PI*0.25;
    mElevRange = M_PI*0.25;
    mDirection.set(1,0,0);
    mName = "ConMirror" + mCounter;
    mCounter = incrementString(mCounter, mCounter.length()-1);    
    mDensity = 60;
    mCatBits = 1;    
    mColBits = 2;
}

void SConMirror::init()
{
    mDirection = osg::Matrix(mOrientation) * osg::Vec3(1,0,0);    
}

string SConMirror::getCounter()
{
    return mCounter;
}

void SConMirror::setCounter(string counter)
{
    mCounter = counter;
}       
 
void SConMirror::setFocalLength(float foc)
{
    mFocalLength = foc;
    mRadius = 2 * mFocalLength;    
}

float SConMirror::getRadius() const
{
    return mRadius;
}    
    
float SConMirror::getFocalLength() const
{
    return mFocalLength;
}

void SConMirror::setAzRange(float az)
{
    mAzRange = az / 2.0;
}

float SConMirror::getAzRange() const
{
    return 2*mAzRange;   
}

void SConMirror::setElevRange(float elev)
{
    mElevRange = elev / 2.0;
}

float SConMirror::getElevRange() const
{
    return 2*mElevRange;
}

//TODO control the orientation of v vector and u vector
bool SConMirror::isValidPoint(osg::Vec3 p)
{    
    osg::Vec3 v = osg::Matrix(mOrientation.inverse()) * p;
    v = v - mPosition;
    
    osg::Vec3 u = osg::Matrix(mOrientation.inverse()) * mDirection;
//     cout<<u<<endl;        

    osg::Vec3 projv = v;
    projv.z() = 0;
    
    float angle = acos( (u*projv) / ( u.length() * projv.length() ) );    
    
    if (abs(angle) > mAzRange ) 
    {
        return false;
    }
    
    if(abs(p.z() - mPosition.z()) > sin(mElevRange) * mRadius)
        return false;
    
    return true;        
}

void SConMirror::createSimObject(dWorldID world, dSpaceID space)
{
    mBodyID = 0;
    mGeomID = dCreateSphere(space, mRadius);    
    dGeomSetPosition(mGeomID, mPosition.x(), mPosition.y(), mPosition.z());
    dGeomSetBody(mGeomID, mBodyID);      
    
    dQuaternion quat;
    quat[0] = mOrientation[3];
    quat[1] = mOrientation[0];
    quat[2] = mOrientation[1];
    quat[3] = mOrientation[2];
    dGeomSetQuaternion(mGeomID, quat);    
}

void SConMirror::calculateAzElevMinMax()
{
    osg::Vec3 vec(mDirection);

    vec.normalize();    // Make sure we have unit length

    // Calculate the elevation range
    float elev = asin(vec.z());   // Elevation angle
    mElevMin = elev - mElevRange;
    mElevMax = elev + mElevRange;

    // Calculate the azimuth range, cater for trig ambiguities
    float xyLen = cos(elev);
    float az;
    if(vec.x() != 0.0f) az = asin(vec.x()/xyLen);
    else az = acos(vec.y()/xyLen);

    mAzMin = az - mAzRange;
    mAzMax = az + mAzRange;
}

osg::Node * SConMirror::createVisualObject()
{
    calculateAzElevMinMax();
    init();
    
    osg::Geode * geode = new osg::Geode();
    
    osg::TessellationHints * hints = new osg::TessellationHints;
    hints->setDetailRatio(0.5f);
    
    osg::StateSet * state = new osg::StateSet();
    osg::Material * material = new osg::Material();
    osg::Vec4 white( 1.f, 1.0f, 1.0f, 1.0f );
    material->setDiffuse( osg::Material::FRONT_AND_BACK, white );
    state->setAttribute( material );
    
    const float azIncr = (mAzMax - mAzMin)/mDensity;
    const float elevIncr = (mElevMax - mElevMin)/mDensity;
    
    for(int i=0; i < mDensity; i++)
    {
        osg::Geometry *geom=new osg::Geometry();
        osg::Vec3Array *vertices=new osg::Vec3Array();
        osg::Vec3Array *normals=new osg::Vec3Array();
    
        // Because we're drawing quad strips, we need to work out
        // two azimuth values, to form each edge of the (z-vertical)
        // strips
        float az1 = mAzMin + (i*azIncr);
        float az2 = mAzMin + ((i+1)*azIncr);

        for (int j=0; j<=mDensity; j++)
        {
            float elev = mElevMin + (j*elevIncr);

            // QuadStrip Edge formed at az1
            // ----------------------------

            // Work out the sphere normal
            float x = cos(elev)*sin(az1);
            float y = cos(elev)*cos(az1);
            float z = sin(elev);
    
            normals->push_back(osg::Vec3(x, y, z));
            vertices->push_back(mPosition + osg::Vec3(x, y, z)*mRadius);
            
            // QuadStrip Edge formed at az2
            // ----------------------------
    
            // Work out the sphere normal
            x = cos(elev)*sin(az2);
            y = cos(elev)*cos(az2);
            //   z = sin(elev);   z doesn't change
            
            normals->push_back(osg::Vec3(x, y, z));
            vertices->push_back(mPosition + osg::Vec3(x, y, z)*mRadius);
        }
        
        geom->setVertexArray(vertices);
        geom->setNormalArray(normals);
        geom->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);
        geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUAD_STRIP, 0, vertices->size()));
        geom->setStateSet(state);      
         
        geode->addDrawable(geom);                
    }

    return geode;
}

void SConMirror::createEditorObject() 
{
    init();
    calculateAzElevMinMax();
    
    /** Remove previously created child nodes of the transform node */
    while(mPosAttTransNode->getNumChildren() > 0)
    {
        mPosAttTransNode->removeChild(mPosAttTransNode->getChild(0));
    }
    
    SOsgGeode * geode = new SOsgGeode();
    geode->setSObject(this);
    
    osg::TessellationHints * hints = new osg::TessellationHints;
    hints->setDetailRatio(0.5f);
    
    osg::StateSet * state = new osg::StateSet();
    osg::Material * material = new osg::Material();
    osg::Vec4 white( 1.f, 1.0f, 1.0f, 1.0f );
    material->setDiffuse( osg::Material::FRONT_AND_BACK, white );
    state->setAttribute( material );
    
    const float azIncr = (mAzMax - mAzMin)/mDensity;
    const float elevIncr = (mElevMax - mElevMin)/mDensity;
    
    for(int i=0; i < mDensity; i++)
    {
        osg::Geometry *geom=new osg::Geometry();
        osg::Vec3Array *vertices=new osg::Vec3Array();
        osg::Vec3Array *normals=new osg::Vec3Array();
    
        // Because we're drawing quad strips, we need to work out
        // two azimuth values, to form each edge of the (z-vertical)
        // strips
        float az1 = mAzMin + (i*azIncr);
        float az2 = mAzMin + ((i+1)*azIncr);

        for (int j=0; j<=mDensity; j++)
        {
            float elev = mElevMin + (j*elevIncr);

            // QuadStrip Edge formed at az1
            // ----------------------------

            // Work out the sphere normal
            float x = cos(elev)*sin(az1);
            float y = cos(elev)*cos(az1);
            float z = sin(elev);
    
            normals->push_back(osg::Vec3(x, y, z));
            vertices->push_back(osg::Vec3(x, y, z)*mRadius);
            
            // QuadStrip Edge formed at az2
            // ----------------------------
    
            // Work out the sphere normal
            x = cos(elev)*sin(az2);
            y = cos(elev)*cos(az2);
            //   z = sin(elev);   z doesn't change
            
            normals->push_back(osg::Vec3(x, y, z));
            vertices->push_back(osg::Vec3(x, y, z)*mRadius);
        }
        
        geom->setVertexArray(vertices);
        geom->setNormalArray(normals);
        geom->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);
        geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUAD_STRIP, 0, vertices->size()));
        geom->setStateSet(state);      
         
        geode->addDrawable(geom);                
    }

    // drawable spheres at the focal points of the mirrors
    osg::ShapeDrawable *focus;
    focus = new osg::ShapeDrawable(new osg::Sphere(osg::Vec3(mFocalLength,0,0), 0.1),hints);
      
    focus->setStateSet(state);    
    geode->addDrawable(focus);   

    
    /** Add the geom into the transform node of this object. */
    mPosAttTransNode->addChild(geode);
    
    SEditorCallback * editorCB = new SEditorCallback(this);
    mPosAttTransNode->setUpdateCallback(editorCB);
}

