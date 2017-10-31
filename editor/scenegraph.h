#ifndef _SCENEGRAPH_H
#define _SCENEGRAPH_H 1

#include <osg/MatrixTransform>
#include <osg/PolygonMode>
#include <osg/ShadeModel>
#include <osg/LineStipple>
#include <osgText/Text>
#include <osg/Projection>

/** @class SceneGraph
 * @brief Editor scene graph, keeps visible data for the environment
 */
class SceneGraph: public osg::Referenced {
    
    /** This vector holds the roots visible on the Editor views. Each root shares the actual objects,
    * and has it's own gridNode and other view-related nodes. */
    std::vector<osg::ref_ptr<osg::Group> > mRoots;
    /** This is the group node for actual visual objects */
    osg::ref_ptr<osg::Group> mScene;
    /** A group node to store the utilities like handles for tools */
    osg::ref_ptr<osg::Group> mUtilities;
    /** A node displaying the current selection*/
    osg::ref_ptr<osg::MatrixTransform> selection;
    /** Decorator for the selected node showing it in wireframe */
    osg::ref_ptr<osg::Group> selection_decorator;
    osg::ref_ptr<osg::StateSet> mStateSet;
    osg::ref_ptr<osg::Node> mSelected;
    /** Path to the selected node */
    osg::NodePath selected;
    /** Transform until the parent of the selected node */
    osg::Matrix selected_parents_transform;
    
public:
    SceneGraph();
     /** Replaces the current scenegraph with another one*/
    void replaceScenegraph(osg::Node *newSG);

    void selectNode(osg::Node * node);
    osg::Matrix getParentTransform() { return selected_parents_transform; }

    void addChild(osg::Node *child) { mScene->addChild(child); }
    void removeChild(osg::Node *child) { mScene->removeChild(child); }
    void replaceChild(osg::Node *oldNode, osg::Node *newNode);

    osg::Group* getRealRoot(int i) { return mRoots[i].get(); }
    osg::Group * getUtilitiesGroup() { return mUtilities.get(); }
    
    osg::Node* getScene() { return mScene.get(); }

private:
};

#endif

