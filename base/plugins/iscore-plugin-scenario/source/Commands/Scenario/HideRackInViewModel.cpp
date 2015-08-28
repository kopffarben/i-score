#include "HideRackInViewModel.hpp"
#include "Document/Constraint/ViewModels/ConstraintViewModel.hpp"
#include "iscore/document/DocumentInterface.hpp"

using namespace iscore;
using namespace Scenario::Command;

HideRackInViewModel::HideRackInViewModel(
        Path<ConstraintViewModel>&& path) :
    SerializableCommand {"ScenarioControl",
                         commandName(),
                         description()},
    m_constraintViewPath {std::move(path) }
{
    auto& constraint_vm = m_constraintViewPath.find();
    m_constraintPreviousId = constraint_vm.shownRack();
}

HideRackInViewModel::HideRackInViewModel(
        const ConstraintViewModel& constraint_vm) :
    SerializableCommand {"ScenarioControl",
                         commandName(),
                         description()},
    m_constraintViewPath {iscore::IDocument::path(constraint_vm) }
{
    m_constraintPreviousId = constraint_vm.shownRack();
}

void HideRackInViewModel::undo()
{
    auto& vm = m_constraintViewPath.find();
    vm.showRack(m_constraintPreviousId);
}

void HideRackInViewModel::redo()
{
    auto& vm = m_constraintViewPath.find();
    vm.hideRack();
}

void HideRackInViewModel::serializeImpl(QDataStream& s) const
{
    s << m_constraintViewPath
      << m_constraintPreviousId;
}

void HideRackInViewModel::deserializeImpl(QDataStream& s)
{
    s >> m_constraintViewPath
            >> m_constraintPreviousId;
}
