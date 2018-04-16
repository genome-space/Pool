#pragma once

#include <vector>
#include <map>
#include <iostream>
#include "UniqueProxy.hpp"

template <class T, std::size_t ChunkSize> 
struct Pool : public UniqueProxy<T, Pool<T,ChunkSize>>
{
    std::vector<std::size_t> free_objects;
    std::map<std::size_t, std::size_t> h2i;
    using super = UniqueProxy<T, Pool<T,ChunkSize>>;
    using del_ptr = typename super::del_ptr;
    std::vector<std::unique_ptr<T>> indexed_objects;


    void del_object(std::size_t h, T *ptr_obj)
    {
        auto i = h2i[h];
        free_objects.push_back(i);
        indexed_objects[i] = std::unique_ptr<T>{ptr_obj};
    }

    auto&& allocate()
    {
        if (free_objects.empty())
        {
            free_objects.resize(ChunkSize);
            auto oldsize = indexed_objects.size();
            indexed_objects.resize(oldsize + ChunkSize);

            for (auto i = 0; i < ChunkSize; i++)
            {
                free_objects[oldsize + i] = oldsize + ChunkSize - i - 1;
                auto new_obj = std::unique_ptr<T>{this->get_object().release()};
                h2i[std::hash<std::unique_ptr<T>>()(new_obj)] = oldsize + i;
                indexed_objects[oldsize + i] = std::move(new_obj);
            }
        }
        auto i = free_objects.back();
        free_objects.pop_back();
        std::unique_ptr<T> available_object = std::move(indexed_objects[i]);
        return del_ptr{available_object.release(),_del_fun};
    }

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

    std::size_t get_i(del_ptr &t) { return h2i[hash(t)]; }

    void obj_info(del_ptr &t)
    {
        std::cout << "value : " << *t << ", i : " << get_i(t) << ", ii : "
                  << ", hash : " << hash(t) << std::endl;
    }
};
