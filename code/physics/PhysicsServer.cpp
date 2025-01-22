#include "PhysicsServer.h"

#include <algorithm>

#include "ext/include/aMath/a_math.h"

bool PhysicsServer::singleton_created = false;
PhysicsServer* PhysicsServer::ps_singleton = nullptr;

// Private

bool PhysicsServer::has_collision_shape(CollisionShape* cs) {
    return (std::find(registered_collision_shapes.begin(), registered_collision_shapes.end(), cs) != registered_collision_shapes.end());
}

// Broad Phase Collision
bool PhysicsServer::_broad_phase_collision_radius(CollisionShape* cs) {
    return false;
}

bool PhysicsServer::_broad_phase_collision_aabb(CollisionShape* cs) {
    // Theoretisch müsste ich den AABB von der CollisionShape bekommen a la
    // cs->get_aabb();
    return false;
}

bool PhysicsServer::_broad_phase_collision_obb(CollisionShape* cs) {
    return false;
}

bool PhysicsServer::_broad_phase_collision_kdop(CollisionShape* cs) {
    return false;
}


// Narrow Phase Collision
bool PhysicsServer::_narrow_phase_collision_voronoi(CollisionShape* cs) {
    return false;
}

bool PhysicsServer::_narrow_phase_collision_gjk(CollisionShape* cs) {
    return false;
}



// Public

bool PhysicsServer::calculate_broad_phase_collision(int collision_type, CollisionShape* cs) {
    if (collision_type == CollisionShape::BroadPhaseCollisionType::BROAD_PHASE_RADIUS) return _broad_phase_collision_radius(cs);
    else if (collision_type == CollisionShape::BroadPhaseCollisionType::BROAD_PHASE_AABB) return _broad_phase_collision_aabb(cs);
    else if (collision_type == CollisionShape::BroadPhaseCollisionType::BROAD_PHASE_OBB) return _broad_phase_collision_obb(cs);
    else if (collision_type == CollisionShape::BroadPhaseCollisionType::BROAD_PHASE_KDOP) return _broad_phase_collision_kdop(cs);
}

bool PhysicsServer::calculate_narrow_phase_collision(int collision_type, CollisionShape* cs) {
    if (collision_type == CollisionShape::NarrowPhaseCollisionType::NARROW_PHASE_VORONOID) return _narrow_phase_collision_voronoi(cs);
    else if (collision_type == CollisionShape::NarrowPhaseCollisionType::NARROW_PHASE_GJK) return _narrow_phase_collision_gjk(cs);
}