#include "Pool.hpp"
#include <iostream>

using namespace std;

// template<class T>
// size_t myhash(T&& t) { return hash<T>()(t); }

int main()
{
    Pool<int, 10> pool;
    cout << "Entering bloc1" << endl;
    {
        cout << "Getting our first object" << endl;
        auto&& i1 = pool.allocate();
        cout << "Pool size : " << pool.size() << " capacity : " << pool.capacity() << endl;
    }
    //     *i1 = 2;
    //     pool.obj_info(i1);
    //     pool.display_free();
    //     cout << "Getting our second object" << endl;
    //     auto& i2 = pool.allocate();
    //     pool.display_free();
    //     cout << "Pool size : " << pool.size() << " capacity : " << pool.capacity() << endl;
    //     *i2 = 3;
    //     pool.obj_info(i2);
    //     cout << "Exiting from bloc1" << endl;
    //     // pool.erase(i1);
    //     // pool.erase(i2);
    // }
    // cout << "Pool size : " << pool.size() << " capacity : " << pool.capacity() << endl;
    //     cout << "Getting our first object" << endl;
    //     auto& i1 = pool.get_object();
    //     cout << "Pool size : " << pool.size() << " capacity : " << pool.capacity() << endl;
    //     *i1 = 2;
    //     pool.obj_info(i1);
    //     pool.display_free();
    //     cout << "Getting our second object" << endl;
    //     auto& i2 = pool.get_object();
    //     pool.display_free();
    //     cout << "Pool size : " << pool.size() << " capacity : " << pool.capacity() << endl;
    //     *i2 = 3;
    //     pool.obj_info(i2);
    //     cout << "Getting second object by index " << endl;
    //     cout << "Exiting..." << endl;
    //     cout.flush();
}