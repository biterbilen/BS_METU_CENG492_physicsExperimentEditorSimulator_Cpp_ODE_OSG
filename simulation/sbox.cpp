#include "sbox.h"
#include "sosggeode.h"
#include "seditorcallback.h"

string SBox::mCounter = "1";

SBox::SBox(SObject::Types t) : SObject(t)
{
    mObjectType = SObject::BOX;
    mDimensions.set(0,0,0);
    mPosition.set(0,0,0);
    mName = "Box" + mCounter;
    mCounter = incrementString(mCounter, mCounter.length()-1);
}

SBox::SBox(float len, osg::Vec3 pos) : SObject(SObject::DYNAMIC)
{
    mObjectType = SObject::BOX;
    mPosition = pos;
    mOrientation = osg::Quat(0, osg::Vec3(0, 0, 1));
    mDimensions = osg::Vec3(len, len, len);
    mName = "Box" + mCounter;
    mCounter = incrementString(mCounter, mCounter.length()-1);
    setMass(1);
}

SBox::SBox(osg::Vec3 len, osg::Vec3 pos) : SObject(SObject::DYNAMIC)
{
    mObjectType = SObject::BOX;
    mPosition = pos;
    mOrientation = osg::Quat(0, osg::Vec3(0, 0, 1));
    mDimensions = len;
    mName = "Box" + mCounter;
    mCounter = incrementString(mCounter, mCounter.length()-1);
    setMass(1);
}

string SBox::getCounter()
{
    return mCounter;
}

void SBox::setCounter(string counter)
{
    mCounter = counter;
}       
 
void SBox::setDimensions(osg::Vec3 dim)
{
    mDimensions = dim;    
}

osg::Vec3 SBox::getDimensions() const
{
    return mDimensions;
}

void SBox::setMass(float m)
{
    // Fill the mMass structure with corresponding mass information
    dMassSetBoxTotal(&mMass, m, mDimensions.x(), mDimensions.y(), mDimensions.z());    
}

float SBox::getVolume()
{
    return mDimensions[0] * mDimensions[1] * mDimensions[2];
}
    
float SBox::getDensity()
{
    return (this->getMass() / this->getVolume());
}

void SBox::createSimObject(dWorldID world, dSpaceID space)
{
    // ODE related stuff
    if (this->isDynamic()) 
    {        
        mBodyID = dBodyCreate(world);
        dBodySetPosition(mBodyID, mPosition.x(), mPosition.y(), mPosition.z());        
        mGeomID = dCreateBox(space, mDimensions.x(), mDimensions.y(), mDimensions.z());
        dBodySetMass(mBodyID, &mMass);        
        dGeomSetBody(mGeomID, mBodyID);
    }
    else 
    {
        mBodyID = 0;
        mGeomID = dCreateBox(space, mDimensions.x(), mDimensions.y(), mDimensions.z());
        dGeomSetPosition(mGeomID, mPosition.x(), mPosition.y(), mPosition.z());
        dGeomSetBody(mGeomID, mBodyID);      
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

void SBox::createEditorObject() 
{
    /** Remove previously created child nodes of the transform node */
    while(mPosAttTransNode->getNumChildren() > 0)
    {
        mPosAttTransNode->removeChild(mPosAttTransNode->getChild(0));
    }
        
    /** Below we create the new child nodes and add them to the transform node */
    
    // OSG related stuff
    osg::TessellationHints * hints = new osg::TessellationHints;
    hints->setDetailRatio(0.5f);

    // Set up a StateSet to make the cube red
    osg::StateSet * boxState = new osg::StateSet();
    osg::Material * redMaterial = new osg::Material();
    osg::Vec4 red( 0.2f, 1.0f, 0.2f, 1.0f );
    redMaterial->setDiffuse( osg::Material::FRONT_AND_BACK, red );
    boxState->setAttribute( redMaterial );

    // Create a drawable shape
    osg:: ShapeDrawable *boxDraw;
    boxDraw = new osg::ShapeDrawable(new osg::Box(osg::Vec3(0,0,0), mDimensions.x(), mDimensions.y(), mDimensions.z()), hints);
    boxDraw->setStateSet( boxState );
    
    // Create a Geometry Node, and add the drawable to that node
    SOsgGeode * geode = new SOsgGeode();
    geode->addDrawable(boxDraw);
    geode->setSObject(this);

    /** Add the geom into the transform node of this object. */
    mPosAttTransNode->addChild(geode);
    
    SEditorCallback * editorCB = new SEditorCallback(this);
    mPosAttTransNode->setUpdateCallback(editorCB);
}

osg::Node * SBox::createVisualObject()
{
    // OSG related stuff
    osg::ref_ptr<osg::TessellationHints> hints = new osg::TessellationHints;
    hints->setDetailRatio(0.5f);

    // Set up a StateSet to make the cube red
    osg::ref_ptr<osg::StateSet> boxState = new osg::StateSet();
    osg::ref_ptr<osg::Material> redMaterial = new osg::Material();
    osg::Vec4 red( 0.2f, 1.0f, 0.2f, 1.0f );
    redMaterial->setDiffuse( osg::Material::FRONT_AND_BACK, red );
    boxState->setAttribute( redMaterial.get() );

    /**
    * Create a drawable shape
    * Note that it is created in coordinates (0, 0, 0)... 
    * That's not so important because it will get it's position from ODE
    * with the help of the updateCallback we will set below...
    */
    osg::ref_ptr<osg:: ShapeDrawable> boxDraw;
    boxDraw = new osg::ShapeDrawable(new osg::Box(osg::Vec3(0,0,0), mDimensions.x(), mDimensions.y(), mDimensions.z()), hints.get());
    boxDraw->setStateSet( boxState.get() );

    // Create a Geometry Node, and add the drawable to that node
    osg::ref_ptr<osg::Geode> geode = new osg::Geode();
    geode->addDrawable(boxDraw.get());
    
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

void SBox::createBobbies()
{       
    vector<osg::Vec4> relative;
    float x = mDimensions.x();
    float y = mDimensions.y();
    float z = mDimensions.z();
    
    if(x < y && y < z)    
    {
        relative = calculatePositionRadius(x, y, z);
    }
    else if(x < z && z < y)
    {
        relative = calculatePositionRadius(x, z, y);
        relative = swap(relative, 1, 2);
    }
    else if(y < x && x < z)
    {
        relative = calculatePositionRadius(y, x, z);
        relative = swap(relative, 0, 1);
    }
    else if(y < z && z < x)
    {
        relative = calculatePositionRadius(y, z, x);
        relative = swap(relative, 0, 1);
        relative = swap(relative, 0, 2);
    }
    else if(z < x && x < y)
    {
        relative = calculatePositionRadius(z, x, y);
        relative = swap(relative, 0, 1);
        relative = swap(relative, 1, 2);
    }
    else if(z < y && y < x)
    {
        relative = calculatePositionRadius(z, y, x);
        relative = swap(relative, 0, 2);
    }
    else if(x == y && x != z) 
    {
        relative = calculatePositionRadius(x, y, z);
    }
    else if(y == z && y != x)
    {
        relative = calculatePositionRadius(y, z, x);
        relative = swap(relative, 0, 1);
        relative = swap(relative, 0, 2);
    }
    else if(z == x && z != y)
    {
        relative = calculatePositionRadius(x, z, y);
        relative = swap(relative, 1, 2);
    }
    else //if(x == y && y == z)
    {
        relative = calculatePositionRadius(x, y, z);
    }
    
    for(unsigned int i = 0 ; i < relative.size() ; i++)
    {        
        float radius = (relative[i]).w();
        SBobby* bobby = new SBobby();
        bobby->setBody(this);        
        bobby->setPosition(osg::Vec3((relative[i]).x(), (relative[i]).y(), (relative[i]).z()));
        bobby->setRadius(radius);                                
        bobby->setAttachedBodyID(mBodyID);
        mBobbies.push_back(bobby);       
    }    
}

vector<osg::Vec4> SBox::swap(vector<osg::Vec4>& v, int j, int k)
{
    float t;
    for(unsigned int i = 0 ; i < v.size() ; i++)
    {
        t = (v[i])[j];
        (v[i])[j] = (v[i])[k];
        (v[i])[k] = t;
    }
    
    return v;
}

bool SBox::isEven(int n)
{
    if(n % 2 == 0)
        return true;
    else 
        return false;    
}

vector<osg::Vec4> SBox::cubic(vector<osg::Vec4>& v, float x, float displacement)
{    
    float radius = pow(3*x*x*x/(32*M_PI),1.0/3.0);    
    x /= 4.0;
    
    v.push_back(osg::Vec4(-x, -x, -x + displacement, radius));
    v.push_back(osg::Vec4(-x, -x,  x + displacement, radius));
    v.push_back(osg::Vec4(-x,  x, -x + displacement, radius));
    v.push_back(osg::Vec4(-x,  x,  x + displacement, radius));
    v.push_back(osg::Vec4( x, -x, -x + displacement, radius));
    v.push_back(osg::Vec4( x, -x,  x + displacement, radius));
    v.push_back(osg::Vec4( x,  x, -x + displacement, radius));
    v.push_back(osg::Vec4( x,  x,  x + displacement, radius));
        
    return v;
}

vector<osg::Vec4> SBox::cubic(vector<osg::Vec4>& v, float x, float disY, float disZ)
{
    float radius = pow(3*x*x*x/(32*M_PI),1.0/3.0);    
    x /= 4.0;

    v.push_back(osg::Vec4(-x, -x + disY, -x + disZ, radius));
    v.push_back(osg::Vec4(-x, -x + disY,  x + disZ, radius));
    v.push_back(osg::Vec4(-x,  x + disY, -x + disZ, radius));
    v.push_back(osg::Vec4(-x,  x + disY,  x + disZ, radius));
    v.push_back(osg::Vec4( x, -x + disY, -x + disZ, radius));
    v.push_back(osg::Vec4( x, -x + disY,  x + disZ, radius));
    v.push_back(osg::Vec4( x,  x + disY, -x + disZ, radius));
    v.push_back(osg::Vec4( x,  x + disY,  x + disZ, radius));
 
    return v;   
}

vector<osg::Vec4> SBox::prism(vector<osg::Vec4>& v, float x, float y, float displacement)
{
    float radius = pow(3*x*x*y/(16*M_PI),1.0/3.0); 
    x /= 4.0;   
    
    v.push_back(osg::Vec4(-x, -x, displacement, radius));
    v.push_back(osg::Vec4(-x,  x, displacement, radius));
    v.push_back(osg::Vec4( x, -x, displacement, radius));
    v.push_back(osg::Vec4( x,  x, displacement, radius));    
    
    return v;
}

vector<osg::Vec4> SBox::calculatePositionRadius(float x, float y, float z)  // x <= y <= z is assumed
{    
    vector<osg::Vec4> result;
    int m = ((int) floorf(y/x));
    int n = ((int) floorf(z/x));     
    
    if(x==y && x==z)            
        result = cubic(result, x, 0);    
    else if(x==y)
    {  
        if(isEven(n))
        {
            float width = z - (x * n);
            if(width != 0) 
                result = prism(result, x, width, 0);                    
            n /= 2;                    
        }
        else
        {        
            float width = (z - (x * n)) / 2;
            if(width != 0)
            {       
                result = cubic(result, x, 0);
                result = prism(result, x, width, -(x + width) / 2);            
                result = prism(result, x, width,  (x + width) / 2);            
            }
        
            n -= 1;
            n /= 2;                   
        }
        for(int i = 0 ; i < n ; i++)
            {                        
                result = cubic(result, x, -z + x * (0.5 + i));            
                result = cubic(result, x,  z - x * (0.5 + i));            
            }    
    }
    else // if(x!=y && y!=z && z!=x)
    {
        if(isEven(m) && isEven(n))
        {
            float height = y - (x * m);            
            float depth = z - (x * n);
            float radius = pow(3*x*height*z/(4.0*n*M_PI), 1.0/3.0);
            float disZ =  - ((n - 1) * x / 2.0);            
            
            if(height != 0)
                for(int i = 0 ; i < n ; i++)            
                    result.push_back(osg::Vec4(0, 0, disZ + i * x, radius));               
                
            m /= 2; 
            
            radius = pow(3*depth*x*x/(4*M_PI) , 1.0/3.0);
            
            if(depth != 0)            
                for(int i = 0 ; i < m ; i++)
                {
                    result.push_back(osg::Vec4(0, -(1+i)*x, 0, radius));
                    result.push_back(osg::Vec4(0,  (1+i)*x, 0, radius));                           
                }
            
            n /= 2;
        }
        else if(isEven(m))
        {
            float height = y - (x * m);
            float depth = (z - (x * n)) / 2;            
            float radius = pow(3*x*height*z/(4.0*n*M_PI), 1.0/3.0);
            float disZ =  - ((n - 1) * x / 2.0);
                        
            if(height != 0)
                for(int i = 0 ; i < n ; i++)            
                    result.push_back(osg::Vec4(0, 0, disZ + i * x, radius));                        
                            
            m /= 2;     
            for(int i = 0 ; i < m ; i++)
                {
                    result = cubic(result, x, -y + x * (0.5 + i), 0);            
                    result = cubic(result, x,  y - x * (0.5 + i), 0);       
                }
                
            radius = pow(3*depth*x*x/(4*M_PI) , 1.0/3.0);

            if(depth != 0)                
                for(int i = 0 ; i < m ; i++)
                {
                    result.push_back(osg::Vec4(0, -(1+i)*x, 0, radius));
                    result.push_back(osg::Vec4(0,  (1+i)*x, 0, radius));        
                }           
            
            n -= 1;    
            n /= 2;  
        }
        else if(isEven(n))
        {
            float height = (y - (x * m)) / 2;
            float depth = z - (x * n);            
            float radius = pow(3*x*height*z/(4.0*n*M_PI), 1.0/3.0);
            float disZ =  - ((n - 1) * x / 2.0);            
            
            if(height != 0)
                for(int i = 0 ; i < n ; i++)            
                {
                    result.push_back(osg::Vec4(0, -x, disZ + i * x, radius));
                    result.push_back(osg::Vec4(0,  x, disZ + i * x, radius));
                }            
            
            n /= 2;
            for(int i = 0 ; i < n ; i++)
            {
                result = cubic(result, x, 0, -z + x * (0.5 + i));            
                result = cubic(result, x, 0,  z - x * (0.5 + i));       
            } 
                
            radius = pow(3*depth*x*x/(4*M_PI) , 1.0/3.0);
       
            m -= 1;    
            m /= 2;  
                     
            if(depth != 0)
            {
                result.push_back(osg::Vec4(0, 0, 0, radius));            
                for(int i = 0 ; i < m ; i++)
                {
                    result.push_back(osg::Vec4(0, -(1+i)*x, 0, radius));
                    result.push_back(osg::Vec4(0,  (1+i)*x, 0, radius)); 
                }
            }
        }
        else
        {
            float height = (y - (x * m)) / 2;
            float depth = (z - (x * n)) / 2;                
            float radius = pow(3*x*height*z/(4.0*n*M_PI), 1.0/3.0);
            float disZ =  - ((n - 1) * x / 2.0);                        
            
            if(height != 0)
                for(int i = 0 ; i < n ; i++)            
                {
                    result.push_back(osg::Vec4(0, -x, disZ + i * x, radius));
                    result.push_back(osg::Vec4(0,  x, disZ + i * x, radius));
                }     
            
            result = cubic(result, x, 0, 0); 
                        
            n -= 1;    
            n /= 2;                          
            for(int i = 0 ; i < n ; i++)
            {
                result = cubic(result, x, 0, -z + x * (0.5 + i));            
                result = cubic(result, x, 0,  z - x * (0.5 + i));       
            }          
            
            m -= 1;    
            m /= 2;              
            for(int i = 0 ; i < m ; i++)
            {
                result = cubic(result, x, -y + x * (0.5 + i), 0);            
                result = cubic(result, x,  y - x * (0.5 + i), 0);       
            }   
            
            radius = pow(3*depth*x*x/(4*M_PI) , 1.0/3.0);
            
            if(depth != 0) 
            {
                result.push_back(osg::Vec4(0, 0, -x/2.0, radius));            
                result.push_back(osg::Vec4(0, 0,  x/2.0, radius));            
                for(int i = 0 ; i < m ; i++)
                {
                    result.push_back(osg::Vec4(0, -(1+i)*x, 0, radius));
                    result.push_back(osg::Vec4(0,  (1+i)*x, 0, radius)); 
                }   
            }
        }
        for(int i = 0 ; i < m ; i++)
            for(int j = 0 ; j < n ; j++)
                {
                    result = cubic(result, x, -y + x * (0.5 + i), -z + x * (0.5 + j));            
                    result = cubic(result, x, -y + x * (0.5 + i),  z - x * (0.5 + j));            
                    result = cubic(result, x,  y - x * (0.5 + i), -z + x * (0.5 + j));            
                    result = cubic(result, x,  y - x * (0.5 + i),  z - x * (0.5 + j));
                }        
    }          
    return result;
}

ostream& operator<<(ostream& out, const SBox& box)
{
    out << "BOX" << endl;    
    out << "name " << box.mName <<endl<< "pos " << box.mPosition.x() << " " << box.mPosition.y() << " " << box.mPosition.z() << endl << "dim " << box.mDimensions.x() << " " << box.mDimensions.y() << " " << box.mDimensions.z() << endl << "mass " << box.mMass.mass << endl << "bobby " << box.mBobby << endl;
    return out;
}

istream& operator>>(istream& in, SBox& box)
{
    string dummy;
    in >> dummy ;
    in >> dummy >> box.mName ;
    in >> dummy >> box.mPosition.x() >>  box.mPosition.y() >>  box.mPosition.z() ;
    in >> dummy >> box.mDimensions.x() >> box.mDimensions.y() >>  box.mDimensions.z() ;
    in >> dummy >> box.mMass.mass ;
    in >> dummy >> box.mBobby ;
    return in;
}

