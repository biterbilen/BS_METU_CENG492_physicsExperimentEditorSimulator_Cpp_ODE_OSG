#ifndef S_TRIMESH_H
#define S_TRIMESH_H 1

#include <vector>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgUtil/Optimizer>
#include <osg/PositionAttitudeTransform>
#include "sobject.h"


class STrimesh : public SObject
{
public:
    STrimesh(SObject::Types t);
    STrimesh(SObject::Types t,  string fileName);

    void setMass(float m);
    
    /** Loads the triMesh file */
    void loadFile(string fName);
    
    string getCounter();
    void setCounter(string);
    
    int getVertexCount() { return mVertexCount; }
    int getIndexCount() { return mIndexCount; }
    
    void setVertices(dVector3 * v, int count);
    void setIndices(int* in, int count);
    
    dVector3 * getVertices() const { return mVertices; }
    int * getIndices() const { return mIndices; }
    
    /** Create the dynamics object in ODE */
    void createSimObject(dWorldID world, dSpaceID space);
    /** Create the visual object for simulation */
    osg::Node * createVisualObject();
    /** Create the visual object for the editor */
    void createEditorObject();

protected:
    ~STrimesh();

    int mVertexCount;
    int mIndexCount;
    dVector3 * mVertices;
    int * mIndices;
    
    static string mCounter;
};

#endif

