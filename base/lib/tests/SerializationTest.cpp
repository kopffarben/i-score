#include <QtTest/QtTest>
#include <interface/serialization/DataStreamVisitor.hpp>
#include <interface/serialization/JSONVisitor.hpp>
#include <core/tools/ObjectPath.hpp>

class SerializationTest: public QObject
{
        Q_OBJECT

    private slots:
        void JSONTest()
        {
            Visitor<Reader<JSON>> reader;
            reader.readFrom (test_path);

            ObjectPath path;
            Visitor<Writer<JSON>> writer (reader.m_obj);
            writer.writeTo (path);

            QVERIFY (path == test_path);
        }

        void DataStreamTest()
        {
            QByteArray arr;
            Visitor<Reader<DataStream>> reader (&arr);
            reader.readFrom (test_path);

            ObjectPath path;
            Visitor<Writer<DataStream>> writer (&arr);
            writer.writeTo (path);

            QVERIFY (path == test_path);
        }

    private:
        const ObjectPath test_path
        {
            {"ConstraintModel", {}
            },
            {"ConstraintModel", 0},
            {"ScenarioProcessSharedModel", 23},
            {"ConstraintModel", -42}
        };
};

QTEST_MAIN (SerializationTest)
#include "SerializationTest.moc"

