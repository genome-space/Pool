#include "Pool.hpp"
#include <iostream>

using namespace std;

// template<class T>
// size_t myhash(T&& t) { return hash<T>()(t); }

int main()
{
    Pool<int> pool;
    {
        cout << "Getting our first object" << endl;
        auto i1 = pool.get_object();
        *i1 = 2;
        cout << "what is in i1 : " << *i1 << endl;
        cout << "hash i1 is " << Pool<int>::hash(i1) << endl;
        cout << "Pool size : " << pool.size() << " capacity : " << pool.capacity() << " allocated : " << pool.allocated() << " inuse : " << pool.inuse() << endl;
        cout << "Exiting from bloc1" << endl;
    }
    cout << "Pool size : " << pool.size() << " capacity : " << pool.capacity() << " allocated : " << pool.allocated() << " inuse : " << pool.inuse() << endl;
    cout << "Getting another object" << endl;
    auto i1 = pool.get_object();
    *i1 = 4;
    cout << "what is in i1 : " << *i1 << endl;
    cout << "hash i1 is " << Pool<int>::hash(i1) << endl;
    cout << "Pool size : " << pool.size() << " capacity : " << pool.capacity() << " allocated : " << pool.allocated() << " inuse : " << pool.inuse() << endl;
    cout << "deleting i1" << endl;
    // i1.reset(nullptr);
    Pool<int>::release(i1);
    cout << "Pool size : " << pool.size() << " capacity : " << pool.capacity() << " allocated : " << pool.allocated() << " inuse : " << pool.inuse() << endl;
    auto i2 = pool.get_object();
    cout << "Getting another object" << endl;
    cout << "Pool size : " << pool.size() << " capacity : " << pool.capacity() << " allocated : " << pool.allocated() << " inuse : " << pool.inuse() << endl;
    cout << "hash i2 is " << Pool<int>::hash(i2) << endl;
    cout << "index of i2 : " << pool.get_i(i2) << endl;
    cout << "Getting another object" << endl;
    auto i3 = pool.get_object();
    cout << "hash i3 is " << Pool<int>::hash(i3) << endl;
    cout << "index of i3 : " << pool.get_i(i3) << endl;
}