#pragma once
#include <QStateMachine>
#include <QPointF>
#include <QVector>
#include <iscore/command/OngoingCommandManager.hpp>
#include <iscore/selection/SelectionDispatcher.hpp>

class CurveModel;
class CurveView;
class CurvePointView;
class CurveSegmentView;
class CurveStateMachine;
class CurvePresenter : public QObject
{
        Q_OBJECT
    public:
        enum class AddPointBehaviour
        { LinearBefore, LinearAfter, DuplicateSegment };
        CurvePresenter(CurveModel*, CurveView*);

        const QVector<CurvePointView*>& points() const
        { return m_points; }
        const QVector<CurveSegmentView*>& segments() const
        { return m_segments; }

        CurveModel* model() const;
        CurveView& view() const;

        // Taken from the view. First set this,
        // then send signals to the state machine.
        QPointF pressedPoint() const;

        void setRect(const QRectF& rect);

        bool lockBetweenPoints() const;
        void setLockBetweenPoints(bool lockBetweenPoints);

        bool suppressOnOverlap() const;
        void setSuppressOnOverlap(bool suppressOnOverlap);

        bool stretchBothBounds() const;
        void setStretchBothBounds(bool stretchBothBounds);

        AddPointBehaviour addPointBehaviour() const;
        void setAddPointBehaviour(const AddPointBehaviour &addPointBehaviour);

    private:
        void removeSelection();
        void setPos(CurvePointView*);
        void setPos(CurveSegmentView*);
        void setupSignals();
        void setupView();
        void setupStateMachine();
        CurveStateMachine* m_sm{};

        // Data relative to the current state of the view
        CurveSegmentView* m_currentSegmentView{};
        CurvePointView* m_currentPointView{};

        CurveModel* m_model{};
        CurveView* m_view{};

        QVector<CurvePointView*> m_points;
        QVector<CurveSegmentView*> m_segments;

        // Boolean values that keep the editing state. Should they go here ?
        // Maybe in the settings, instead ?
        bool m_lockBetweenPoints{};
        bool m_suppressOnOverlap{true};
        bool m_stretchBothBounds{};
        AddPointBehaviour m_addPointBehaviour{};

        CommandDispatcher<> m_commandDispatcher;
        iscore::SelectionDispatcher m_selectionDispatcher;
};