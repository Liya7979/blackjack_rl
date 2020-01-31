#ifndef DEFAULT_DICT_H
#define DEFAULT_DICT_H

#include <unordered_map>

template<typename K, typename V>
class DefaultDict {
private:
    std::unordered_map<K, V> map;
    static constexpr V default_value = 0;

    bool map_contains(const K& key) {
        return map.find(key) != map.end();
    }

    void fill_if_empty(const K& key) {
        if (!map_contains(key)) {
            map.insert(std::make_pair(key, default_value));
        }
    }

public:
    V& operator[](const K &key) {
        fill_if_empty(key);
        return map.at(key);
    }
    V& at(const K &key) {
        fill_if_empty(key);
        return map.at(key);
    }

    std::unordered_map<K, V> &to_map() {
        return this->map;
    }

    void push(const K &key, const V& value = default_value) {
        if (map_contains(key)) {
            map.at(key) = value;
        } else {
            map.insert(std::make_pair(key, value));
        }
    }
    V pull(const K &key) {
        if (map_contains(key)) {
            V value = map.at(key);
            map.erase(key);
            return value;
        } else {
            return default_value;
        }
    }

    std::size_t size() {
        return map.size();
    }
};

#endif //DEFAULT_DICT_H
