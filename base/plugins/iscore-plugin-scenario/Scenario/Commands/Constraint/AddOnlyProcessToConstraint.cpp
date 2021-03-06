#include <Process/ProcessFactory.hpp>
#include <Process/ProcessList.hpp>
#include <Scenario/Document/Constraint/ConstraintModel.hpp>

#include <Scenario/Document/State/StateModel.hpp>
#include <Scenario/Document/State/ItemModel/MessageItemModel.hpp>
#include <Scenario/Document/Constraint/ConstraintDurations.hpp>
#include <Scenario/Process/Algorithms/Accessors.hpp>
#include <Scenario/Process/Algorithms/ProcessPolicy.hpp>
#include <Scenario/Process/ScenarioInterface.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/multi_index/detail/hash_index_iterator.hpp>

#include <iscore/tools/SettableIdentifierGeneration.hpp>
#include <algorithm>
#include <vector>

#include "AddOnlyProcessToConstraint.hpp"
#include <iscore/application/ApplicationContext.hpp>
#include <iscore/plugins/customfactory/FactoryFamily.hpp>

#include <iscore/plugins/customfactory/StringFactoryKey.hpp>
#include <iscore/serialization/DataStreamVisitor.hpp>
#include <iscore/tools/ModelPathSerialization.hpp>
#include <iscore/tools/EntityMap.hpp>
#include <iscore/plugins/customfactory/StringFactoryKeySerialization.hpp>


namespace Scenario
{
namespace Command
{
AddOnlyProcessToConstraint::AddOnlyProcessToConstraint(
        Path<ConstraintModel>&& constraintPath,
        UuidKey<Process::ProcessModelFactory> process) :
    AddOnlyProcessToConstraint{
        std::move(constraintPath),
        getStrongId(constraintPath.find().processes),
        process}
{

}

AddOnlyProcessToConstraint::AddOnlyProcessToConstraint(
        Path<ConstraintModel>&& constraintPath,
        Id<Process::ProcessModel> processId,
        UuidKey<Process::ProcessModelFactory> process):
    m_path{std::move(constraintPath)},
    m_processName{process},
    m_createdProcessId{std::move(processId)}
{
}

void AddOnlyProcessToConstraint::undo() const
{
    undo(m_path.find());
}

void AddOnlyProcessToConstraint::redo() const
{
    redo(m_path.find());
}

void AddOnlyProcessToConstraint::undo(ConstraintModel& constraint) const
{
    RemoveProcess(constraint, m_createdProcessId);
}

Process::ProcessModel& AddOnlyProcessToConstraint::redo(ConstraintModel& constraint) const
{
    // Create process model
    auto fac = context.components.factory<Process::ProcessFactoryList>().get(m_processName);
    ISCORE_ASSERT(fac);
    auto proc = fac->make(
                constraint.duration.defaultDuration(), // TODO should maybe be max ?
                m_createdProcessId,
                &constraint);

    AddProcess(constraint, proc);
    return *proc;
}

void AddOnlyProcessToConstraint::serializeImpl(DataStreamInput& s) const
{
    s << m_path << m_processName << m_createdProcessId;
}

void AddOnlyProcessToConstraint::deserializeImpl(DataStreamOutput& s)
{
    s >> m_path >> m_processName >> m_createdProcessId;
}
}
}
