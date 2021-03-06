#include <State/Widgets/AddressLineEdit.hpp>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>

#include <QSpinBox>
#include <QString>
#include <QVariant>

#include <Device/Protocol/ProtocolSettingsWidget.hpp>
#include "HTTPProtocolSettingsWidget.hpp"
#include "HTTPSpecificSettings.hpp"

#include <QPlainTextEdit>
#include <iscore/widgets/JS/JSEdit.hpp>
class QWidget;

namespace Engine
{
namespace Network
{
HTTPProtocolSettingsWidget::HTTPProtocolSettingsWidget(QWidget* parent)
    : ProtocolSettingsWidget(parent)
{
    QLabel* deviceNameLabel = new QLabel(tr("Device name"), this);
    m_deviceNameEdit = new State::AddressFragmentLineEdit{this};

    QLabel* codeLabel = new QLabel(tr("Code"), this);
    m_codeEdit = new JSEdit(this);


    QGridLayout* gLayout = new QGridLayout;

    gLayout->addWidget(deviceNameLabel, 0, 0, 1, 1);
    gLayout->addWidget(m_deviceNameEdit, 0, 1, 1, 1);

    gLayout->addWidget(codeLabel, 3, 0, 1, 1);
    gLayout->addWidget(m_codeEdit, 3, 1, 1, 1);

    setLayout(gLayout);

    setDefaults();
}

void
HTTPProtocolSettingsWidget::setDefaults()
{
    ISCORE_ASSERT(m_deviceNameEdit);
    ISCORE_ASSERT(m_codeEdit);

    m_deviceNameEdit->setText("newDevice");
    m_codeEdit->setPlainText("");
}

Device::DeviceSettings HTTPProtocolSettingsWidget::getSettings() const
{
    ISCORE_ASSERT(m_deviceNameEdit);

    Device::DeviceSettings s;
    s.name = m_deviceNameEdit->text();

    Network::HTTPSpecificSettings specific;
    specific.text = m_codeEdit->toPlainText();

    s.deviceSpecificSettings = QVariant::fromValue(specific);
    return s;
}

void
HTTPProtocolSettingsWidget::setSettings(const Device::DeviceSettings &settings)
{
    m_deviceNameEdit->setText(settings.name);
    Network::HTTPSpecificSettings specific;
    if(settings.deviceSpecificSettings.canConvert<Network::HTTPSpecificSettings>())
    {
        specific = settings.deviceSpecificSettings.value<Network::HTTPSpecificSettings>();
        m_codeEdit->setPlainText(specific.text);
    }
}
}
}
