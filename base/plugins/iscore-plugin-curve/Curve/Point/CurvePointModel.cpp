#include "CurvePointModel.hpp"


const Id<CurveSegmentModel>& CurvePointModel::following() const
{
    return m_following;
}

void CurvePointModel::setFollowing(const Id<CurveSegmentModel> &following)
{
    m_following = following;
}

CurvePoint CurvePointModel::pos() const
{
    return m_pos;
}

void CurvePointModel::setPos(const CurvePoint &pos)
{
    m_pos = pos;
}


const Id<CurveSegmentModel> &CurvePointModel::previous() const
{
    return m_previous;
}

void CurvePointModel::setPrevious(const Id<CurveSegmentModel> &previous)
{
    m_previous = previous;
}
