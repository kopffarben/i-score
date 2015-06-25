#include "ScenarioSelectionState.hpp"

#include "Process/Temporal/TemporalScenarioPresenter.hpp"
#include "Process/ScenarioModel.hpp"

#include "Document/Event/EventView.hpp"
#include "Document/TimeNode/TimeNodeView.hpp"
#include "Document/Constraint/ViewModels/AbstractConstraintView.hpp"
#include "Document/Constraint/ViewModels/AbstractConstraintViewModel.hpp"


ScenarioSelectionState::ScenarioSelectionState(
        iscore::SelectionStack &stack,
        const ScenarioStateMachine &parentSM,
        TemporalScenarioView &scenarioview,
        QState *parent):
    CommonSelectionState{stack, &scenarioview, parent},
    m_parentSM{parentSM},
    m_scenarioView{scenarioview}
{

}

void ScenarioSelectionState::on_pressAreaSelection()
{
    m_initialPoint = m_parentSM.scenePoint;
}

void ScenarioSelectionState::on_moveAreaSelection()
{
    m_movePoint = m_parentSM.scenePoint;
    m_scenarioView.setSelectionArea(
                QRectF{m_scenarioView.mapFromScene(m_initialPoint),
                       m_scenarioView.mapFromScene(m_movePoint)}.normalized());
    setSelectionArea(QRectF{m_initialPoint, m_movePoint}.normalized());
}

void ScenarioSelectionState::on_releaseAreaSelection()
{
    m_scenarioView.setSelectionArea(QRectF{});
}

void ScenarioSelectionState::on_deselect()
{
    dispatcher.setAndCommit(Selection{});
    m_scenarioView.setSelectionArea(QRectF{});
}

void ScenarioSelectionState::on_delete()
{
    ScenarioGlobalCommandManager mgr{m_parentSM.commandStack()};
    mgr.removeSelection(m_parentSM.model());
}

void ScenarioSelectionState::on_deleteContent()
{
    ScenarioGlobalCommandManager mgr{m_parentSM.commandStack()};
    mgr.clearContentFromSelection(m_parentSM.model());
}

void ScenarioSelectionState::setSelectionArea(const QRectF &area)
{
    using namespace std;
    QPainterPath path;
    path.addRect(area);
    Selection sel;

    auto scenario = &m_parentSM.presenter().viewModel().sharedProcessModel();
    for(const auto& item : m_parentSM.scene().items(path))
    {
        switch(item->type())
        {
            case QGraphicsItem::UserType + 1: // event
            {
                const auto& ev_model = static_cast<const EventView*>(item)->presenter().model();
                if(ev_model.parentScenario() == scenario)
                    sel.insert(&ev_model);
                break;
            }
            case QGraphicsItem::UserType + 2: // constraint
            {
                const auto& cst_model = static_cast<const AbstractConstraintView*>(item)->presenter().abstractConstraintViewModel().model();
                if(cst_model.parentScenario() == scenario)
                    sel.insert(&cst_model);
                break;
            }
            case QGraphicsItem::UserType + 3: // timenode
            {
                const auto& tn_model = static_cast<const TimeNodeView*>(item)->presenter().model();
                if(tn_model.parentScenario() == scenario)
                    sel.insert(&tn_model);
                break;
            }
        }
    }

    dispatcher.setAndCommit(filterSelections(sel,
                                               m_parentSM.model().selectedChildren(),
                                               multiSelection()));
}
