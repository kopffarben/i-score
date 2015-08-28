#pragma once
#include <iscore/command/SerializableCommand.hpp>
#include <iscore/tools/ModelPath.hpp>

#include <ProcessInterface/ExpandMode.hpp>
#include <tests/helpers/ForwardDeclaration.hpp>
#include <ProcessInterface/TimeValue.hpp>

class ConstraintModel;
class ScenarioModel;

/*
 * Command for vertical move so it does'nt have to resize anything on time axis
 * */

namespace Scenario
{
    namespace Command
    {
        class MoveConstraint : public iscore::SerializableCommand
        {
                ISCORE_COMMAND_DECL("MoveConstraint", "MoveConstraint")
#include <tests/helpers/FriendDeclaration.hpp>
            public:
                MoveConstraint();
                ~MoveConstraint();
                MoveConstraint(
                        Path<ScenarioModel>&& scenarioPath,
                    const Id<ConstraintModel>& id,
                    const TimeValue& date,
                    double y);

                void update(const Path<ScenarioModel>&,
                            const Id<ConstraintModel>& ,
                            const TimeValue& date,
                            double height);

                virtual void undo() override;
                virtual void redo() override;

            protected:
                virtual void serializeImpl(QDataStream&) const override;
                virtual void deserializeImpl(QDataStream&) override;

            private:
                Path<ScenarioModel> m_path;
                Id<ConstraintModel> m_constraint;
                double m_oldHeight{},
                       m_newHeight{};
        };
    }
}
