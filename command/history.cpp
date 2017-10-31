#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include "history.h"
#include "command.h"

History::CommandList History::commands;
History::ObserverList History::observers;
unsigned History::position=0;

void History::addCommand(Command *c) {
    if (position == commands.size()) {
	commands.push_back(c);
        position++;
    } else {
	while (position < commands.size()) {
	    commands.erase(commands.begin()+position);
	}
	commands.push_back(c);
        position++;
    }
    notifyExecuted(c);
}

void History::undo() {
    if (position > 0) {
	position--;
	commands[position]->undo();
	notifyUnExecuted(commands[position].get());
    } else {
        //throw _("No more commands to undo");
	//std::cout << "No more commands to undo" << std::endl;
    }
}

void History::redo() {
    if (position < commands.size()) {
	commands[position]->execute(false);
	notifyExecuted(commands[position].get());
	position++;
    } else {
	//throw _("No more commands to redo");
	//std::cout << "No more commands to redo" << std::endl;
    }
}

void History::clear() {
    commands.clear();
    position=0;
}

void History::addObserver(HistoryObserver *obs) {
    observers.push_back(obs);
}

void History::removeObserver(HistoryObserver *obs) {
    ObserverList::iterator iter;
    for (iter=observers.begin();iter!=observers.end();) {
	HistoryObserver *ho = *iter;
	if (ho == obs) {
	    iter=observers.erase(iter);
	} else {
            iter++;
	}
    }
}

void History::notifyExecuted(Command *c) {
    for (unsigned i=0;i<observers.size();i++) {
	observers[i]->executed(c);
    }
}

void History::notifyUnExecuted(Command *c) {
    for (unsigned i=0;i<observers.size();i++) {
	observers[i]->unexecuted(c);
    }
}
