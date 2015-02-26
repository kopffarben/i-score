#include "DeviceExplorerInterface.hpp"

#include "../Panel/DeviceExplorerModel.hpp"
#include "core/interface/document/DocumentInterface.hpp"
#include "core/document/Document.hpp"
#include <core/document/DocumentModel.hpp>
#include <interface/panel/PanelModelInterface.hpp>
#include "../Panel/Node.hpp"

QString DeviceExplorer::panelName()
{
    return "DeviceExplorerPanelModel";
}

QString DeviceExplorer::explorerName()
{
    return "DeviceExplorerModel";
}


DeviceExplorerModel* DeviceExplorer::getModel (QObject* object)
{
    return iscore::IDocument::documentFromObject (object)
           ->model()
           ->panel (DeviceExplorer::panelName() )
           ->findChild<DeviceExplorerModel*> (DeviceExplorer::explorerName() );
}


QJsonObject DeviceExplorer::toJson (DeviceExplorerModel* deviceExplorer)
{
    return nodeToJson (deviceExplorer->rootNode() );
}


QString DeviceExplorer::addressFromModelIndex (const QModelIndex& m)
{
    QModelIndex index = m;
    QString txt;

    while (index.isValid() )
    {
        txt.prepend (QString ("/%1")
                     .arg (index.data (0).toString() ) );
        index = index.parent();
    }

    return txt;
}
