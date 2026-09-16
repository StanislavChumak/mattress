#ifndef SPARSE_SET_HPP
#define SPARSE_SET_HPP

#include <deque>
#include <memory>
#include <limits>
#include <cstdint>
#include <utility>
#include <type_traits>

typedef uint64_t EntityID;
constexpr EntityID NULL_ENTITY = std::numeric_limits<EntityID>::max();

namespace mtrs::comp
{

template<typename TypeComponent>
class SparseSet
{
    std::deque<std::unique_ptr<TypeComponent>> _dense;
    std::deque<EntityID> _sparse;
    std::deque<EntityID> _entities;
    size_t _size = 0;

public:
    SparseSet() = default;

    template<typename ...Args>
    TypeComponent *add(EntityID entity, Args&& ...args)
    {
        if (entity >= _sparse.size())
        {
            _sparse.resize(entity + 1, NULL_ENTITY);
        }
        
        if (_sparse[entity] == NULL_ENTITY)
        {
            _sparse[entity] = _size;
            _entities.push_back(entity);
            _dense.push_back(std::unique_ptr<TypeComponent>(nullptr));
            _size++;
        }

        _dense[_sparse[entity]] = std::make_unique<TypeComponent>(args...);
        return _dense[_sparse[entity]].get();
    }

    TypeComponent *get(EntityID entity)
    {
        if (entity >= _sparse.size()) return nullptr;
        EntityID dense_index = _sparse[entity];
        if (dense_index == NULL_ENTITY) return nullptr;
        return _dense[dense_index].get();
    }
    
    const TypeComponent *get(EntityID entity) const
    {
        if (entity >= _sparse.size()) return nullptr;
        EntityID dense_index = _sparse[entity];
        if (dense_index == NULL_ENTITY) return nullptr;
        return _dense[dense_index].get();
    }

    bool has(EntityID entity) const
    {
        return entity < _sparse.size() && _sparse[entity] != NULL_ENTITY;
    }

    void remove(EntityID entity)
    {
        if (!has(entity)) return;
        
        EntityID dense_index = _sparse[entity];
        EntityID last_entity = _entities.back();
        
        if (dense_index != _size - 1)
        {
            _dense[dense_index] = std::move(_dense.back());
            _entities[dense_index] = last_entity;
            
            _sparse[last_entity] = dense_index;
        }
        
        _dense.pop_back();
        _entities.pop_back();
        _sparse[entity] = NULL_ENTITY;
        _size--;
    }

    template<bool IsConst>
    class IteratorImpl
    {
        using EntitiesRef = std::conditional_t<IsConst, const std::deque<EntityID>&, std::deque<EntityID>&>;
        using DenseRef = std::conditional_t<IsConst, const std::deque<std::unique_ptr<TypeComponent>>&, std::deque<std::unique_ptr<TypeComponent>>&>;
        using ComponentRef = std::conditional_t<IsConst, const TypeComponent&, TypeComponent&>;

        EntitiesRef _entities;
        DenseRef _dense;
        size_t _index;

    public:
        IteratorImpl(EntitiesRef entities, DenseRef dense, size_t index)
        : _entities(entities), _dense(dense), _index(index)
        {}

        std::pair<EntityID, ComponentRef> operator*() const
        {
            return {_entities[_index], *_dense[_index]};
        }

        IteratorImpl& operator++() { _index++; return *this; }

        bool operator!=(const IteratorImpl& other) const
        {
            return _index != other._index;
        }

        EntityID get_entity() const
        {
            return _entities[_index];
        }
    };

    using Iterator = IteratorImpl<false>;
    using ConstIterator = IteratorImpl<true>;

    Iterator begin()
    {
        return Iterator(_entities, _dense, 0);
    }

    Iterator end()
    {
        return Iterator(_entities, _dense, _size);
    }

    ConstIterator begin() const
    {
        return ConstIterator(_entities, _dense, 0);
    }

    ConstIterator end() const
    {
        return ConstIterator(_entities, _dense, _size);
    }
    

    const std::deque<EntityID>& get_entities() const
    {
        return _entities;
    }

    size_t size() const
    {
        return _size;
    }

    bool empty() const
    {
        return _size == 0;
    }

    void clear()
    {
        _dense.clear();
        _entities.clear();
        for (auto& idx : _sparse)
        {
            idx = NULL_ENTITY;
        }
        _size = 0;
    }
};

}

#endif