#ifndef __EDITOR_H__
#define __EDITOR_H__

#include <string>
#include <map>
#include <osg/Referenced>

class Tool;

#include "history.h"
#include "cameraview.h"
#include "scenegraph.h"
#include "senvironment.h"
#include "sguitreeview.h"
#include "command.h"
#include "sfileinputoutput.h"

/** @class Editor
 * @brief Representation of the generic editor controller
 *
 * This class must be subclassed on each concrete editor. This is part of the strategy followed
 * allow multiple views (Model-View-Controller pattern).
 */
class Editor: public osg::Referenced, public HistoryObserver {
    /* Private data, don't worry about this */
    osg::ref_ptr<Tool> activeTool; ///< The currently active tool
    
    typedef std::vector<osg::ref_ptr<CameraView> > CameraViewList;
    CameraViewList viewList; ///< The view of the editor
    
    osg::ref_ptr<osg::Node> selectedNode; ///< The currently selected node
    osg::ref_ptr<osg::Group> selectedParent; ///< The parent of the currently selected node

protected:
    /** protected constructor to ensure nobody instantiates this class */
    Editor();

    /** Scenegraph of the visually seen objects on the editor */
    osg::ref_ptr<SceneGraph> sceneGraph;
    
    /** This stores all the information about the environment */
    osg::ref_ptr<SEnvironment> mEnv;

    SGuiTreeView * mTreeView;
    std::string mLoadedFileName; ///< The name of the loaded file, to know where to save

    /// Here you will receive events when the user selects one node to highlight
    virtual void selected(osg::Node *node)=0;
public:
    /// Your interface must call this method in order to activate a tool
    void useTool(char *toolname);

    /// Call here if you want to select a node (for tools that operate on nodes)
    void selectNode(osg::Node *node, osg::NodePath &path);
    
    /// Returns the currently selected node
    osg::Node *getSelected() { return selectedNode.get(); }

    /// Returns the camera view representing the point of view
    CameraView *getCameraView(int i) { return viewList[i].get(); }

    /** Returns the editor scenegraph */
    SceneGraph *getSceneGraph() { return sceneGraph.get(); }

    /** Returns the simulation environment */
    SEnvironment *getEnvironment() { return mEnv.get(); }
    
    /** Sets the simulation environment */
    void setEnvironment(SEnvironment * env);
    
    SGuiTreeView *getTreeView() { return mTreeView; }

    /**
     * Here you will receive events when some command is executed. It is
     * not needed to implement this method, but is usefull to update the
     * interface as this command could have changed the scenegraph or
     * the attributes of some node */
    virtual void executed(Command *) {}

    /// Called when some command is un-executed, see executed for details
    virtual void unexecuted(Command *) {}

    /**
     * As the Editor is osg::Referenced, you can have problems when
     * terminating the application. Use this method to solve this problems
     * for example freeing timers */
    virtual void finalize() {}

    /** Load environment file */
    void loadFile(const char *  fileName);
    
    /** Save loaded file */
    void saveFile();
    
    /** Save File with name fileName*/
    void saveFileAs(const char * fileName);

    /** Gets the name of the loaded file */
    std::string getLoadedFile() { return mLoadedFileName.c_str(); }

    /**
     * Gets the path to the loaded file. Needed for example, to figure out
     * where the textures are placed in the filesystem and show them in
     * your toolkit's image widget */
    std::string getLoadedFilePath();
};

#endif

