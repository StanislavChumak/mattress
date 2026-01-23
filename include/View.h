#ifndef VIEM_H
#define VIEM_H
#include <tuple>
#include "SparseSet.h"

template<typename... ComponentTypes>
class View {
private:
    std::tuple<SparseSet<ComponentTypes>&...> component_sets;

public:
    View(SparseSet<ComponentTypes>&... sets) : component_sets(sets...) {}

    class Iterator {
    private:
        std::tuple<SparseSet<ComponentTypes>&...>& sets;
        
        // Используем правильный тип итератора SparseSet
        typename SparseSet<std::tuple_element_t<0, std::tuple<ComponentTypes...>>>::Iterator current;
        typename SparseSet<std::tuple_element_t<0, std::tuple<ComponentTypes...>>>::Iterator end;

        bool has_all_components(EntityID entity) {
            return (std::get<SparseSet<ComponentTypes>&>(sets).has(entity) && ...);
        }

        void advance_to_valid() {
            while (current != end && !has_all_components(current.get_entity())) {
                ++current;
            }
        }

    public:
        // Конструктор принимает правильные типы итераторов
        Iterator(decltype(sets) sets_ref, 
                typename SparseSet<std::tuple_element_t<0, std::tuple<ComponentTypes...>>>::Iterator start,
                typename SparseSet<std::tuple_element_t<0, std::tuple<ComponentTypes...>>>::Iterator end_pos)
            : sets(sets_ref), current(start), end(end_pos) 
        {
            advance_to_valid();
        }

        bool operator!=(const Iterator& other) const {
            return current != other.current;
        }

        Iterator& operator++() {
            ++current;
            advance_to_valid();
            return *this;
        }

        auto operator*() {
            EntityID entity = current.get_entity();
            return std::tuple_cat(
                std::make_tuple(entity),
                std::make_tuple(std::get<SparseSet<ComponentTypes>&>(sets).get(entity))...
            );
        }
    };

    Iterator begin() {
        auto& first_set = std::get<0>(component_sets);
        return Iterator(component_sets, first_set.entity_begin(), first_set.entity_end());
    }

    Iterator end() {
        auto& first_set = std::get<0>(component_sets);
        return Iterator(component_sets, first_set.entity_end(), first_set.entity_end());
    }
};


#endif
