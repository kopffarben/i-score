#pragma once
#include <core/presenter/command/SerializableCommand.hpp>
#include <QPointF>
#include <QNamedObject>

class CreatEventCommand : public iscore::SerializableCommand
{
	public:
		CreatEventCommand(ObjectPath&& scenarioPath, int time);
		virtual void undo() override;
		virtual void redo() override;
		virtual int id() const override;
		virtual bool mergeWith(const QUndoCommand* other) override;

	protected:
		virtual void serializeImpl(QDataStream&) override;
		virtual void deserializeImpl(QDataStream&) override;
		
	private:
		ObjectPath m_path;
		
		int m_timeEventId{};
		int m_time{};
};
