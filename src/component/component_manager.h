#pragma once

#include <unordered_map>

static unsigned entities_id_ref = 1;
struct c_entity {
    unsigned id;

    c_entity() : id(entities_id_ref++) {
    }

    bool operator==(const c_entity &other) const {
        return this->id == other.id;
    }
};

namespace std {
    template<>
    struct hash<c_entity> {
        size_t operator()(const c_entity &e) const {
            return hash<unsigned>()(e.id);
        }
    };
}

struct component_manager {
    struct instance {
        unsigned index;
    };

    struct component_buffer {
        unsigned num;
        unsigned allocated;
        void *buffer;
    } buffer;
    
    std::unordered_map<c_entity, unsigned> entity_instance_map;

    virtual void create_component_instance_data(unsigned count) = 0;

    void assign_entity(const c_entity &e) {
        auto i = make_instance(buffer.num);
        entity_instance_map[e] = i.index;
        entity(e);
        ++buffer.num;
    }

    virtual void entity(const c_entity &e) = 0;

    bool exists(const c_entity & e) {
        return entity_instance_map.find(e) != entity_instance_map.end();
    }

    instance lookup(const c_entity &e) {
        return make_instance(entity_instance_map.find(e)->second);
    }

    instance make_instance(unsigned i) {
        return { i };
    }

    void destroy_entity_instance(const c_entity &e) {
        auto i = lookup(e);
        destroy_instance(i);
    }

    virtual void destroy_instance(instance i) = 0;

    virtual ~component_manager() {
        // allocated in derived create_component_instance_data() calls
        free(buffer.buffer);
        buffer.buffer = nullptr;
    }
};