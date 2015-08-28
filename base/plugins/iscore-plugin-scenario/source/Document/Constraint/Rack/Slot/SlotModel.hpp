#pragma once
#include <ProcessInterface/LayerModel.hpp>
#include <iscore/tools/IdentifiedObjectMap.hpp>

#include <vector>

class RackModel;
class ConstraintModel;

class Process;
class LayerModel;

// Note : the SlotModel is assumed to be in a Rack, itself in a Constraint.
class SlotModel : public IdentifiedObject<SlotModel>
{
        Q_OBJECT

        Q_PROPERTY(qreal height
                   READ height
                   WRITE setHeight
                   NOTIFY heightChanged)

        Q_PROPERTY(bool focus
                   READ focus
                   WRITE setFocus
                   NOTIFY focusChanged)

    public:
        SlotModel(const Id<SlotModel>& id,
                  RackModel* parent);

        // Copy
        SlotModel(std::function<void(const SlotModel&, SlotModel&)> lmCopyMethod,
                  const SlotModel& source,
                  const Id<SlotModel>& id,
                  RackModel* parent);

        static void copyViewModelsInSameConstraint(const SlotModel&, SlotModel&);

        template<typename Impl>
        SlotModel(Deserializer<Impl>& vis, QObject* parent) :
            IdentifiedObject{vis, parent}
        {
            vis.writeTo(*this);
        }

        virtual ~SlotModel() = default;

        void addLayerModel(
                LayerModel*);
        void deleteLayerModel(
                const Id<LayerModel>& layerModelId);


         // A process is selected for edition when it is
         // the edited process when the interface is clicked.
        void putToFront(
                const Id<LayerModel>& layerId);
        const Id<LayerModel>& frontLayerModel() const;

        const auto& layerModels() const
        { return m_layerModels; }

        LayerModel& layerModel(
                const Id<LayerModel>& layerModelId) const;

        // A slot is always in a constraint
        ConstraintModel& parentConstraint() const;

        qreal height() const;
        bool focus() const;

    signals:
        void layerModelCreated(const Id<LayerModel>& layerModelId);
        void layerModelRemoved(const Id<LayerModel>& layerModelId);
        void layerModelPutToFront(const Id<LayerModel>& layerModelId);

        void heightChanged(qreal arg);
        void focusChanged(bool arg);

    public slots:
        void on_deleteSharedProcessModel(const Id<Process>& sharedProcessId);

        void setHeight(qreal arg);
        void setFocus(bool arg);

    private:
        Id<LayerModel> m_frontLayerModelId;
        IdContainer<LayerModel> m_layerModels;

        qreal m_height {200};
        bool m_focus{false};
};

/**
 * @brief parentConstraint Utility function to get the parent constraint of a process view model
 * @param lm Process view model pointer
 *
 * @return A pointer to the parent constraint if there is one, or nullptr.
 */
ConstraintModel* parentConstraint(LayerModel* lm);
