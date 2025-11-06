#ifndef SPARSESET_H
#define SPARSESET_H

#include <vector>
#include <limits>

typedef unsigned long EntityID;
constexpr EntityID NULL_ENTITY = std::numeric_limits<EntityID>::max();


template<typename TypeComponent>
class SparseSet
{
    std::vector<TypeComponent> dense;
    std::vector<EntityID> sparse;
    std::vector<EntityID> entities;
    size_t size_ = 0;

public:
    SparseSet() = default;
    
    void reserve(size_t capacity) {
        dense.reserve(capacity);
        entities.reserve(capacity);
    }

    TypeComponent &add(EntityID entity, TypeComponent component) {
        if (entity >= sparse.size()) {
            sparse.resize(entity + 1, NULL_ENTITY);
        }
        
        if (sparse[entity] != NULL_ENTITY) {
            dense[sparse[entity]] = std::move(component);
            return dense[sparse[entity]];
        }
        
        sparse[entity] = size_;
        entities.push_back(entity);
        dense.push_back(std::move(component));
        size_++;
        return dense[sparse[entity]];
    }

    TypeComponent* get(EntityID entity) {
        if (entity >= sparse.size()) return nullptr;
        EntityID dense_index = sparse[entity];
        if (dense_index == NULL_ENTITY) return nullptr;
        return &dense[dense_index];
    }
    
    const TypeComponent* get(EntityID entity) const {
        if (entity >= sparse.size()) return nullptr;
        EntityID dense_index = sparse[entity];
        if (dense_index == NULL_ENTITY) return nullptr;
        return &dense[dense_index];
    }

    bool has(EntityID entity) const {
        return entity < sparse.size() && sparse[entity] != NULL_ENTITY;
    }

    void remove(EntityID entity) {
        if (!has(entity)) return;
        
        EntityID dense_index = sparse[entity];
        EntityID last_entity = entities.back();
        
        if (dense_index != size_ - 1) {

            dense[dense_index] = std::move(dense.back());
            entities[dense_index] = last_entity;
            

            sparse[last_entity] = dense_index;
        }
        
        dense.pop_back();
        entities.pop_back();
        sparse[entity] = NULL_ENTITY;
        size_--;
    }


    auto begin() { return dense.begin(); }
    auto end() { return dense.end(); }
    auto begin() const { return dense.begin(); }
    auto end() const { return dense.end(); }


    class Iterator {
    private:
        std::vector<EntityID>& entities;
        std::vector<TypeComponent>& dense;
        size_t index;
        
    public:
        Iterator(std::vector<EntityID>& e, std::vector<TypeComponent>& d, size_t i) 
            : entities(e), dense(d), index(i) {}
            
        std::pair<EntityID, TypeComponent&> operator*() {
            return {entities[index], dense[index]};
        }
        
        Iterator& operator++() { index++; return *this; }
        bool operator!=(const Iterator& other) { return index != other.index; }

        bool operator!=(const Iterator& other) const {
            return index != other.index;
        }

        EntityID get_entity() const {
            return entities[index];
        }
    };

    Iterator entity_begin() { return Iterator(entities, dense, 0); }
    Iterator entity_end() { return Iterator(entities, dense, size_); }


    size_t size() const { return size_; }
    bool empty() const { return size_ == 0; }
    size_t capacity() const { return dense.capacity(); }


    void clear() {
        dense.clear();
        entities.clear();
        for (auto& idx : sparse) {
            idx = NULL_ENTITY;
        }
        size_ = 0;
    }
};

#endif
