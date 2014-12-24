#include <QtTest/QtTest>

#include <Commands/Scenario/CreateEvent.hpp>

#include <Document/Event/EventModel.hpp>
#include <Document/Event/EventData.hpp>

#include <Process/ScenarioProcessSharedModel.hpp>

using namespace iscore;
using namespace Scenario::Command;


class CreateEventTest: public QObject
{
		Q_OBJECT
	public:

	private slots:

		void CreateTest()
		{
			ScenarioProcessSharedModel* scenar = new ScenarioProcessSharedModel(0, qApp);
			EventData data{};
			// data.id = 0; unused here
			data.x = 10;
			data.relativeY = 0.5;

			CreateEvent cmd(
			{
				{"ScenarioProcessSharedModel", {}},
			}, data.x, data.relativeY);

			cmd.redo();
			QCOMPARE((int)scenar->events().size(), 2); // TODO 3 if endEvent
			QCOMPARE(scenar->event(cmd.m_createdEventId)->heightPercentage(), 0.5);

			cmd.undo();
			QCOMPARE((int)scenar->events().size(), 1); // TODO 2 if endEvent
			cmd.redo();

			QCOMPARE((int)scenar->events().size(), 2);
			QCOMPARE(scenar->event(cmd.m_createdEventId)->heightPercentage(), 0.5);


			// Delete them else they stay in qApp !

			delete scenar;
		}
};

QTEST_MAIN(CreateEventTest)
#include "CreateEventTest.moc"


