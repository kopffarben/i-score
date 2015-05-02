#pragma once

#include <QMap>
#include "source/Process/AbstractScenarioViewModel.hpp"
class ScenarioModel;
class BoxModel;
class TemporalConstraintViewModel;
class TemporalScenarioPresenter;

class ConstraintModel;


class TemporalScenarioViewModel : public AbstractScenarioViewModel
{
        Q_OBJECT

        TemporalScenarioPresenter* m_presenter{};
    public:
        // TODO UGLYY
        void setPresenter(TemporalScenarioPresenter* p)
        { m_presenter = p; }
        TemporalScenarioPresenter* presenter() const
        { return m_presenter; }

        using model_type = ScenarioModel;
        using constraint_view_model_type = TemporalConstraintViewModel;
        // using event_type = TemporalEventViewModel;

        TemporalScenarioViewModel(const id_type<ProcessViewModelInterface>& id,
                                  const QMap<id_type<ConstraintModel>, id_type<AbstractConstraintViewModel>>& constraintIds,
                                  ScenarioModel& model,
                                  QObject* parent);

        // Copy
        TemporalScenarioViewModel(const TemporalScenarioViewModel& source,
                                  const id_type<ProcessViewModelInterface>& id,
                                  ScenarioModel& model,
                                  QObject* parent);

        // Load
        template<typename Impl>
        TemporalScenarioViewModel(Deserializer<Impl>& vis,
                                  ScenarioModel& model,
                                  QObject* parent) :
            AbstractScenarioViewModel {vis, model, parent}
        {
            vis.writeTo(*this);
        }

        virtual ProcessViewModelPanelProxy* make_panelProxy() override;

        virtual ~TemporalScenarioViewModel() = default;

        virtual void serialize(const VisitorVariant&) const override;

        virtual void makeConstraintViewModel(id_type<ConstraintModel> constraintModelId,
                                             id_type<AbstractConstraintViewModel> constraintViewModelId) override;

        void addConstraintViewModel(constraint_view_model_type* constraint_view_model);

    public slots:
        virtual void on_constraintRemoved(id_type<ConstraintModel> constraintId) override;

};
