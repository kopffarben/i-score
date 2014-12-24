#pragma once
#include <core/presenter/command/SerializableCommand.hpp>
#include <tools/ObjectPath.hpp>

class AbstractConstraintViewModel;
class ConstraintModel;

namespace Scenario
{
	namespace Command
	{
		/**
		 * @brief The HideBoxInViewModel class
		 *
		 * For a given constraint view model, hides the box.
		 * Can only be called if a box was being displayed.
		 */
		class HideBoxInViewModel : public iscore::SerializableCommand
		{
			public:
				HideBoxInViewModel(ObjectPath&& path);

				/**
				 * @brief HideBoxInViewModel
				 * @param constraint A pointer to a valid constraint view model.
				 *
				 * Note : this will search it and make a path from an object named "BaseConstraintModel"
				 * Hence this constructor has to be used in a Scenario.
				 */
				HideBoxInViewModel(AbstractConstraintViewModel* constraint);

				virtual void undo() override;
				virtual void redo() override;
				virtual int id() const override;
				virtual bool mergeWith(const QUndoCommand* other) override;

			protected:
				virtual void serializeImpl(QDataStream&) override;
				virtual void deserializeImpl(QDataStream&) override;

			private:
				ObjectPath m_constraintViewModelPath;

				bool m_constraintPreviousId{};
		};
	}
}
