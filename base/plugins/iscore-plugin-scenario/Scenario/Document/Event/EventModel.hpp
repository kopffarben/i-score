#pragma once
#include <iscore/tools/Metadata.hpp>
#include <iscore/model/Entity.hpp>
#include <Process/TimeValue.hpp>
#include <Scenario/Document/Event/ExecutionStatus.hpp>
#include <Scenario/Document/VerticalExtent.hpp>
#include <State/Expression.hpp>
#include <iscore/tools/std/Optional.hpp>
#include <iscore/selection/Selectable.hpp>
#include <iscore/tools/IdentifiedObject.hpp>
#include <iscore/tools/SettableIdentifier.hpp>
#include <QString>
#include <QVector>
#include <iscore_plugin_scenario_export.h>
#include <iscore/component/Component.hpp>
class DataStream;
class JSONObject;
class QObject;

namespace Scenario
{
class StateModel;
class ScenarioInterface;
class TimeNodeModel;

class ISCORE_PLUGIN_SCENARIO_EXPORT EventModel final :
        public iscore::Entity<EventModel>
{
        Q_OBJECT

        ISCORE_SERIALIZE_FRIENDS(Scenario::EventModel, DataStream)
        ISCORE_SERIALIZE_FRIENDS(Scenario::EventModel, JSONObject)

        Q_PROPERTY(Scenario::OffsetBehavior offsetBehavior READ offsetBehavior WRITE setOffsetBehavior NOTIFY offsetBehaviorChanged FINAL)

    public:
        /** Public properties of the class **/
        Selectable selection;

        /** The class **/
        EventModel(const Id<EventModel>&,
                   const Id<TimeNodeModel>& timenode,
                   const VerticalExtent& extent,
                   const TimeValue& date,
                   QObject* parent);

        // Copy
        EventModel(const EventModel& source,
                   const Id<EventModel>&,
                   QObject* parent);

        template<typename DeserializerVisitor>
        EventModel(DeserializerVisitor&& vis, QObject* parent) :
            Entity{vis, parent}
        {
            vis.writeTo(*this);
        }

        // Timenode
        void changeTimeNode(const Id<TimeNodeModel>& elt)
        { m_timeNode = elt; }
        const auto& timeNode() const
        { return m_timeNode; }

        // States
        void addState(const Id<StateModel>& ds);
        void removeState(const Id<StateModel>& ds);
        const QVector<Id<StateModel>>& states() const;


        // Other properties
        const State::Condition& condition() const;
        OffsetBehavior offsetBehavior() const;

        VerticalExtent extent() const;

        const TimeValue& date() const;
        void translate(const TimeValue& deltaTime);

        ExecutionStatus status() const;
        void reset();


        void setCondition(const State::Condition& arg);

        void setExtent(const VerticalExtent &extent);
        void setDate(const TimeValue& date);

        void setStatus(ExecutionStatus status);
        void setOffsetBehavior(OffsetBehavior);

    signals:
        void extentChanged(const VerticalExtent&);
        void dateChanged(const TimeValue&);

        void conditionChanged(const State::Condition&);

        void statesChanged();

        void statusChanged(Scenario::ExecutionStatus status);

        void offsetBehaviorChanged(OffsetBehavior);

    private:
        Id<TimeNodeModel> m_timeNode;

        QVector<Id<StateModel>> m_states;

        State::Condition m_condition;

        VerticalExtent m_extent;
        TimeValue m_date{TimeValue::zero()};

        ExecutionStatusProperty m_status{};
        OffsetBehavior m_offset{};
};
}

DEFAULT_MODEL_METADATA(Scenario::EventModel, "Event")
TR_TEXT_METADATA(, Scenario::EventModel, PrettyName_k, "Event")

