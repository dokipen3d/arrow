#pragma once
#include <functional>
#include <utility>
#include <vector>

template <typename T>
class sparestack {

public:

    void reserve(std::size_t count) {
        _data.reserve(count);
    }

    // returns the position the item was inserted into
    template <typename U, typename Callable>
    std::size_t push(U&& in, const Callable& callable)
    {
        if (spareIds.size() == 0) {

            _data.push_back(std::forward<U>(in));
            return _data.size()-1;
        } else {

            int accessElement = spareIds.front();

            _data.at(accessElement - 1) = std::forward<U>(in);
            spareIds.pop_back();
            callable(accessElement - 1, std::forward<U>(in));
            return accessElement - 1;
        }
    }

    std::size_t size()
    {
        return _data.size() - spareIds.size();
    }

    void eraseAt(std::size_t position)
    {
        if (position < _data.size()) { // if position is within range
            if (position == _data.size()) { // if UIView id is at the end exactly
                //_data.at(id)->deRegisterChildren();
                _data.pop_back();
            } else { // item is in middle of data somewhere
                spareIds.push_back(position); // add the spare slot to sparestack
            }
        }
    }
    template<typename U>
    std::size_t push(U&& in)
    {
        return push(std::forward<U>(in), [](auto a, auto b) { return; });
    }

    T& operator[](int idx)
    {
        return _data[idx];
    }

    T operator[](int idx) const
    {
        return _data[idx];
    }

private:
    std::vector<T> _data;
    std::vector<int> spareIds; // index of spare slots to fill ids
};
