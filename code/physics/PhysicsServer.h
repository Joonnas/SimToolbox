#ifndef ST_PHYSICSSERVER_H
#define ST_PHYSICSSERVER_H

#include <vector>

#include "Utils/macros.h"

#include "data_structures/quadtree.h"
#include "physics/CollisionShape.h"

/**
 * 
 * The PhysicsServer registers and stores all physics objects
 * that can collide with each other.
 * 
 */

class PhysicsServer {
private:
    static bool singleton_created;
    static PhysicsServer* ps_singleton;

private:
    std::vector<CollisionShape*> registered_collision_shapes;

private:
    bool has_collision_shape(CollisionShape* cs);

    bool _broad_phase_collision_radius(CollisionShape* cs);
    bool _broad_phase_collision_aabb(CollisionShape* cs);
    bool _broad_phase_collision_obb(CollisionShape* cs);
    bool _broad_phase_collision_kdop(CollisionShape* cs);

    bool _narrow_phase_collision_voronoi(CollisionShape* cs);
    bool _narrow_phase_collision_gjk(CollisionShape* cs);

public:
    static void create_singleton() { if (!singleton_created) {ps_singleton = new PhysicsServer(); singleton_created = true; }};
    static void delete_singleton() { if (singleton_created) {delete ps_singleton; singleton_created = false; }}
    static PhysicsServer* get_singleton() { return ps_singleton; };

public:

    void register_collision_shape(CollisionShape* cs) { if(!has_collision_shape(cs)) registered_collision_shapes.push_back(cs); };
    void unregister_collision_shape(CollisionShape* cs) { /* TODO: */};

    bool calculate_broad_phase_collision(int collision_type, CollisionShape* cs);
    bool calculate_narrow_phase_collision(int collision_type, CollisionShape* cs);

    PhysicsServer() {};
    ~PhysicsServer() {};
};

#endif