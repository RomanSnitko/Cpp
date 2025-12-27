#include <iostream>
#include <iostream>
#include <unordered_map>
#include <list>
#include <optional>

template <typename K, typename V>
class LRUCache
{
private:
    size_t _capacity;
    std::list<std::pair<K, V>> _items;

    using typeCacheMap = std::unordered_map<K, typename std::list<std::pair<K, V>>::iterator>;
    typeCacheMap _cacheMap;

    void makeRecentnlyUsed(typeCacheMap::iterator it)
    {
        _items.splice(_items.begin(), _items, it->second); //splice мувает в листе
    }
public:
    explicit LRUCache(size_t capacity) : _capacity(capacity){}

    std::optional<V> get(const K& key)
    {
        auto it = _cacheMap.find(key);

        if (it == _cacheMap.end())
        {
            return std::nullopt;
        }

        makeRecentnlyUsed(it);
        return it->second->second;
    }

    void put(const K& key, const V& value)
    {
        auto it = _cacheMap.find(key);

        if (it != _cacheMap.end())
        {
            it->second->second = value;
            makeRecentnlyUsed(it);
            return;
        }

        if (_cacheMap.size() >= _capacity)
        {
            K keyToDelete = _items.back().first;
            _items.pop_back();
            _cacheMap.erase(keyToDelete);
        }

        _items.emplace_front(key, value);
        _cacheMap[key] = _items.begin();
    }
};

int main()
{
    LRUCache<int, std::string> cache(2);

    cache.put(1, "A");
    cache.put(2, "B");

    std::cout << "key 1: " << cache.get(1).value_or("missing") << std::endl;

    cache.put(3, "C");

    std::cout << "key 2: " << cache.get(2).value_or("missing") << std::endl; // missing
    std::cout << "key 3: " << cache.get(3).value_or("missing") << std::endl; // C
}
