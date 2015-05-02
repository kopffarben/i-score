#pragma once

#include <ProcessInterface/ProcessViewModelInterface.hpp>

class AutomationModel;
class AutomationViewModel : public ProcessViewModelInterface
{
    public:
        AutomationViewModel(AutomationModel& model,
                            const id_type<ProcessViewModelInterface>& id,
                            QObject* parent);

        // Copy
        AutomationViewModel(const AutomationViewModel& source,
                            AutomationModel& model,
                            const id_type<ProcessViewModelInterface>& id,
                            QObject* parent);

        // Load
        template<typename Impl>
        AutomationViewModel(Deserializer<Impl>& vis,
                            AutomationModel& model,
                            QObject* parent) :
            ProcessViewModelInterface {vis, model, parent}
        {
            vis.writeTo(*this);
        }

        virtual ProcessViewModelPanelProxy* make_panelProxy() override;
        virtual void serialize(const VisitorVariant&) const override;

        const AutomationModel& model();
};
