#pragma once
#include <Process/Inspector/ProcessInspectorWidgetDelegate.hpp>
#include <iscore/command/Dispatchers/CommandDispatcher.hpp>
class QWidget;

namespace iscore{
class Document;
struct DocumentContext;
}
namespace State
{
struct AddressAccessor;
}

namespace Explorer
{
class AddressAccessorEditWidget;
class DeviceExplorerModel;
}
class QDoubleSpinBox;

namespace Mapping
{
class ProcessModel;
class InspectorWidget :
        public Process::InspectorWidgetDelegate_T<ProcessModel>
{
    public:
        explicit InspectorWidget(
                const ProcessModel& object,
                const iscore::DocumentContext& context,
                QWidget* parent);

    public slots:
        void on_sourceAddressChange(const State::AddressAccessor& newText);
        void on_sourceMinValueChanged();
        void on_sourceMaxValueChanged();

        void on_targetAddressChange(const State::AddressAccessor& newText);
        void on_targetMinValueChanged();
        void on_targetMaxValueChanged();
    private:
        Explorer::AddressAccessorEditWidget* m_sourceLineEdit{};
        QDoubleSpinBox* m_sourceMin{}, *m_sourceMax{};

        Explorer::AddressAccessorEditWidget* m_targetLineEdit{};
        QDoubleSpinBox* m_targetMin{}, *m_targetMax{};

        CommandDispatcher<> m_dispatcher;
};
}
