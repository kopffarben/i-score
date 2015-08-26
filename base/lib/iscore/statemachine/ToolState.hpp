#pragma once
#include <QStateMachine>
class QGraphicsItem;
class QGraphicsScene;

class ToolState : public QState
{
    public:
        void start();

    protected:
        ToolState(const QGraphicsScene& scene, QState*);

        virtual void on_pressed() = 0;
        virtual void on_moved() = 0;
        virtual void on_released() = 0;

        QGraphicsItem* itemUnderMouse(const QPointF& point) const;

        const QGraphicsScene& scene() const { return m_scene; }
        QStateMachine& localSM() { return m_localSM; }

    private:
        const QGraphicsScene& m_scene;
        QStateMachine m_localSM;
};
