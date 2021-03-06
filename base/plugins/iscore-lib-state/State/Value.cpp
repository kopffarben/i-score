#include "Value.hpp"
#include <State/ValueConversion.hpp>
#include <ossia/editor/value/value.hpp>

namespace State {
bool Value::operator==(const Value& m) const
{
    return val == m.val;
}

bool Value::operator!=(const Value& m) const
{
    return val != m.val;
}

ValueImpl::ValueImpl(impulse_t v): m_variant{v} { }
ValueImpl::ValueImpl(int v): m_variant{v} { }
ValueImpl::ValueImpl(float v): m_variant{v} { }
ValueImpl::ValueImpl(double v): m_variant{(float)v} { }
ValueImpl::ValueImpl(bool v): m_variant{v} { }
ValueImpl::ValueImpl(const QString& v): m_variant{v.toStdString()} { }
ValueImpl::ValueImpl(std::string v): m_variant{std::move(v)} { }
ValueImpl::ValueImpl(QChar v): m_variant{v.toLatin1()} { }
ValueImpl::ValueImpl(char v): m_variant{v} { }
ValueImpl::ValueImpl(vec2f v): m_variant{v} { }
ValueImpl::ValueImpl(vec3f v): m_variant{v} { }
ValueImpl::ValueImpl(vec4f v): m_variant{v} { }
ValueImpl::ValueImpl(tuple_t v): m_variant{std::move(v)} { }

ValueImpl& ValueImpl::operator=(impulse_t v) { m_variant = v; return *this; }
ValueImpl& ValueImpl::operator=(int v) { m_variant = v; return *this;  }
ValueImpl& ValueImpl::operator=(float v) { m_variant = v; return *this;  }
ValueImpl& ValueImpl::operator=(double v) { m_variant = (float)v; return *this;  }
ValueImpl& ValueImpl::operator=(bool v) { m_variant = v; return *this;  }
ValueImpl& ValueImpl::operator=(const QString& v) { m_variant = v.toStdString(); return *this;  }
ValueImpl& ValueImpl::operator=(QString&& v) { m_variant = std::move(v).toStdString(); return *this;  }
ValueImpl& ValueImpl::operator=(const std::string& v) { m_variant = v; return *this;  }
ValueImpl& ValueImpl::operator=(std::string&& v) { m_variant = std::move(v); return *this;  }
ValueImpl& ValueImpl::operator=(QChar v) { m_variant = v.toLatin1(); return *this;  }
ValueImpl& ValueImpl::operator=(char v) { m_variant = v; return *this;  }
ValueImpl& ValueImpl::operator=(vec2f v) { m_variant = v; return *this;  }
ValueImpl& ValueImpl::operator=(vec3f v) { m_variant = v; return *this;  }
ValueImpl& ValueImpl::operator=(vec4f v) { m_variant = v; return *this;  }
ValueImpl& ValueImpl::operator=(const tuple_t& v) { m_variant = v; return *this;  }
ValueImpl& ValueImpl::operator=(tuple_t&& v) { m_variant = std::move(v); return *this;  }


bool ValueImpl::operator ==(const ValueImpl& other) const
{
    return m_variant == other.m_variant;
}

bool ValueImpl::operator !=(const ValueImpl& other) const
{
    return m_variant != other.m_variant;
}

bool ValueImpl::isNumeric() const
{
    auto t = m_variant.which();
    return t == 1 || t == 2;
}

bool ValueImpl::isValid() const
{
    return m_variant.which() != m_variant.npos;
}

bool ValueImpl::isArray() const
{
    return is<tuple_t>()
            || is<vec2f>()
            || is<vec3f>()
            || is<vec4f>();
}

ISCORE_LIB_STATE_EXPORT QDebug& operator<<(QDebug& s, const Value& m)
{
    s << convert::textualType(m) << convert::toPrettyString(m);
    return s;
}

ossia::value toOSSIAValue(const State::ValueImpl& val)
{
    struct {
            using return_type = ossia::value;
            return_type operator()() const { return ossia::value{}; }
            return_type operator()(const State::impulse_t&) const { return ossia::Impulse{}; }
            return_type operator()(int v) const { return v; }
            return_type operator()(float v) const { return v; }
            return_type operator()(bool v) const { return v; }
            return_type operator()(const QString& v) const { return v.toStdString(); }
            return_type operator()(QChar v) const { return v.toLatin1(); }
            return_type operator()(const std::string& v) const { return v; }
            return_type operator()(char v) const { return v; }
            return_type operator()(const State::vec2f& v) const { return v; }
            return_type operator()(const State::vec3f& v) const { return v; }
            return_type operator()(const State::vec4f& v) const { return v; }
            return_type operator()(const State::tuple_t& v) const
            {
                ossia::Tuple ossia_tuple;
                ossia_tuple.reserve(v.size());
                for(const auto& tuple_elt : v)
                {
                    ossia_tuple.push_back(eggs::variants::apply(*this, tuple_elt.impl()));
                }

                return ossia_tuple;
            }
    } visitor{};

    return ossia::apply(visitor, val.impl());
}

Value fromOSSIAValue(const ossia::value& val)
{
    struct {
            using return_type = State::Value;
            return_type operator()(ossia::Destination) const { return {}; }
            return_type operator()(ossia::Impulse) const { return State::Value::fromValue(State::impulse_t{}); }
            return_type operator()(ossia::Int v) const { return State::Value::fromValue(v); }
            return_type operator()(ossia::Float v) const { return State::Value::fromValue(v); }
            return_type operator()(ossia::Bool v) const { return State::Value::fromValue(v); }
            return_type operator()(ossia::Char v) const { return State::Value::fromValue(v); }
            return_type operator()(const ossia::String& v) const { return State::Value::fromValue(QString::fromStdString(v)); }
            return_type operator()(ossia::Vec2f v) const { return State::Value::fromValue(v); }
            return_type operator()(ossia::Vec3f v) const { return State::Value::fromValue(v); }
            return_type operator()(ossia::Vec4f v) const { return State::Value::fromValue(v); }
            return_type operator()(const ossia::Tuple& v) const
            {
                State::tuple_t tuple;

                tuple.reserve(v.size());
                for (const auto & e : v)
                {
                    tuple.push_back(fromOSSIAValue(e).val); // TODO REVIEW THIS
                }

                return State::Value::fromValue(std::move(tuple));
            }
    } visitor{};

    if(val.valid())
        return eggs::variants::apply(visitor, val.v);
    return {};
}

}
