#include "LocalTreeDocumentPlugin.hpp"
#include <ossia/network/base/device.hpp>
#include <ossia/editor/value/value.hpp>
#include <ossia/network/base/address.hpp>
#include <ossia/editor/state/state_element.hpp>


#include <Curve/CurveModel.hpp>
#include <Automation/AutomationModel.hpp>

#include <Scenario/Document/ScenarioDocument/ScenarioDocumentModel.hpp>
#include <Scenario/Document/BaseScenario/BaseScenario.hpp>
#include <Scenario/Document/Constraint/ConstraintModel.hpp>
#include <Scenario/Document/Event/EventModel.hpp>
#include <Scenario/Document/State/StateModel.hpp>
#include <Scenario/Document/State/ItemModel/MessageItemModel.hpp>
#include <Scenario/Document/TimeNode/TimeNodeModel.hpp>
#include <Scenario/Document/TimeNode/Trigger/TriggerModel.hpp>
#include <Scenario/Process/ScenarioModel.hpp>
#include <Scenario/Process/Algorithms/Accessors.hpp>

#include <State/Message.hpp>
#include <State/Value.hpp>

#include <Process/State/MessageNode.hpp>

#include <Curve/Segment/CurveSegmentData.hpp>

#include <core/document/Document.hpp>
#include <core/document/DocumentModel.hpp>
#include <iscore/document/DocumentInterface.hpp>
#include <Engine/OSSIA2iscore.hpp>
#include <Engine/iscore2OSSIA.hpp>

#include "Scenario/ScenarioComponent.hpp"
#include <Engine/LocalTree/Settings/LocalTreeModel.hpp>
#include <Engine/Protocols/Local/LocalProtocolFactory.hpp>
#include <Engine/Protocols/Local/LocalSpecificSettings.hpp>
#include <Explorer/DocumentPlugin/DeviceDocumentPlugin.hpp>

Engine::LocalTree::DocumentPlugin::DocumentPlugin(
        iscore::Document& doc,
        Id<iscore::DocumentPlugin> id,
        QObject* parent):
    iscore::DocumentPlugin{doc.context(), std::move(id), "LocalTree::DocumentPlugin", parent},
    m_localDevice{std::make_unique<ossia::net::local_protocol>(), "i-score"},
    m_localDeviceWrapper{
        m_localDevice,
        doc.context(),
        Network::LocalProtocolFactory::static_defaultSettings()}
{
    con(doc, &iscore::Document::aboutToClose,
        this, &DocumentPlugin::cleanup);
}

Engine::LocalTree::DocumentPlugin::~DocumentPlugin()
{
    cleanup();

    auto docplug = context().findPlugin<Explorer::DeviceDocumentPlugin>();
    if(docplug)
        docplug->list().setLocalDevice(nullptr);
}

void Engine::LocalTree::DocumentPlugin::init()
{
    auto& set = m_context.app.settings<Settings::Model>();
    if(set.getLocalTree())
    {
        create();
    }

    con(set, &Settings::Model::LocalTreeChanged,
        this, [=] (bool b) {
        if(b)
            create();
        else
            cleanup();
    }, Qt::QueuedConnection);

    auto docplug = context().findPlugin<Explorer::DeviceDocumentPlugin>();
    if(docplug)
        docplug->list().setLocalDevice(&m_localDeviceWrapper);
}

void Engine::LocalTree::DocumentPlugin::create()
{
    if(m_root)
        cleanup();

    auto& doc = m_context.document.model().modelDelegate();
    auto scenar = dynamic_cast<Scenario::ScenarioDocumentModel*>(&doc);
    if(!scenar)
        return;

    auto& cstr = scenar->baseScenario().constraint();
    m_root = new Constraint(
                m_localDevice.getRootNode(),
                getStrongId(cstr.components()),
                cstr,
                *this,
                this);
    cstr.components().add(m_root);
}

void Engine::LocalTree::DocumentPlugin::cleanup()
{
    if(!m_root)
        return;

    m_root->constraint().components().remove(m_root);
    m_root = nullptr;
}
