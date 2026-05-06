#include <cstddef>
#include <unordered_map>
#include <list>
#include <functional>
#include <utility>

template<typename Key, typename Value>
class LRU_Cache {
public:

    LRU_Cache(const LRU_Cache&) = delete;
    LRU_Cache& operator=(const LRU_Cache&) = delete;

    LRU_Cache(std::size_t C)
        : cap(C)
    { }

    Value* get(const Key& key) {
        auto it = cache.find(key);
        if(it == cache.end()) {
            return nullptr;
        }
        items.splice(items.begin(), items, it->second);
        return &(it->second->second);
    }

    void put(const Key& key, Value value) {
        auto it = cache.find(key);

        if(it == cache.end()) {
            items.emplace_front(key, std::move(value));
            cache.emplace(key, items.begin());
            
            if(cache.size() > cap) {
                cache.erase(items.back().first);
                items.pop_back();
            }
        } else {
            it->second->second = std::move(value);
            items.splice(items.begin(), items, it->second);
        }
    }

    void clear() {
        items.clear();
        cache.clear();
    }

private:

    std::list<std::pair<Key, Value>> items;

    std::size_t cap;
    std::unordered_map<Key, typename std::list<std::pair<Key, Value>>::iterator> cache;

};
