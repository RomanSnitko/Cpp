#pragma once

#include <optional>
#include <functional>
#include <vector>
#include <memory>
#include <bit>

#include <boost/intrusive/list.hpp>
#include <boost/intrusive/unordered_set.hpp>

namespace lru 
{

namespace bi = boost::intrusive;

template <typename K, typename V,typename Hash = std::hash<K>,typename KeyEq = std::equal_to<K>>
class LRUCache
{
private:
    struct Node
    {
        K key;
        V value;

        bi::list_member_hook<> lru_hook;

        bi::unordered_set_member_hook<> set_hook;

        Node(const K& k, const V& v) : key(k), value(v) {}
        Node(K&& k, V&& v) : key(std::move(k)), value(std::move(v)) {}
    };

    using ItemsList = bi::list<
        Node,bi::member_hook<Node, 
        bi::list_member_hook<>, 
        &Node::lru_hook>>;

    struct KeyOfValue 
    {
        using type = K;
        const K& operator()(const Node& n) const noexcept { return n.key; }
    };

    using LookupSet = bi::unordered_set<
        Node,
        bi::member_hook<Node, bi::unordered_set_member_hook<>, &Node::set_hook>,
        bi::key_of_value<KeyOfValue>,
        bi::hash<Hash>,
        bi::equal<KeyEq>>;

private:
    std::size_t _capacity;
    ItemsList _items;

    std::vector<typename LookupSet::bucket_type> _buckets;
    typename LookupSet::bucket_traits _bucket_traits;
    LookupSet _lookup;

private:
    void makeRecentlyUsed(typename LookupSet::iterator it)
    {
        auto list_it = _items.iterator_to(*it);
        _items.splice(_items.begin(), _items, list_it);
    }

    void evictOne()
    {
        Node& victim = _items.back();
        _items.pop_back();
        _lookup.erase(_lookup.iterator_to(victim));
        delete &victim;
    }

public:
    explicit LRUCache(std::size_t capacity)
        : _capacity(capacity),
          _buckets(capacity != 0 ? 2 * std::bit_ceil(capacity) : 1),
          _bucket_traits(_buckets.data(), _buckets.size()),
          _lookup(_bucket_traits)
    {}

    LRUCache(const LRUCache&) = delete;
    LRUCache& operator=(const LRUCache&) = delete;
    LRUCache(LRUCache&&) = delete;
    LRUCache& operator=(LRUCache&&) = delete;

    ~LRUCache()
    {
        _lookup.clear();
        _items.clear_and_dispose([](Node* p){ delete p; });
    }

    std::optional<V> get(const K& key)
    {
        auto it = _lookup.find(key);
        if (it == _lookup.end())
            return std::nullopt;

        makeRecentlyUsed(it);
        return std::ref(it->value);
    }

    void put(const K& key, const V& value)
    {
        if (_capacity == 0) return;

        auto it = _lookup.find(key);
        if (it != _lookup.end())
        {
            it->value = value;
            makeRecentlyUsed(it);
            return;
        }

        if (_items.size() >= _capacity)
            evictOne();

        auto node = std::make_unique<Node>(key, value);

        _lookup.insert(*node);
        _items.push_front(*node);

        node.release();
    }

    void put(K&& key, V&& value)
    {
        if (_capacity == 0) return;

        auto it = _lookup.find(key);
        if (it != _lookup.end())
        {
            it->value = std::move(value);
            makeRecentlyUsed(it);
            return;
        }

        if (_items.size() >= _capacity)
            evictOne();

        auto node = std::make_unique<Node>(std::move(key), std::move(value));

        _lookup.insert(*node);
        _items.push_front(*node);

        node.release();
    }
};

} // namespace lru
