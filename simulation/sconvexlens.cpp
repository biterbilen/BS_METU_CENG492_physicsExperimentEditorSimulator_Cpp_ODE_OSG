#include "sconvexlens.h"

//TODO write methods for setting mScale

//TODO WRITE recalculate parameters

//FIXME mElevRange and mAzRange should be divided by 2 and stored like that as in SConvexMirror for efficiency
string SConvexLens::mCounter = "1";

SConvexLens::SConvexLens() : SObject(SObject::STATIC)
{    
    mObjectType = SObject::CONVEXLENS;
    mPosition.set(0,0,20);
    mFocalLength = 5;
    mDirection.set(1,0,0);
    mThickness = 0.2;
    mName = "ConvexLens" + mCounter;
    mCounter = incrementString(mCounter, mCounter.length()-1);   
}

SConvexLens::SConvexLens(osg::Vec3 pos, float foc, float thickness) : SObject(SObject::STATIC)
{
    mObjectType = SObject::CONVEXLENS;
    mPosition = pos;
    mFocalLength = foc;
    mDirection.set(1,0,0);
    mThickness = thickness;
    mName = "ConvexLens" + mCounter;
    mCounter = incrementString(mCounter, mCounter.length()-1);
}

void SConvexLens::init() 
{
    mRadius = 2 * mFocalLength;    
    mAzRange = M_PI/2;
    mElevRange = M_PI; 
    mDensity = 100;
    mCatBits = 1;    
    mColBits = 2;
    mLensRadius = sqrt( mRadius*mRadius-(mRadius-mThickness)*(mRadius-mThickness) );
    mScale = mThickness/mLensRadius;
    mLensCenter1 = mPosition;
    mLensCenter1.x() -= (mRadius - mThickness);    
    mLensCenter2 = mPosition;
    mLensCenter2.x() += (mRadius - mThickness);    
    mRefractionIndex = 4.0/3.0;            
}

dGeomID SConvexLens::getGeomID2()
{ 
    return mGeomID2; 
}

void SConvexLens::calculateAzElevMinMax()
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

string SConvexLens::getCounter()
{
    return mCounter;
}

void SConvexLens::setCounter(string counter)
{
    mCounter = counter;
}       
 
void SConvexLens::setFocalLength(float foc)
{
    mFocalLength = foc;
    mRadius = 2 * mFocalLength;    
}

void SConvexLens::setThickness(float thickness)
{
    mThickness = thickness;    
}

float SConvexLens::getThickness() const
{
    return mThickness;
}
    
float SConvexLens::getFocalLength() const
{
    return mFocalLength;
}

void SConvexLens::setRefractionIndex(float ri)
{
    mRefractionIndex = ri;       
}

float SConvexLens::getRefractionIndex() const
{
    return mRefractionIndex;
}

bool SConvexLens::isValidPoint(osg::Vec3 p)
{
    std::cout<< "collision point p "<< p << std::endl;
    return ((p-mLensCenter1).length() + (p-mLensCenter2).length() < 2 * mRadius);    
}

void SConvexLens::createSimObject(dWorldID world, dSpaceID space)
{
    init();
    calculateAzElevMinMax(); 
    
    mBodyID = 0;
    mGeomID = dCreateSphere(space, mRadius);    
    dGeomSetPosition(mGeomID, mLensCenter1.x(), mLensCenter1.y(), mLensCenter1.z());
    dGeomSetBody(mGeomID, mBodyID);      

    mGeomID2 = dCreateSphere(space, mRadius);    
    dGeomSetPosition(mGeomID2, mLensCenter2.x(), mLensCenter2.y(), mLensCenter2.z());
    dGeomSetBody(mGeomID2, mBodyID);
    
    dQuaternion quat;
    quat[0] = mOrientation[3];
    quat[1] = mOrientation[0];
    quat[2] = mOrientation[1];
    quat[3] = mOrientation[2];
    
    dGeomSetQuaternion(mGeomID, quat);
    dGeomSetQuaternion(mGeomID2, quat);
}

osg::Node * SConvexLens::createVisualObject()
{   
     // OSG related stuff
    osg::TessellationHints * hints = new osg::TessellationHints;
    hints->setDetailRatio(0.5f);

    osg::Geode * geode = new osg::Geode();
        
    osg::StateSet * state = new osg::StateSet();
    osg::Material * material = new osg::Material();
    osg::Vec4 white( 1.f, 1.0f, 1.0f, 1.0f );
    material->setDiffuse( osg::Material::FRONT_AND_BACK, white );
    material->setTransparency(osg::Material::FRONT, 0.4);
    state->setRenderBinDetails( 11, "DepthSortedBin");
    state->setRenderingHint( osg::StateSet::TRANSPARENT_BIN );
    state->setMode(GL_BLEND, osg::StateAttribute::ON);
    state->setAttribute( material );
    
    const float azIncr = (mAzMax - mAzMin)/mDensity;
    const float elevIncr = (mElevMax - mElevMin)/mDensity;
    
    for(int i=0; i+1<=mDensity; i++)
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
    
            normals->push_back(osg::Vec3(x*mScale, y, z));
            vertices->push_back(osg::Vec3(x*mScale, y, z)*mLensRadius);
            
            // QuadStrip Edge formed at az2
            // ----------------------------

            // Work out the sphere normal
            x = cos(elev)*sin(az2);
            y = cos(elev)*cos(az2);
            //   z = sin(elev);   z doesn't change
            
            normals->push_back(osg::Vec3(x*mScale, y, z));
            vertices->push_back(osg::Vec3(x*mScale, y, z)*mLensRadius);
        }
        
        geom->setVertexArray(vertices);
        geom->setNormalArray(normals);
        geom->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);
        geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUAD_STRIP, 0, vertices->size()));
        geom->setStateSet(state);
        geode->addDrawable(geom);
    }
        
    // drawable spheres at the centers of the lenses
    osg::ShapeDrawable *lens1;
    lens1 = new osg::ShapeDrawable(new osg::Sphere(mLensCenter1, 0.1),hints);
    osg::ShapeDrawable *lens2;
    lens2 = new osg::ShapeDrawable(new osg::Sphere(mLensCenter2, 0.1),hints);
    
    lens1->setStateSet(state);
    lens2->setStateSet(state);
    
    geode->addDrawable(lens1);
    geode->addDrawable(lens2);

    osg::PositionAttitudeTransform *transNode = new osg::PositionAttitudeTransform();
    transNode->addChild(geode);
    
    transNode->setPosition(mPosition);
    transNode->setAttitude(mOrientation);
    
    return transNode;
}

void SConvexLens::createEditorObject() 
{
    init();
    calculateAzElevMinMax(); 
    
    /** Remove previously created child nodes of the transform node */
    while(mPosAttTransNode->getNumChildren() > 0)
    {
        mPosAttTransNode->removeChild(mPosAttTransNode->getChild(0));
    }
    
    // OSG related stuff
    osg::TessellationHints * hints = new osg::TessellationHints;
    hints->setDetailRatio(0.5f);

    SOsgGeode * geode = new SOsgGeode();
    geode->setSObject(this);

    osg::StateSet * state = new osg::StateSet();
    osg::Material * material = new osg::Material();
    osg::Vec4 white( 1.f, 1.0f, 1.0f, 1.0f );
    material->setDiffuse( osg::Material::FRONT_AND_BACK, white );
    material->setTransparency(osg::Material::FRONT, 0.4);
    state->setRenderBinDetails( 11, "DepthSortedBin");
    state->setRenderingHint( osg::StateSet::TRANSPARENT_BIN );
    state->setMode(GL_BLEND, osg::StateAttribute::ON);
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
    
            normals->push_back(osg::Vec3(x*mScale, y, z));
            vertices->push_back(osg::Vec3(x*mScale, y, z)*mLensRadius);
            
            // QuadStrip Edge formed at az2
            // ----------------------------

            // Work out the sphere normal
            x = cos(elev)*sin(az2);
            y = cos(elev)*cos(az2);
            //   z = sin(elev);   z doesn't change
            
            normals->push_back(osg::Vec3(x*mScale, y, z));
            vertices->push_back(osg::Vec3(x*mScale, y, z)*mLensRadius);
        }
        
        geom->setVertexArray(vertices);
        geom->setNormalArray(normals);
        geom->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);
        geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUAD_STRIP, 0, vertices->size()));
        geom->setStateSet(state);
        geode->addDrawable(geom);
    }
        
    // drawable spheres at the centers of the lenses
    osg::ShapeDrawable *lens1;
    lens1 = new osg::ShapeDrawable(new osg::Sphere(mLensCenter1, 0.1),hints);
    osg::ShapeDrawable *lens2;
    lens2 = new osg::ShapeDrawable(new osg::Sphere(mLensCenter2, 0.1),hints);
    
    lens1->setStateSet(state);
    lens2->setStateSet(state);

    geode->addDrawable(lens1);
    geode->addDrawable(lens2);

    /** Add the geom into the transform node of this object. */
    mPosAttTransNode->addChild(geode);
    
    SEditorCallback * editorCB = new SEditorCallback(this);
    mPosAttTransNode->setUpdateCallback(editorCB);
}

