#include "editor.h"
#include "tool.h"
#include "toolbox.h"
#include "toolviewfactory.h"
#include "seditorcallback.h"

Editor::Editor()
{
    activeTool=0;

    viewList.resize(4);

    viewList[0]=new CameraView();
    viewList[1]=new CameraView();
    viewList[2]=new CameraView();
    viewList[3]=new CameraView();

    sceneGraph=new SceneGraph();
    selectedParent=new osg::Group();
    selectedNode=selectedParent.get();

    sceneGraph->replaceScenegraph(selectedNode.get());
    
    mEnv = new SEnvironment();
    
    mLoadedFileName = "";

    History::addObserver(this);
}

void Editor::setEnvironment(SEnvironment * env)
{
    mEnv = env;
    
    selectedParent=new osg::Group();
    selectedNode=selectedParent.get();
    sceneGraph->replaceScenegraph(selectedNode.get());

    mTreeView->setEnvironment(mEnv.get());
    mTreeView->clearObjects();
    for(list<SObject*>::iterator i = mEnv->getObjectBegin() ; i != mEnv->getObjectEnd() ; i++)
    {
        (*i)->createEditorObject();
        sceneGraph->addChild( (*i)->getTransformNode() );
        mTreeView->addObject( (*i) );
    }
    
     for(list<SJoint*>::iterator i = mEnv->getJointBegin() ; i != mEnv->getJointEnd() ; i++)
    {
        (*i)->createEditorJoint();
        sceneGraph->addChild( (*i)->getTransformNode() );
    }
    
     for(list<SForce*>::iterator i = mEnv->getForceBegin() ; i != mEnv->getForceEnd() ; i++)
    {
        (*i)->createEditorForce();
        sceneGraph->addChild( (*i)->getTransformNode() );
    }
}

void Editor::useTool(char *toolname)
{
    Tool *tool = ToolBox::instance()->getTool(toolname);
    if (tool)
    {
         if (activeTool.valid())
             activeTool->done(); // Notify that old tool is no longer needed

        activeTool=tool;
        activeTool->setEditor(this);
        ToolView *tv=ToolViewFactory::instance()->getView(tool);

        if (tv!=0)
        {
            tv->setEditor(this);
            tv->setActiveTool(tool);
            tv->enable();
        }
        try
        {
            activeTool->init();
            if (selectedNode.valid())
            {
                activeTool->selected(selectedNode.get());
            }
        }
        catch (const char *s)
        {
            activeTool->done();            
            //my_warning(s);
            std::cout << s << std::endl;
            activeTool=0;
        }
    }
    else
    {
        std::cerr << "No such tool: " << toolname << std::endl;
        std::cout << "FATAL ERROR: called an unknown tool" << std::endl;
    }
}

void Editor::selectNode(osg::Node *node, osg::NodePath &path)
{
    this->selectedNode=node;

    if (path.size() > 1)
    {
        osg::Node *node = path[path.size()-2];
        selectedParent = dynamic_cast<osg::Group*>(node);
    }
    else
    {
        selectedParent = 0;
    }

    selected(node);
    sceneGraph->selectNode(node);
}

void Editor::loadFile(const char *  fileName)
{
    SEnvironment * env = new SEnvironment();
    SFileInputOutput * sfin = new SFileInputOutput();
    
    env = sfin->readFile(fileName, env);
    mLoadedFileName = fileName;
        
    setEnvironment(env);
    
    if(sfin)
        delete sfin;
}

void Editor::saveFile()
{
    SFileInputOutput * sfout = new SFileInputOutput();
    
    sfout->writeFile(mLoadedFileName.c_str(), mEnv.get());
    
    if(sfout)
        delete sfout;
}

void Editor::saveFileAs(const char * fileName)
{
    SFileInputOutput * sfout = new SFileInputOutput();
    
    sfout->writeFile(fileName, mEnv.get());
    mLoadedFileName = fileName;
    
    if(sfout)
        delete sfout;    
}


std::string Editor::getLoadedFilePath()
{
    unsigned pos = mLoadedFileName.rfind('/');
    if (pos != std::string::npos)
    { // There is a slash
        return std::string(mLoadedFileName,0,pos);
    }
    else
    {
        return ".";
    }
}


