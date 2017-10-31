#include "strimesh.h"

#include <osg/ShapeDrawable>

#include "sosggeode.h"
#include "ssimcallback.h"
#include "seditorcallback.h"

string STrimesh::mCounter = "1";

STrimesh::STrimesh(SObject::Types t) : SObject(t)
{
    mObjectType = SObject::TRIMESH;
    mVertexCount = 0;
    mIndexCount = 0;
    mVertices = 0;
    mIndices = 0;
    
    mName = "Trimesh" + mCounter;
    mCounter = incrementString(mCounter, mCounter.length()-1);
    
    setMass(1);
}

STrimesh::STrimesh(SObject::Types t, string fileName) : SObject(t)
{
    mObjectType = SObject::TRIMESH;
    mVertexCount = 0;
    mIndexCount = 0;
    mVertices = 0;
    mIndices = 0;
    mName = "Trimesh" + mCounter;
    mCounter = incrementString(mCounter, mCounter.length()-1);
    
    setMass(1);
    
    loadFile(fileName);
}

string STrimesh::getCounter()
{
    return mCounter;
}

void STrimesh::setCounter(string counter)
{
    mCounter = counter;
} 

STrimesh::~STrimesh()
{
    if(mIndices)
        delete [] mIndices;

    if(mVertices)
        delete [] mVertices;
}

void STrimesh::setMass(float m)
{
    dMassSetBoxTotal(&mMass, m, 1, 1, 1);
}

void STrimesh::setVertices(dVector3 * v, int count)
{
    mVertexCount = count;
    
    if(mVertices)
        delete [] mVertices;
    
    mVertices = v;
}

void STrimesh::setIndices(int *in, int count)
{
    mIndexCount = count;
    
    if(mIndices)
        delete [] mIndices;
    
    mIndices = in;
}

void STrimesh::loadFile(string name)
{
    osg::Node * node = osgDB::readNodeFile(name);

    osg::Group * mTriMeshNode = dynamic_cast<osg::Group *>(node);

    if(mTriMeshNode == 0L)
    {
        std::cout << "Cannot load trimesh file: " << name << std::endl;
        return;
    }

    osg::Geode * geode = (osg::Geode *)(mTriMeshNode->getChild(0));
    osg::Geometry * geom = (osg::Geometry *)(geode->getDrawable(0));

    osg::Array * vertexArr = geom->getVertexArray();
    osg::DrawElementsUShort * primSet = dynamic_cast<osg::DrawElementsUShort *>(geom->getPrimitiveSet(0));

    mIndexCount = primSet->getNumIndices();
    if(mIndices)
        delete [] mIndices;
    mIndices = new int[mIndexCount];

    for(int i=0; i < mIndexCount; i++)
    {
        mIndices[i] = primSet->index(i);
    }

    mVertexCount = vertexArr->getNumElements();
    float * vertices = (float *)vertexArr->getDataPointer();
    if(mVertices)
        delete [] mVertices;
    mVertices = new dVector3[mVertexCount];

    int j = 0;
    for(int i=0; i < mVertexCount; i++)
    {
        mVertices[i][0] = vertices[j++];
        mVertices[i][1] = vertices[j++];
        mVertices[i][2] = vertices[j++];
    }

    if(mTriMeshNode != 0)
    {
        mTriMeshNode = 0;
    }
}

void STrimesh::createSimObject(dWorldID world, dSpaceID space)
{

    // ODE related stuff
    if (this->isDynamic())
    {
        mBodyID = dBodyCreate(world);
        dBodySetMass(mBodyID, &mMass);

        // Create TriMesh in ODE
        dTriMeshDataID triMeshID;
        triMeshID = dGeomTriMeshDataCreate();
        dGeomTriMeshDataBuildSimple(triMeshID, (dReal*)mVertices, mVertexCount, (int*)mIndices, mIndexCount);
        mGeomID = dCreateTriMesh(space, triMeshID, 0, 0, 0);
        dGeomSetBody(mGeomID, mBodyID);
        dGeomSetPosition(mGeomID, mPosition.x(), mPosition.y(), mPosition.z());
    }
    else
    {
        mBodyID = 0;
        // Create TriMesh in ODE
        dTriMeshDataID triMeshID;
        triMeshID = dGeomTriMeshDataCreate();
        dGeomTriMeshDataBuildSimple(triMeshID, (dReal*)mVertices, mVertexCount, (int*)mIndices, mIndexCount);
        mGeomID = dCreateTriMesh(space, triMeshID, 0, 0, 0);
        dGeomSetBody(mGeomID, mBodyID);
        dGeomSetPosition(mGeomID, mPosition.x(), mPosition.y(), mPosition.z());
    }

    dQuaternion quat;
    quat[0] = mOrientation[3];
    quat[1] = mOrientation[0];
    quat[2] = mOrientation[1];
    quat[3] = mOrientation[2];
    dGeomSetQuaternion(mGeomID, quat);
}

osg::Node * STrimesh::createVisualObject()
{
    osg::PositionAttitudeTransform * transNode = new osg::PositionAttitudeTransform();

    osg::Geode * geode = new osg::Geode();
    osg::TriangleMesh * mesh = new osg::TriangleMesh();

    osg::Vec3Array * arr = new osg::Vec3Array();
    for(int i=0; i < mVertexCount; i++)
    {
        arr->push_back(osg::Vec3(mVertices[i][0], mVertices[i][1], mVertices[i][2]));
    }

    mesh->setVertices(arr);

    osg::UShortArray * ind = new UShortArray();
    for(int i=0; i < mIndexCount; i++)
    {
        ind->push_back(mIndices[i]);
    }

    mesh->setIndices(ind);

    osg:: ShapeDrawable * meshDraw;
    meshDraw = new osg::ShapeDrawable(mesh);

    geode->addDrawable(meshDraw);

    transNode->addChild(geode);

    osg::ref_ptr<SSimCallback> odeCallBack = new SSimCallback(mGeomID);
    transNode->setUpdateCallback( odeCallBack.get() );

    return transNode;
}

void STrimesh::createEditorObject()
{
    /** Remove previously created child nodes of the transform node */
    while(mPosAttTransNode->getNumChildren() > 0)
    {
        mPosAttTransNode->removeChild(mPosAttTransNode->getChild(0));
    }
        
    /** Below we create the new child nodes and add them to the transform node */

    SOsgGeode * geode = new SOsgGeode();
    geode->setSObject(this);
    
    osg::TriangleMesh * mesh = new osg::TriangleMesh();

    osg::Vec3Array * arr = new osg::Vec3Array();
    for(int i=0; i < mVertexCount; i++)
    {
        arr->push_back(osg::Vec3(mVertices[i][0], mVertices[i][1], mVertices[i][2]));
    }

    mesh->setVertices(arr);

    osg::UShortArray * ind = new UShortArray();
    for(int i=0; i < mIndexCount; i++)
    {
        ind->push_back(mIndices[i]);
    }

    mesh->setIndices(ind);

    osg:: ShapeDrawable * meshDraw;
    meshDraw = new osg::ShapeDrawable(mesh);

    geode->addDrawable(meshDraw);

    /** Add the geom into the transform node of this object. */
    mPosAttTransNode->addChild(geode);
    
    SEditorCallback * editorCB = new SEditorCallback(this);
    mPosAttTransNode->setUpdateCallback(editorCB);
}

