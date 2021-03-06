#include "ScenarioSettingsModel.hpp"
#include <QSettings>
#include <QFile>
#include <QJsonDocument>
#include <iscore/plugins/customfactory/FactoryFamily.hpp>
#include <iscore/model/Skin.hpp>

namespace Scenario
{
namespace Settings
{
namespace Parameters
{
        const iscore::sp<ModelSkinParameter> Skin{QStringLiteral("Skin/Skin"), "Default"};
        const iscore::sp<ModelGraphicZoomParameter> GraphicZoom{QStringLiteral("Skin/Zoom"), 1};
        const iscore::sp<ModelSlotHeightParameter> SlotHeight{QStringLiteral("Skin/slotHeight"), 200};
        const iscore::sp<ModelDefaultDurationParameter> DefaultDuration{QStringLiteral("Skin/defaultDuration"), TimeValue::fromMsecs(15000)};
        const iscore::sp<ModelSnapshotOnCreateParameter> SnapshotOnCreate{QStringLiteral("Scenario/SnapshotOnCreate"), true};
        const iscore::sp<ModelAutoSequenceParameter> AutoSequence{QStringLiteral("Scenario/AutoSequence"), true};

        static auto list() {
            return std::tie(Skin, GraphicZoom, SlotHeight, DefaultDuration, SnapshotOnCreate, AutoSequence);
        }
}

Model::Model(QSettings& set, const iscore::ApplicationContext& ctx)
{
    iscore::setupDefaultSettings(set, Parameters::list(), *this);
}

QString Model::getSkin() const
{
    return m_Skin;
}

void Model::setSkin(const QString& skin)
{
    if (m_Skin == skin)
        return;

    QFile f(":/DefaultSkin.json");
    if(skin == QStringLiteral("IEEE"))
    {
        f.setFileName(":/IEEESkin.json");
    }

    if(f.open(QFile::ReadOnly))
    {
        auto arr = f.readAll();
        QJsonParseError err;
        auto doc = QJsonDocument::fromJson(arr, &err);
        if(err.error)
        {
            qDebug() << "could not load skin : "<< err.errorString() << err.offset;
        }
        else
        {
            auto obj = doc.object();
            iscore::Skin::instance().load(obj);
        }
    }
    else
    {
        qDebug() << "could not open" << f.fileName();
    }

    m_Skin = skin;

    QSettings s;
    s.setValue(Parameters::Skin.key, m_Skin);
    emit SkinChanged(skin);
}

ISCORE_SETTINGS_PARAMETER_CPP(double, Model, GraphicZoom)
ISCORE_SETTINGS_PARAMETER_CPP(qreal, Model, SlotHeight)
ISCORE_SETTINGS_PARAMETER_CPP(TimeValue, Model, DefaultDuration)
ISCORE_SETTINGS_PARAMETER_CPP(bool, Model, SnapshotOnCreate)
ISCORE_SETTINGS_PARAMETER_CPP(bool, Model, AutoSequence)

}
}
