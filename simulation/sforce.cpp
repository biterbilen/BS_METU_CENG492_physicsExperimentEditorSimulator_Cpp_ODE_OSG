#include "sforce.h"

#include <osg/ShapeDrawable>
#include <osg/Material>

#include "seditorforcecallback.h"

SForce::SForce(osg::Vec3 f)
{
    mForce = f;
    mStart = 0;
    mDuration = -1;
    mPosAttTransNode = new osg::PositionAttitudeTransform();
}

SForce::SForce()
{
    mForce = osg::Vec3(0, 0, 0);    
    mStart = 0;
    mDuration = -1;
    mPosAttTransNode = new osg::PositionAttitudeTransform();
}

void SForce::setName(string s)
{
    mName = s;    
}

string SForce::getName() const
{
    return mName;
}

SForce::ForceTypes SForce::getType() const
{
    return mType;
} 

void SForce::setForce(osg::Vec3 f)
{
    mForce = f;
}

osg::Vec3 SForce::getForce() const
{
    return mForce;
}

void SForce::setObject(SObject* obj)
{
    mObject = obj;
}

SObject* SForce::getObject()
{
    return mObject.get();
}

float SForce::getDuration() const
{
    return mDuration;
}

void SForce::setDuration(float d)
{
    mDuration = d;    
}

float SForce::getStart() const
{
    return mStart;
}

void SForce::setStart(float s)
{
    mStart = s;
}

void SForce::setBodyIndex(int i)
{
    mBodyIndex = i;
}

int SForce::getBodyIndex() const
{
    return mBodyIndex;
}

void SForce::createEditorForce()
{
    /** Remove previously created child nodes of the transform node */
    while(mPosAttTransNode->getNumChildren() > 0)
    {
        mPosAttTransNode->removeChild(mPosAttTransNode->getChild(0));
    }

    osg::StateSet * state = new osg::StateSet();
    osg::Material * material = new osg::Material();
    osg::Vec4 coneColor( 0.2f, 1.0f, 0.2f, 1.0f );
    material->setDiffuse( osg::Material::FRONT_AND_BACK, coneColor );
    state->setAttribute( material );

    osg::ShapeDrawable* drawable;
    osg::Cone * cone = new osg::Cone(mForce, 0.1, 0.4); 
    
    osg::Matrix m;
    m.makeRotate(osg::Vec3(0, 0, 1), mForce);
    osg::Quat quat;
    quat.set(m);
    cone->setRotation(quat);
    
    drawable = new osg::ShapeDrawable(cone);
    drawable->setStateSet( state );
    
    osg::StateSet * state2 = new osg::StateSet();
    osg::Material * material2 = new osg::Material();
    osg::Vec4 color2( 0.5f, 0.5f, 0.5f, 1.0f );
    material2->setDiffuse( osg::Material::FRONT_AND_BACK, color2 );
    state2->setAttribute( material2 );

    osg::Geometry *geom=new osg::Geometry();
    osg::Vec3Array *vertices=new osg::Vec3Array();

    vertices->push_back(osg::Vec3(0,0,0));
    vertices->push_back(mForce);
    geom->setVertexArray(vertices);
    geom->setStateSet(state2);
        
    osg::Vec4Array *color = new osg::Vec4Array();
    color->push_back(osg::Vec4( 1.0f, 1.0f, 0.0f, 1.0f ));
    geom->setColorArray(color);
    geom->setColorBinding(osg::Geometry::BIND_PER_PRIMITIVE);

    geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, 2));
    
    osg::Geode * geode = new osg::Geode();
    geode->addDrawable(geom);
    geode->addDrawable(drawable);
    
    mPosAttTransNode->addChild(geode);

    SEditorForceCallback * editorCFB = new SEditorForceCallback(this);
    mPosAttTransNode->setUpdateCallback(editorCFB);
}

string SForce::incrementString(string s, int n)
{
    if(s[n] == '9')
    {
        s[n] = '0';
        if(n == 0)
            s = "1" + s;
        else
            s = incrementString(s, n-1);    
    }
    else
    {
        switch(s[n])
            {
                case '0' : s[n] = '1'; break;
                case '1' : s[n] = '2'; break;
                case '2' : s[n] = '3'; break;
                case '3' : s[n] = '4'; break;
                case '4' : s[n] = '5'; break;
                case '5' : s[n] = '6'; break;
                case '6' : s[n] = '7'; break;
                case '7' : s[n] = '8'; break;
                default  : s[n] = '9'; break;            
            }    
    }    
    return s;
}
