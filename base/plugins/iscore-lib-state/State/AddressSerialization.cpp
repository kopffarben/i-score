#include <iscore/serialization/DataStreamVisitor.hpp>
#include <iscore/serialization/JSONVisitor.hpp>
#include <iscore/tools/VariantSerialization.hpp>
#include <ossia/editor/dataspace/dataspace_visitors.hpp>
#include <QDataStream>
#include <QtGlobal>
#include <QJsonObject>
#include <QJsonValue>
#include <QString>
#include <QStringList>

#include "Address.hpp"

/// AccessorVector ///
template<>
ISCORE_LIB_STATE_EXPORT void Visitor<Reader<DataStream>>::readFrom(const State::AccessorVector& a)
{
    int32_t n = a.size();
    m_stream << n;
    for(int32_t i = 0; i < n; i++)
        m_stream << (int32_t)a[i];

    insertDelimiter();
}

template<>
ISCORE_LIB_STATE_EXPORT void Visitor<Writer<DataStream>>::writeTo(State::AccessorVector& a)
{
    int32_t size;
    m_stream >> size;
    for(int i = 0; i < size; i++)
    {
        int32_t t;
        m_stream >> t;
        a.push_back(t);
    }

    checkDelimiter();
}


/// Address ///
template<>
ISCORE_LIB_STATE_EXPORT void Visitor<Reader<DataStream>>::readFrom(const State::Address& a)
{
    m_stream << a.device << a.path;
    insertDelimiter();
}

template<>
ISCORE_LIB_STATE_EXPORT void Visitor<Reader<JSONObject>>::readFrom(const State::Address& a)
{
    m_obj[strings.Device] = a.device;
    m_obj[strings.Path] = a.path.join('/');
}

template<>
ISCORE_LIB_STATE_EXPORT void Visitor<Writer<DataStream>>::writeTo(State::Address& a)
{
    m_stream >> a.device >> a.path;
    checkDelimiter();
}

template<>
ISCORE_LIB_STATE_EXPORT void Visitor<Writer<JSONObject>>::writeTo(State::Address& a)
{
    a.device = m_obj[strings.Device].toString();

    auto path = m_obj[strings.Path].toString();

    if(!path.isEmpty())
        a.path = m_obj[strings.Path].toString().split('/');
}


/// AddressQualifiers ///
template<>
ISCORE_LIB_STATE_EXPORT void Visitor<Reader<DataStream>>::readFrom(const ossia::destination_qualifiers& a)
{
    m_stream << a.accessors << a.unit;
}

template<>
ISCORE_LIB_STATE_EXPORT void Visitor<Reader<JSONObject>>::readFrom(const ossia::destination_qualifiers& a)
{
    m_obj["Accessors"] = toJsonValueArray(a.accessors);
    m_obj[strings.Unit] = QString::fromStdString(ossia::get_pretty_unit_text(a.unit));
}

template<>
ISCORE_LIB_STATE_EXPORT void Visitor<Writer<DataStream>>::writeTo(ossia::destination_qualifiers& a)
{
    m_stream >> a.accessors >> a.unit;
}

template<>
ISCORE_LIB_STATE_EXPORT void Visitor<Writer<JSONObject>>::writeTo(ossia::destination_qualifiers& a)
{
    auto arr = m_obj["Accessors"].toArray();
    for(auto v : arr)
    {
        a.accessors.push_back(v.toInt());
    }
    a.unit = ossia::parse_pretty_unit(m_obj[strings.Unit].toString().toStdString());
}


/// AddressAccessor ///
template<>
ISCORE_LIB_STATE_EXPORT void Visitor<Reader<DataStream>>::readFrom(const State::AddressAccessor& rel)
{
    m_stream << rel.address << rel.qualifiers;

    insertDelimiter();
}

template<>
ISCORE_LIB_STATE_EXPORT void Visitor<Reader<JSONObject>>::readFrom(const State::AddressAccessor& rel)
{
    m_obj[strings.address] = toJsonObject(rel.address);
    readFrom(rel.qualifiers);
}

template<>
ISCORE_LIB_STATE_EXPORT void Visitor<Writer<DataStream>>::writeTo(State::AddressAccessor& rel)
{
    m_stream >> rel.address >> rel.qualifiers;

    checkDelimiter();
}

template<>
ISCORE_LIB_STATE_EXPORT void Visitor<Writer<JSONObject>>::writeTo(State::AddressAccessor& rel)
{
    fromJsonObject(m_obj[strings.address], rel.address);
    writeTo(rel.qualifiers);
}



/// AddressAccessorHead ///
template<>
ISCORE_LIB_STATE_EXPORT void Visitor<Reader<DataStream>>::readFrom(const State::AddressAccessorHead& rel)
{
    m_stream << rel.name << rel.qualifiers;

    insertDelimiter();
}

template<>
ISCORE_LIB_STATE_EXPORT void Visitor<Reader<JSONObject>>::readFrom(const State::AddressAccessorHead& rel)
{
    m_obj[strings.Name] = rel.name;
    readFrom(rel.qualifiers);
}

template<>
ISCORE_LIB_STATE_EXPORT void Visitor<Writer<DataStream>>::writeTo(State::AddressAccessorHead& rel)
{
    m_stream >> rel.name >> rel.qualifiers;

    checkDelimiter();
}

template<>
ISCORE_LIB_STATE_EXPORT void Visitor<Writer<JSONObject>>::writeTo(State::AddressAccessorHead& rel)
{
    rel.name = m_obj[strings.Name].toString();
    writeTo(rel.qualifiers);
}


