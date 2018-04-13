#pragma once

#include <memory>
#include <vector>
#include <functional>
#include <map>
#include <type_traits>
#include <iostream>

template <class T, std::size_t ChunkSize = 1>
struct Pool
{
    static_assert(ChunkSize > 0);

    std::map<std::size_t,std::size_t> h2i;
    std::vector<std::size_t> free_objects;
    std::vector<std::unique_ptr<T>> indexed_objects;
    std::vector<std::size_t> allocated_ids;

    void del_object(T *ptr_obj) {
        auto h = std::hash<T*>()(ptr_obj);
        auto i = h2i[h];
        std::cout << "Deleting object : {" << h << "," << i << "}" << std::endl;
        free_objects.push_back(i);
        indexed_objects[i] = std::unique_ptr<T>{ptr_obj};
    }

    using del_type = std::function<void(T *)>;
    del_type del_fun = std::bind(&Pool::del_object,this,std::placeholders::_1);;
    using del_ptr = std::unique_ptr<T, del_type>;

    using hash_t = std::hash<typename Pool<T, ChunkSize>::del_ptr>;
    static std::size_t hash(del_ptr& t) { return hash_t()(t); }
    std::map<std::size_t,del_ptr> allocated_objects;

  public:

    auto& get_object()
    {
        if (free_objects.empty())
        {
            free_objects.resize(ChunkSize);
            auto oldsize = indexed_objects.size();
            indexed_objects.resize(oldsize+ChunkSize);
            for(auto i = 0; i < ChunkSize; i++) {
                free_objects[oldsize + i] = oldsize + ChunkSize - i - 1;
                auto new_obj = std::make_unique<T>();
                h2i[std::hash<std::unique_ptr<T>>()(new_obj)] = oldsize + i;
                indexed_objects[oldsize + i] = std::move(new_obj);
            }
        } 
        auto i = free_objects.back();
//        std::cout << "getting object n : " << i << std::endl;        
        free_objects.pop_back();
        std::unique_ptr<T> available_object = std::move(indexed_objects[i]);
        allocated_objects.emplace(i,del_ptr{available_object.release(),del_fun});
        return allocated_objects[i];
    }

    void display_free() {
        std::cout << "free_objects : ";
        for(auto i : free_objects) std::cout << i << " ";
        std::cout << std::endl;
        std::cout << "hash objects : ";
        for(auto p : h2i) std::cout << " {" << p.first << "," << p.second << "}";
        std::cout << std::endl;
    }

    std::size_t size() {
        return indexed_objects.size() - free_objects.size();
    }

    std::size_t capacity() {
        return indexed_objects.size();
    }

    std::size_t get_i(del_ptr& t) { return h2i[hash(t)]; }
    
    void erase(del_ptr& t) { allocated_objects.erase(get_i(t)); t.reset(nullptr); }

    void obj_info(del_ptr& t) {
        std::cout << "value : " << *t << ", i : " << get_i(t) <<  ", ii : " << ", hash : " << hash(t) << std::endl;
    }

};