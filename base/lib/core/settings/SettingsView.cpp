#include <core/settings/SettingsView.hpp>
#include <interface/settingsdelegate/SettingsDelegatePresenterInterface.hpp>
#include <interface/settingsdelegate/SettingsDelegateViewInterface.hpp>
#include <QListWidgetItem>
#include <QStyle>
using namespace iscore;


SettingsView::SettingsView (QWidget* parent) :
    QDialog {parent}
{
    {
        auto centerWidg = new QWidget{this};
        {
            m_hboxLayout->addWidget (m_settingsList);
            m_hboxLayout->addWidget (m_stackedWidget);
            centerWidg->setLayout (m_hboxLayout);
        }

        m_vertLayout->addWidget (centerWidg);
        m_vertLayout->addWidget (m_buttons);
        this->setLayout (m_vertLayout);
    }

    connect (m_settingsList, SIGNAL (currentRowChanged (int) ),
    m_stackedWidget, SLOT (setCurrentIndex (int) ) );

    connect (m_buttons, &QDialogButtonBox::accepted,
    this,	   &SettingsView::accept);
    connect (m_buttons, &QDialogButtonBox::rejected,
    this,	   &SettingsView::reject);
}

void SettingsView::addSettingsView (SettingsDelegateViewInterface* view)
{
    view->setParent (this);
    QListWidgetItem* it = new QListWidgetItem {view->getPresenter()->settingsIcon(),
                                               view->getPresenter()->settingsName(),
                                               m_settingsList
                                              };
    it->setSizeHint (QSize {0, 30});
    m_settingsList->addItem (it);
    m_stackedWidget->addWidget (view->getWidget() );

    m_pluginViews.insert (view);
}
