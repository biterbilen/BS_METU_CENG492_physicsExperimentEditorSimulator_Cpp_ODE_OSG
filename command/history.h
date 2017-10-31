#ifndef __HISTORY_H__
#define __HISTORY_H__

class Command;
#include <vector>
#include <osg/ref_ptr>

class HistoryObserver {
public:
    virtual void executed(Command *c)=0;
    virtual void unexecuted(Command *c)=0;
};

class History {
    typedef std::vector<osg::ref_ptr<Command> > CommandList;
    typedef std::vector<HistoryObserver*> ObserverList;
    static CommandList commands;
    static unsigned position;
    static ObserverList observers;
public:
    static void addCommand(Command *c);
    static void undo();
    static void redo();
    static void clear();
    static void addObserver(HistoryObserver *obs);
    static void removeObserver(HistoryObserver *obs);

protected:
    static void notifyExecuted(Command *c);
    static void notifyUnExecuted(Command *c);
};

#endif
