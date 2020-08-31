#pragma once
#include <vector>
#include <utility>
#include <functional>

template <typename T>
class sparestack {

public:
// returns the position the item was inserted into
    template <typename Callable>
    std::pair<std::size_t, T&> push(T in, const Callable& callable)
    {
        if (spareIds.size() == 0) {

            _data.push_back(in);
        } else {

            int accessElement = spareIds.front();

            _data.at(accessElement - 1) = in;
            spareIds.pop_back();
            callable(accessElement - 1, in);
            return {accessElement - 1, in };
        }
    };

    std::pair<std::size_t, T&> push(T in){
        return push(in, [](auto a, auto b){ return;});
    }

private:
    std::vector<T> _data;
    std::vector<int> spareIds; // index of spare slots to fill ids
};
