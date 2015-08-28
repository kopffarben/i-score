#pragma once

#include <iscore/command/SerializableCommand.hpp>

#include <iscore/tools/ModelPath.hpp>

class ScenarioModel;
class EventModel;
class TimeNodeModel;

namespace Scenario
{
    namespace Command
    {
        class MergeTimeNodes : public iscore::SerializableCommand
        {
                ISCORE_COMMAND_DECL("MergeTimeNodes", "MergeTimeNodes")
            public:
                ISCORE_SERIALIZABLE_COMMAND_DEFAULT_CTOR(MergeTimeNodes, "ScenarioControl")
                MergeTimeNodes(Path<ScenarioModel>&& path, Id<TimeNodeModel> aimedTimeNode, Id<TimeNodeModel> movingTimeNode);
                virtual void undo() override;
                virtual void redo() override;

            protected:
                virtual void serializeImpl(QDataStream&) const override;
                virtual void deserializeImpl(QDataStream&) override;

            private:
                Path<ScenarioModel> m_path;

                Id<TimeNodeModel> m_aimedTimeNodeId;
                Id<TimeNodeModel> m_movingTimeNodeId;

                QByteArray m_serializedTimeNode;
         };
    }
}
