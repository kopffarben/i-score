#pragma once
#include <vector>
#include <iscore/tools/EntityMap.hpp>

template<typename T>
std::vector<T*> shallow_copy(const IdContainer<T>& m)
{
    return {m.get().begin(), m.get().end()};
}

template<typename T>
std::vector<T*> shallow_copy(const EntityMap<T>& m)
{
    return shallow_copy(m.map());
}
