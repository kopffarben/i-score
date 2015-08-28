#pragma once
#include "CurveSegmentFactory.hpp"

class LinearCurveSegmentFactory : public CurveSegmentFactory
{
    public:
        QString name() const override;

        CurveSegmentModel *make(
                const Id<CurveSegmentModel>& id,
                QObject* parent) override;

        CurveSegmentModel *load(
                const VisitorVariant& vis,
                QObject* parent) override;
};
