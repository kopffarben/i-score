#include "ProcessModel.hpp"
#include "LayerModel.hpp"
#include <iscore/document/DocumentInterface.hpp>

Process::Process(
        const TimeValue& duration,
        const Id<Process>& id,
        const QString& name,
        QObject* parent):
    IdentifiedObject<Process>{id, name, parent},
    m_duration{duration}
{

}

Process::Process(
        const Process& source,
        const Id<Process>& id,
        const QString& name,
        QObject* parent):
    IdentifiedObject<Process>{id, name, parent},
    m_duration{source.duration()}
{

}


QByteArray Process::makeViewModelConstructionData() const { return {}; }


LayerModel*Process::makeLayer(
        const Id<LayerModel>& viewModelId,
        const QByteArray& constructionData,
        QObject* parent)
{
    auto lm = makeLayer_impl(viewModelId, constructionData, parent);
    addLayer(lm);

    return lm;
}


LayerModel*Process::loadLayer(
        const VisitorVariant& v,
        QObject* parent)
{
    auto lm = loadLayer_impl(v, parent);
    addLayer(lm);

    return lm;
}

LayerModel*Process::cloneLayer(
        const Id<LayerModel>& newId,
        const LayerModel& source,
        QObject* parent)
{
    auto lm = cloneLayer_impl(newId, source, parent);
    addLayer(lm);

    return lm;
}


LayerModel*Process::makeTemporaryLayer(
        const Id<LayerModel>& newId,
        const LayerModel& source,
        QObject* parent)
{
    return cloneLayer_impl(newId, source, parent);
}



QVector<LayerModel*> Process::layers() const
{
    return m_layers;
}


void Process::expandProcess(ExpandMode mode, const TimeValue& t)
{
    if(mode == ExpandMode::Scale)
    {
        setDurationAndScale(t);
    }
    else if (mode == ExpandMode::Grow)
    {
        if(duration() < t)
            setDurationAndGrow(t);
        else
            setDurationAndShrink(t);
    }
}


void Process::setDuration(const TimeValue& other)
{
    m_duration = other;
}


const TimeValue& Process::duration() const
{
    return m_duration;
}


void Process::addLayer(LayerModel* m)
{
    connect(m, &LayerModel::destroyed,
            this, [=] () { removeLayer(m); });
    m_layers.push_back(m);
}


void Process::removeLayer(LayerModel* m)
{
    int index = m_layers.indexOf(m);

    if(index != -1)
    {
        m_layers.remove(index);
    }
}


Process* parentProcess(QObject* obj)
{
    QString objName (obj ? obj->objectName() : "INVALID");
    while(obj && !obj->inherits(Process::staticMetaObject.className()))
    {
        obj = obj->parent();
    }

    if(!obj)
        throw std::runtime_error(
                QString("Object (name: %1) is not child of a Process!")
                .arg(objName)
                .toStdString());

    return static_cast<Process*>(obj);
}


const Process* parentProcess(const QObject* obj)
{
    QString objName (obj ? obj->objectName() : "INVALID");
    while(obj && !obj->inherits(Process::staticMetaObject.className()))
    {
        obj = obj->parent();
    }

    if(!obj)
        throw std::runtime_error(
                QString("Object (name: %1) is not child of a Process!")
                .arg(objName)
                .toStdString());

    return static_cast<const Process*>(obj);
}
