#include <Automation/AutomationColors.hpp>
#include <Automation/AutomationLayerModel.hpp>
#include <Automation/AutomationModel.hpp>
#include <Automation/AutomationPresenter.hpp>
#include <Automation/AutomationView.hpp>
#include <unordered_map>

#include <Automation/AutomationProcessMetadata.hpp>
#include <Automation/Commands/AutomationCommandFactory.hpp>
#include <Inspector/InspectorWidgetFactoryInterface.hpp>
#include <Process/ProcessFactory.hpp>
#include <Process/Inspector/ProcessInspectorWidgetDelegate.hpp>
#include <iscore/plugins/customfactory/StringFactoryKey.hpp>
#include <iscore/plugins/customfactory/FactorySetup.hpp>
#include "iscore_plugin_automation.hpp"

#include <Automation/Inspector/AutomationInspectorFactory.hpp>
#include <Automation/Inspector/AutomationStateInspectorFactory.hpp>
#include <Automation/Inspector/CurvePointInspectorFactory.hpp>
#include <iscore_plugin_automation_commands_files.hpp>
#include <Curve/Process/CurveProcessFactory.hpp>
#include <Process/GenericProcessFactory.hpp>
namespace Automation {
using AutomationFactory =
    Process::GenericProcessModelFactory<Automation::ProcessModel>;
using AutomationLayerFactory =
    Curve::CurveLayerFactory_T<
Automation::ProcessModel,
Automation::Layer,
Automation::LayerPresenter,
Automation::LayerView,
Automation::Colors>;
}

iscore_plugin_automation::iscore_plugin_automation() = default;
iscore_plugin_automation::~iscore_plugin_automation() = default;

std::vector<std::unique_ptr<iscore::FactoryInterfaceBase>> iscore_plugin_automation::factories(
        const iscore::ApplicationContext& ctx,
        const iscore::AbstractFactoryKey& key) const
{
    return instantiate_factories<
            iscore::ApplicationContext,
    TL<
        FW<Process::ProcessModelFactory,
             Automation::AutomationFactory>,
        FW<Process::LayerFactory,
             Automation::AutomationLayerFactory>,
        FW<Inspector::InspectorWidgetFactory,
             Automation::StateInspectorFactory,
             Automation::PointInspectorFactory>,
        FW<Process::InspectorWidgetDelegateFactory,
             Automation::InspectorFactory>
    >>(ctx, key);
}

std::pair<const CommandParentFactoryKey, CommandGeneratorMap> iscore_plugin_automation::make_commands()
{
    using namespace Automation;
    std::pair<const CommandParentFactoryKey, CommandGeneratorMap> cmds{CommandFactoryName(), CommandGeneratorMap{}};

    using Types = TypeList<
#include <iscore_plugin_automation_commands.hpp>
      >;
    for_each_type<Types>(iscore::commands::FactoryInserter{cmds.second});

    return cmds;
}


iscore::Version iscore_plugin_automation::version() const
{
    return iscore::Version{1};
}

UuidKey<iscore::Plugin> iscore_plugin_automation::key() const
{
    return_uuid("255cbc40-c7e9-4bb2-87ea-8ad803fb9f2b");
}
