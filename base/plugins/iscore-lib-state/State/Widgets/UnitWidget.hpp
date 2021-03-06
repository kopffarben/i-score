#pragma once
#include <QWidget>
#include <iscore_lib_state_export.h>
#include <State/Unit.hpp>


class QComboBox;
class QHBoxLayout;
namespace State
{
class ISCORE_LIB_STATE_EXPORT UnitWidget : public QWidget
{
        Q_OBJECT
    public:
        UnitWidget(ossia::unit_t u, QWidget* parent);

        ossia::unit_t unit() const;
        void setUnit(ossia::unit_t);

    signals:
        void unitChanged(ossia::unit_t);

    private:
        void on_dataspaceChanged(ossia::unit_t);

        QHBoxLayout* m_layout{};
        QComboBox* m_dataspace{};
        QComboBox* m_unit{};
};
}
