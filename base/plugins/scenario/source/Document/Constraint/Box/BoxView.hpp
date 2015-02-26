#pragma once
#include <QGraphicsObject>

class BoxView : public QGraphicsObject
{
        Q_OBJECT

    public:
        BoxView (QGraphicsObject* parent);
        virtual ~BoxView() = default;


        virtual QRectF boundingRect() const override;
        virtual void paint (QPainter* painter,
                            const QStyleOptionGraphicsItem* option,
                            QWidget* widget) override;

        void setText (QString s)
        {
            m_text = s;
            update();
        }

        void setHeight (int height)
        {
            prepareGeometryChange();
            m_height = height;
        }

        void setWidth (int width)
        {
            prepareGeometryChange();
            m_width = width;
        }

    private:
        int m_height {};
        int m_width {};
        QString m_text {"Box"};
};
