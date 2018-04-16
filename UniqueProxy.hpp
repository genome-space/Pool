#pragma once

#include <functional>
#include <memory>
#include <tuple>

template <class T, class InitManager, class... PipelinedManagers>
class UniqueProxy
{
  public:
    using del_fun_type = std::function<void(std::size_t, T *)>;

  private:
    std::tuple<InitManager, class... PipelinedManagers> _managers;

    template<std::size_t... Is>
    void call_deleters(std::size_t h, T *ptr_obj, std::index_sequence<Is...>) {
        (std::get<Is>(_managers).delete(h, ptr_obj),...)
    }

    void del_object(T *ptr_obj)
    {
        auto h = std::hash<T *>()(ptr_obj);
        call_deleters(h, ptr_obj, std::index_sequence_for<InitManager, PipelinedManagers...>)
    }

    using del_internalfun_type = std::function<void(T *)>;

  public:
    using del_ptr = std::unique_ptr<T, del_internalfun_type>;

  private:
    const del_internalfun_type &_del_fun = std::bind(&UniqueProxy::del_object, this, std::placeholders::_1);

    using hash_t = std::hash<typename UniqueProxy<T>::del_ptr>;

  public:

    template<std::size_t... Is>
    auto& call_allocaters(std::size_t h, del_ptr& t, std::index_sequence<Is, Rest...>) {
        auto& res = std::get<Is>(_managers).allocate(h, t);
        return call_allocaters(h, t, std::index_sequence<Rest...>);
    }
    
    template<>
    auto& call_allocaters(std::size_t h, del_ptr& t, std::index_sequence<>) {
        return t;
    }

    auto &allocate()
    {
        del_ptr& t = std::get<0>(_managers).allocate()
        std::size_t h = hash_t()(t);
        return call_allocaters(h, t, std::index_sequence_for<PipelinedManagers...>);
    }

    static std::size_t hash(del_ptr &t) { return hash_t()(t); }
};