#include <osg/Geometry>
#include <osg/Geode>
#include <osg/Material>
#include <osg/PolygonOffset>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osg/Transform>

#include "scenegraph.h"
#include "clipboard.h"
// #include "appconfig.h"

#include <comdelete.h>
#include <comaddchild.h>
#include <comaddtransform.h>
#include "editor.h"

osg::Group *makeSelection(osg::LineStipple *stipple)
{
    osg::Group* decorator = new osg::Group;
    // set up the state so that the underlying color is not seen through
    // and that the drawing mode is changed to wireframe, and a polygon offset
    // is added to ensure that we see the wireframe itself, and turn off
    // so texturing too.
    osg::StateSet* stateset = new osg::StateSet;
    osg::Material* material = new osg::Material;
    osg::PolygonOffset* polyoffset = new osg::PolygonOffset;
    polyoffset->setFactor(-1.0f);
    polyoffset->setUnits(-1.0f);
    osg::PolygonMode* polymode = new osg::PolygonMode;
    polymode->setMode(osg::PolygonMode::FRONT_AND_BACK,osg::PolygonMode::LINE);
    stateset->setAttributeAndModes(material,osg::StateAttribute::OVERRIDE|osg::StateAttribute::ON);
    stateset->setAttributeAndModes(polyoffset,osg::StateAttribute::OVERRIDE|osg::StateAttribute::ON);
    stateset->setAttributeAndModes(polymode,osg::StateAttribute::OVERRIDE|osg::StateAttribute::ON);
    stateset->setMode(GL_LIGHTING,osg::StateAttribute::OVERRIDE|osg::StateAttribute::OFF);
    stateset->setTextureMode(0,GL_TEXTURE_2D,osg::StateAttribute::OVERRIDE|osg::StateAttribute::OFF);

    decorator->setStateSet(stateset);

    return decorator;
}

SceneGraph::SceneGraph()
{
    mRoots.resize(4);
    mRoots[0] =  new osg::Group();
    mRoots[1] =  new osg::Group();
    mRoots[2] =  new osg::Group();
    mRoots[3] =  new osg::Group();
    
    mScene= new osg::Group();
    
    osg::LineStipple *stipple=new osg::LineStipple();
    selection=new osg::MatrixTransform();
    selection_decorator=makeSelection(stipple);
    selection->addChild(selection_decorator.get());
    mRoots[0]->addChild(selection.get());
    mRoots[1]->addChild(selection.get());
    mRoots[2]->addChild(selection.get());
    mRoots[3]->addChild(selection.get());
    
    mUtilities = new osg::Group();
    mRoots[0]->addChild(mUtilities.get());
    mRoots[1]->addChild(mUtilities.get());
    mRoots[2]->addChild(mUtilities.get());
    mRoots[3]->addChild(mUtilities.get());
    
    osg::Matrix m;
    m.makeScale(0,0,0);
    selection->setMatrix(m);
    
    mSelected = 0;
}

void SceneGraph::replaceScenegraph(osg::Node *newSG)
{
    if (mScene.valid())
    {
        mRoots[0]->removeChild(mScene.get());
        mRoots[1]->removeChild(mScene.get());
        mRoots[2]->removeChild(mScene.get());
        mRoots[3]->removeChild(mScene.get());
    }
    
    mScene = new osg::Group();
    mScene->addChild(newSG);
    
    mRoots[0]->addChild(mScene.get());
    mRoots[1]->addChild(mScene.get());
    mRoots[2]->addChild(mScene.get());
    mRoots[3]->addChild(mScene.get());

    History::clear();
}

void SceneGraph::replaceChild(osg::Node *oldNode, osg::Node *newNode)
{
    mScene->replaceChild(oldNode,newNode);
}

void SceneGraph::selectNode(osg::Node * node)
{
    if(mSelected.valid())
        mSelected->setStateSet(mStateSet.get());

    mSelected = node;
    mStateSet = node->getStateSet();
    node->setStateSet(selection_decorator->getStateSet());
}

