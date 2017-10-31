#include "senvitem.h"

#include <qstring.h>

SEnvItem::SEnvItem(SEnvironment * env, QListView * parent)
                    : SGuiTreeViewItem(SGuiTreeViewItem::ENVIRONMENT, parent)
{
    mEnvironment = env;
    this->setText(0, QString("Environment"));
}

void SEnvItem::setEnvironment(SEnvironment * env)
{
    mEnvironment = env;
}

