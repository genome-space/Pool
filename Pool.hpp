#pragma once

#include <vector>
#include <map>

template <class T, std::size_t ChunkSize>
class Pool
{

  private:
    std::vector<std::size_t> free_objects;
    std::map<std::size_t, std::size_t> h2i;
    using del_ptr = typename UniqueProxy<T>::del_ptr;
    std::vector<del_ptr> indexed_objects;

    auto &get_object()
    {
        auto new_obj = T();
        auto &unique_obj = del_ptr{&new_obj, _del_fun};
        return std::move(unique_obj);
    }

    void del_object(std::size_t h, T *ptr_obj)
    {
        auto i = h2i[h];
        free_objects.push_back(i);
        indexed_objects[i] = del_ptr{ptr_obj};
    }

    del_ptr &pool_allocate()
    {
        if (free_objects.empty())
        {
            free_objects.resize(ChunkSize);
            auto oldsize = indexed_objects.size();
            indexed_objects.resize(oldsize + ChunkSize);

            for (auto i = 0; i < ChunkSize; i++)
            {
                free_objects[oldsize + i] = oldsize + ChunkSize - i - 1;
                auto &new_obj = _unique_proxy.get_object();
                h2i[UniqueProxy<T>::hash(new_obj)] = oldsize + i;
            }
        }
        auto i = free_objects.back();
        free_objects.pop_back();
        return std::move(indexed_objects[i]);
    }

  public:

    void display_free()
    {
        std::cout << "free_objects : ";
        for (auto i : free_objects)
            std::cout << i << " ";
        std::cout << std::endl;
        std::cout << "hash objects : ";
        for (auto p : h2i)
            std::cout << " {" << p.first << "," << p.second << "}";
        std::cout << std::endl;
    }

    std::size_t size()
    {
        return indexed_objects.size() - free_objects.size();
    }

    std::size_t capacity()
    {
        return indexed_objects.size();
    }

    std::size_t get_i(del_ptr &t) { return h2i[UniqueProxy<T>::hash(t)]; }

    void obj_info(del_ptr &t)
    {
        std::cout << "value : " << *t << ", i : " << get_i(t) << ", ii : "
                  << ", hash : " << hash(t) << std::endl;
    }
};
