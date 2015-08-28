#include "SlotModel.hpp"

#include "Document/Constraint/ConstraintModel.hpp"
#include "Document/Constraint/Rack/RackModel.hpp"

#include "ProcessInterface/ProcessModel.hpp"
#include "ProcessInterface/LayerModel.hpp"

SlotModel::SlotModel(
        const Id<SlotModel>& id,
        RackModel* parent) :
    IdentifiedObject<SlotModel> {id, "SlotModel", parent}
{
}

SlotModel::SlotModel(
        std::function<void(const SlotModel&, SlotModel&)> lmCopyMethod,
        const SlotModel& source,
        const Id<SlotModel>& id,
        RackModel *parent):
    IdentifiedObject<SlotModel> {id, "SlotModel", parent},
    m_frontLayerModelId {source.frontLayerModel() }, // Keep the same id.
    m_height {source.height() }
{
    lmCopyMethod(source, *this);
}

void SlotModel::copyViewModelsInSameConstraint(
        const SlotModel &source,
        SlotModel &target)
{
    for(const auto& lm : source.layerModels())
    {
        // We can safely reuse the same id since it's in a different slot.
        auto& proc = lm.processModel();
        target.addLayerModel(
                    proc.cloneLayer(lm.id(),
                                    lm,
                                    &target));
    }
}

void SlotModel::addLayerModel(LayerModel* viewmodel)
{
    m_layerModels.insert(viewmodel);

    emit layerModelCreated(viewmodel->id());

    putToFront(viewmodel->id());
}

void SlotModel::deleteLayerModel(
        const Id<LayerModel>& layerId)
{
    auto& lm = layerModel(layerId);
    m_layerModels.remove(layerId);

    emit layerModelRemoved(layerId);


    if(!m_layerModels.empty())
    {
        putToFront((*m_layerModels.begin()).id());
    }
    else
    {
        m_frontLayerModelId.setVal({});
    }

    delete &lm;
}

void SlotModel::putToFront(
        const Id<LayerModel>& layerId)
{
    if(!layerId.val())
        return;

    if(layerId != m_frontLayerModelId)
    {
        m_frontLayerModelId = layerId;
        emit layerModelPutToFront(layerId);
    }
}

const Id<LayerModel>& SlotModel::frontLayerModel() const
{
    return m_frontLayerModelId;
}

LayerModel& SlotModel::layerModel(
        const Id<LayerModel>& layerModelId) const
{
    return m_layerModels.at(layerModelId);
}

void SlotModel::on_deleteSharedProcessModel(
        const Id<Process>& sharedProcessId)
{
    using namespace std;
    auto it = find_if(begin(m_layerModels),
                      end(m_layerModels),
                      [&sharedProcessId](const LayerModel& lm)
    {
        return lm.processModel().id() == sharedProcessId;
    });

    if(it != end(m_layerModels))
    {
        deleteLayerModel((*it).id());
    }
}

void SlotModel::setHeight(qreal arg)
{
    if(m_height != arg)
    {
        m_height = arg;
        emit heightChanged(arg);
    }
}

void SlotModel::setFocus(bool arg)
{
    if (m_focus == arg)
        return;

    m_focus = arg;
    emit focusChanged(arg);
}

ConstraintModel& SlotModel::parentConstraint() const
{
    return static_cast<ConstraintModel&>(*parent()->parent());
}

qreal SlotModel::height() const
{
    return m_height;
}

bool SlotModel::focus() const
{
    return m_focus;
}
