#pragma once
#include <ProcessModel/TimeProcessWithConstraint.hpp>
#include <State/Value.hpp>
#include <QJSEngine>
class DeviceList;
class JSProcess : public TimeProcessWithConstraint
{
    public:
        JSProcess(DeviceList& devices);

        void setTickFun(const QString& val);

        std::shared_ptr<OSSIA::StateElement> state(
                const OSSIA::TimeValue&,
                const OSSIA::TimeValue&) override;

        const std::shared_ptr<OSSIA::State>& getStartState() const override
        {
            return m_start;
        }

        const std::shared_ptr<OSSIA::State>& getEndState() const override
        {
            return m_end;
        }


    private:
        DeviceList& m_devices;
        QJSEngine m_engine;
        QJSValue m_tickFun;

        std::shared_ptr<OSSIA::TimeConstraint> m_constraint;
        std::shared_ptr<OSSIA::State> m_start;
        std::shared_ptr<OSSIA::State> m_end;
};