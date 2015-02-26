#pragma once
#include "Document/Constraint/ViewModels/AbstractConstraintPresenter.hpp"

class FullViewConstraintViewModel;
class FullViewConstraintView;
class BoxPresenter;
class BoxModel;


namespace iscore
{
    class SerializableCommand;
}
class ProcessPresenterInterface;

/**
 * @brief The FullViewConstraintPresenter class
 *
 * Présenteur : reçoit signaux depuis modèle et vue et présenteurs enfants.
 * Exemple : cas d'un process ajouté : le modèle reçoit la commande addprocess, émet un signal, qui est capturé par le présenteur qui va instancier le présenteur nécessaire en appelant la factory.
 */
class FullViewConstraintPresenter : public AbstractConstraintPresenter
{
        Q_OBJECT

    public:
        using view_model_type = FullViewConstraintViewModel;
        using view_type = FullViewConstraintView;

        FullViewConstraintPresenter (FullViewConstraintViewModel* viewModel,
                                     FullViewConstraintView* view,
                                     QObject* parent);
        virtual ~FullViewConstraintPresenter();

};

