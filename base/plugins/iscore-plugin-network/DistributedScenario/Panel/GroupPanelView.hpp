#pragma once
#include <iscore/plugins/panel/PanelView.hpp>
class GroupManager;
class Session;

namespace iscore
{
    class View;
}

class GroupPanelView : public iscore::PanelView
{
    public:
        GroupPanelView(QObject* v);

        QWidget* getWidget() override;
        Qt::DockWidgetArea defaultDock() const override;
        int priority() const override;
        QString prettyName() const override;

        void setView(const GroupManager* mgr,
                     const Session* session);

        void setEmptyView();

    private:
        QWidget* m_widget{}, *m_subWidget{};
};