#ifndef COMMON_BASE_STRICT_BOOL_HPP
#define COMMON_BASE_STRICT_BOOL_HPP
#pragma once

#include "common/base/static_assert.h"
#include "common/base/type_traits.hpp"

namespace base {

class strict_bool
{
    typedef bool (strict_bool::*SafeBool)() const;

public:
    strict_bool() : m_value(false) {}

    template <typename T>
    strict_bool(T src,
                typename TypeTraits::EnableIf<TypeTraits::IsSame<T, bool>::Value>::Type* dummy = 0) :
        m_value(src)
    {
    }

    strict_bool& operator=(bool src)
    {
        m_value = src;
        return *this;
    }

    // operator SafeBool() const { return m_value ? &strict_bool::operator! : 0; }
    template <typename T>
    operator T() const
    {
//        typedef typename TypeTraits::EnableIf<TypeTraits::IsSame<T, bool>::Value>::Type Type;
        return value();
    }

    bool operator!() const
    {
        return !m_value;
    }
    bool value() const
    {
        return m_value;
    }

    template <typename T>
    typename TypeTraits::EnableIf<TypeTraits::IsSame<T, bool>::Value, bool>::Type
    operator==(T rhs)
    {
        return value() == rhs;
    }

    bool operator==(strict_bool rhs) const
    {
        return value() == rhs.value();
    }

    template <typename T>
    typename TypeTraits::EnableIf<TypeTraits::IsSame<T, bool>::Value, bool>::Type
    operator!=(T rhs)
    {
        return value() != rhs;
    }

    bool operator!=(strict_bool rhs) const
    {
        return value() != rhs.value();
    }

private:
    bool m_value;
};

template <typename T>
typename TypeTraits::EnableIf<TypeTraits::IsSame<T, bool>::Value, bool>::Type
operator==(T lhs, const strict_bool& rhs)
{
    return rhs.value() == lhs;
}

template <typename T>
typename TypeTraits::EnableIf<TypeTraits::IsSame<T, bool>::Value, bool>::Type
operator!=(T lhs, const strict_bool& rhs)
{
    return rhs.value() != lhs;
}

} // end namespace base

using ::base::strict_bool;

#endif // COMMON_BASE_STRICT_BOOL_HPP
