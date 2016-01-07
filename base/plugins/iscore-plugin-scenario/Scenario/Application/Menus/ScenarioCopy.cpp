#include "ScenarioCopy.hpp"
#include <Scenario/Document/Constraint/ConstraintModel.hpp>
#include <Scenario/Document/Event/EventModel.hpp>
#include <Scenario/Document/State/StateModel.hpp>
#include <Scenario/Document/TimeNode/TimeNodeModel.hpp>
#include <Scenario/Process/ScenarioModel.hpp>
#include <Scenario/Process/Algorithms/ProcessPolicy.hpp>
#include <boost/optional/optional.hpp>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QList>
#include <algorithm>
#include <vector>


#include <Scenario/Document/BaseScenario/BaseScenario.hpp>
#include <iscore/serialization/VisitorCommon.hpp>
#include <iscore/tools/NotifyingMap.hpp>
#include <iscore/tools/SettableIdentifier.hpp>
#include <iscore/document/DocumentContext.hpp>
#include <iscore/tools/std/Algorithms.hpp>

QJsonObject copyBaseConstraint(const ConstraintModel& cst)
{
    QJsonObject base;
    QJsonArray arr;
    arr.push_back(marshall<JSONObject>(cst));
    base["Constraints"] = arr;

    return base;
}

template<typename Selected_T>
static auto arrayToJson(Selected_T &&selected)
{
    QJsonArray array;
    if (!selected.empty())
    {
        for (const auto &element : selected)
        {
            array.push_back(marshall<JSONObject>(*element));
        }
    }

    return array;
}

QJsonObject copySelectedScenarioElements(const Scenario::ScenarioModel &sm)
{
    auto selectedConstraints = selectedElements(sm.constraints);
    auto selectedEvents = selectedElements(sm.events);
    auto selectedTimeNodes = selectedElements(sm.timeNodes);
    auto selectedStates = selectedElements(sm.states);

    for(const ConstraintModel* constraint : selectedConstraints)
    {
        auto start_it = find_if(selectedStates,
                                [&] (const StateModel* state) { return state->id() == constraint->startState();});
        if(start_it == selectedStates.end())
        {
            selectedStates.push_back(&sm.state(constraint->startState()));
        }

        auto end_it = find_if(selectedStates,
                              [&] (const StateModel* state) { return state->id() == constraint->endState();});
        if(end_it == selectedStates.end())
        {
            selectedStates.push_back(&sm.state(constraint->endState()));
        }
    }

    for(const StateModel* state : selectedStates)
    {
        auto ev_it = find_if(selectedEvents,
                             [&] (const EventModel* event) { return state->eventId() == event->id(); });
        if(ev_it == selectedEvents.end())
        {
            selectedEvents.push_back(&sm.event(state->eventId()));
        }

        // If the previous or next constraint is not here, we set it to null in a copy.
    }
    for(const EventModel* event : selectedEvents)
    {
        auto tn_it = find_if(selectedTimeNodes,
                             [&] (const TimeNodeModel* tn) { return tn->id() == event->timeNode(); });
        if(tn_it == selectedTimeNodes.end())
        {
            selectedTimeNodes.push_back(&sm.timeNode(event->timeNode()));
        }

        // If some events aren't there, we set them to null in a copy.
    }

    std::vector<TimeNodeModel*> copiedTimeNodes;
    copiedTimeNodes.reserve(selectedTimeNodes.size());
    for(const auto& tn : selectedTimeNodes)
    {
        auto clone_tn = new TimeNodeModel(*tn, tn->id(), sm.parent());
        auto events = clone_tn->events();
        for(const auto& event : events)
        {
            auto absent = none_of(selectedEvents,
                                  [&] (const EventModel* ev) { return ev->id() == event; });
            if(absent)
                clone_tn->removeEvent(event);
        }

        copiedTimeNodes.push_back(clone_tn);
    }


    std::vector<EventModel*> copiedEvents;
    copiedEvents.reserve(selectedEvents.size());
    for(const auto& ev : selectedEvents)
    {
        auto clone_ev = new EventModel(*ev, ev->id(), sm.parent());
        auto states = clone_ev->states();
        for(const auto& state : states)
        {
            auto absent = none_of(selectedStates,
                                  [&] (const StateModel* st) { return st->id() == state; });
            if(absent)
                clone_ev->removeState(state);
        }

        copiedEvents.push_back(clone_ev);
    }

    std::vector<StateModel*> copiedStates;
    copiedStates.reserve(selectedStates.size());
    auto& stack = iscore::IDocument::documentContext(sm).commandStack;
    for(const StateModel* st : selectedStates)
    {
        auto clone_st = new StateModel(*st, st->id(), stack, sm.parent());

        // NOTE : we must not serialize the state with their previous / next constraint
        // since they will change once pasted and cause crash at the end of the ctor
        // of StateModel. They are saved in the previous / next state of constraint anyway.
        SetNoPreviousConstraint(*clone_st);
        SetNoNextConstraint(*clone_st);

        copiedStates.push_back(clone_st);
    }


    QJsonObject base;
    base["Constraints"] = arrayToJson(selectedConstraints);
    base["Events"] = arrayToJson(copiedEvents);
    base["TimeNodes"] = arrayToJson(copiedTimeNodes);
    base["States"] = arrayToJson(copiedStates);
    base["Comments"] = arrayToJson(selectedElements(sm.comments));

    for(auto elt : copiedTimeNodes)
        delete elt;
    for(auto elt : copiedEvents)
        delete elt;
    for(auto elt : copiedStates)
        delete elt;

    return base;
}


QJsonObject copySelectedScenarioElements(BaseScenario &sm)
{
    QJsonObject base;
    if(sm.constraint().selection.get())
    {
        std::vector<TimeNodeModel*> copiedTimeNodes;
        copiedTimeNodes.reserve(2);
        for(const auto& tn : sm.timeNodes())
        {
            auto clone_tn = new TimeNodeModel(tn, tn.id(), &sm);
            copiedTimeNodes.push_back(clone_tn);
        }


        std::vector<EventModel*> copiedEvents;
        copiedEvents.reserve(2);
        for(const auto& ev : sm.events())
        {
            auto clone_ev = new EventModel(ev, ev.id(), &sm);
            copiedEvents.push_back(clone_ev);
        }

        std::vector<StateModel*> copiedStates;
        copiedStates.reserve(2);
        auto& stack = iscore::IDocument::documentContext(sm.parentObject()).commandStack;
        for(const StateModel& st : sm.states())
        {
            auto clone_st = new StateModel(st, st.id(), stack, &sm);

            // NOTE : see above note on copySelectedScenarioElements(const Scenario::ScenarioModel &)
            SetNoPreviousConstraint(*clone_st);
            SetNoNextConstraint(*clone_st);
            copiedStates.push_back(clone_st);
        }


        QJsonArray arr;
        arr.push_back(marshall<JSONObject>(sm.constraint()));
        base["Constraints"] = arr;

        base["Events"] = arrayToJson(copiedEvents);
        base["TimeNodes"] = arrayToJson(copiedTimeNodes);
        base["States"] = arrayToJson(copiedStates);


        for(auto elt : copiedTimeNodes)
            delete elt;
        for(auto elt : copiedEvents)
            delete elt;
        for(auto elt : copiedStates)
            delete elt;
    }

    return base;

}