#pragma once

#include <functional>
#include <memory>
#include <tuple>

template <class T, class Manager>
struct UniqueProxy
{
    using del_fun_type = std::function<void(std::size_t, T *)>;

    void del_object(T *ptr_obj)
    {
        auto h = std::hash<T *>()(ptr_obj);
        static_cast<Manager *>(this)->del_object(h, ptr_obj);
    }

    using del_internalfun_type = std::function<void(T *)>;

    using del_ptr = std::unique_ptr<T, del_internalfun_type>;

    const del_internalfun_type _del_fun = std::bind(&UniqueProxy::del_object, this, std::placeholders::_1);

    using hash_t = std::hash<typename UniqueProxy<T, Manager>::del_ptr>;

    del_ptr&& get_object()
    {
        del_ptr&& allocated_object = del_ptr{new T(), _del_fun};
        return std::move(allocated_object);
    }

   
    auto& allocate()
    {
        return static_cast<const Manager *>(this)->allocate();
    }

    static std::size_t hash(del_ptr &t) { return hash_t()(t); }
};