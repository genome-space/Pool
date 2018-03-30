#include <memory>
#include <vector>
#include <functional>
#include <map>

template <class T>
struct Pool
{
    std::vector<std::unique_ptr<T>> free_objects;
    std::size_t _allocated = 0;
    std::size_t _inuse = 0;
    std::map<std::size_t,std::size_t> h2i;

    auto extract_from_free_list()
    {
        std::unique_ptr<T> available_obj;
        if (!free_objects.empty())
        {
            available_obj = std::move(free_objects.back());
            free_objects.pop_back();
        }
        return available_obj;
    }

    void del_object(T *ptr_obj) {
        free_objects.emplace_back(ptr_obj);
        _inuse--;
    }

    using del_type = std::function<void(T *)>;
    del_type del_fun = std::bind(&Pool::del_object,this,std::placeholders::_1);;
    using del_ptr = std::unique_ptr<T, del_type>;

  public:
    using hash_t = std::hash<typename Pool<T>::del_ptr>;
    static std::size_t hash(del_ptr& t) { return hash_t()(t); }

    auto get_object()
    {
        auto free_object = extract_from_free_list();
        _inuse++;
        if (free_object)
        {
            return del_ptr{free_object.release(), del_fun};
        }
        else
        {
            auto ptr = del_ptr{new T(), del_fun};
            auto hptr = hash(ptr);
            h2i[hptr] = _allocated;
            _allocated++;
            return ptr;
        }
    }

    auto size()
    {
        return free_objects.size();
    }

    auto capacity()
    {
        return free_objects.capacity();
    }

    auto allocated() {
        return _allocated;
    }

    auto inuse() {
        return _inuse;
    }

    auto get_i(del_ptr& t) { return h2i[hash(t)]; }

    static void release(del_ptr& t) { t.reset(nullptr); }
};