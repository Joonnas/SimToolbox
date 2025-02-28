#ifndef ST_COLLISIONSHAPE_H
#define ST_COLLISIONSHAPE_H

#include "ext/include/GL/glew.h"
#include "ext/include/aMath/a_math.h"

#include "Utils/macros.h"
#include "Utils/properties.h"
#include "data_structures/quadtree.h"

struct AABB {
private:
    aMath::Vec2 position;
    aMath::Vec2 size;
public:
    bool intersects_point(aMath::Vec2 p_point) {
        if (p_point.x < position.x) return false;
        if (p_point.x > (position.x + size.x)) return false;
        if (p_point.y < position.y) return false;
        if (p_point.y > (position.y + size.y)) return false;
        return true;
    }
    bool intersects_aabb(AABB& p_aabb) {
        if (p_aabb.position.x > (position.x + size.x)) return false;
        if ((p_aabb.position.x + p_aabb.size.x) < position.x) return false;
        if (p_aabb.position.y > (position.y + size.y)) return false;
        if ((p_aabb.position.y + p_aabb.size.y) < position.y) return false;
        return true;
    }

    void set_position(aMath::Vec2 p_position) { position = p_position; }
    void set_size(aMath::Vec2 p_size) { size = p_size; }
    aMath::Vec2 get_position() const { return position; }
    aMath::Vec2 get_size() const { return size; }
};

struct OBB_2 {
private:
    aMath::Vec2 center;
    aMath::Vec2 axis_x;
    aMath::Vec2 axis_y;
    aMath::Vec2 half_edge_lengths;
public:
    bool intersects_obb(OBB_2& p_aabb) {
        return false;
    }

    void set_center(aMath::Vec2 p_center) { center = p_center; }
    void set_axis_x(aMath::Vec2 p_axis_x) { axis_x = p_axis_x; }
    void set_axis_y(aMath::Vec2 p_axis_y) { axis_y = p_axis_y; }
    void set_half_edge_lenths(aMath::Vec2 p_half_edge_lengths) { half_edge_lengths = p_half_edge_lengths; }
    aMath::Vec2 get_center() const { return center; }
    aMath::Vec2 get_axis_x() const { return axis_x; }
    aMath::Vec2 get_axis_y() const { return axis_y; }
    aMath::Vec2 get_half_edge_lengths() const { return half_edge_lengths; }
    
    aMath::Vec2 project(aMath::Vec2& p_axis, OBB_2& p_obb) {
        aMath::Vec2 ret;
        float r = abs(aMath::dot(p_obb.axis_x, p_axis) * half_edge_lengths.x) + abs(aMath::dot(p_obb.axis_y, p_axis) * half_edge_lengths.y);
        ret.x = aMath::dot(p_obb.center, p_axis) - r;
        ret.y = aMath::dot(p_obb.center, p_axis) + r;
        return ret;
    }
    bool intersects_obb2(OBB_2& p_obb) {
        aMath::Vec2 axes[4] = {axis_x, axis_y,
                            p_obb.axis_x, p_obb.axis_y,
        };
        // For every axis project the 2 OBBs onto the axis
        for (int i = 0; i < 4; i++) {
            aMath::Vec2 pr1 = project(axes[i], *this);
            aMath::Vec2 pr2 = project(axes[i], p_obb);
            // If the intervals don't overlap we have no collission along this axis and can return early
            if (!(pr1.y >= pr2.x && pr1.x <= pr2.y)) return false;
        }
        return true;
    }
};


class CollisionShape {
private:
    static float vertices_circle[180]; // 20 triangles
    static float vertices_rectangle[18];
    static float vertices_pentagon[45];
    static float vertices_octagon[72];

private:
    unsigned int VBO_circle;
    unsigned int VAO_circle;
    unsigned int shaderProgram;

public:
    enum Shapes {SHAPE_CIRCLE, SHAPE_RECTANGLE, SHAPE_PENTAGON, SHAPE_OCTAGON, SHAPE_WORLD_BOUNDARY};
    enum BroadPhaseCollisionType { BROAD_PHASE_RADIUS, BROAD_PHASE_AABB, BROAD_PHASE_OBB, BROAD_PHASE_KDOP };
    enum NarrowPhaseCollisionType { NARROW_PHASE_VORONOID, NARROW_PHASE_GJK };

private: 
    
    Property<aMath::Vec3>* pos;
    Property<aMath::Vec4>* color;

    Property<bool>* react_to_forces;
    Property<bool>* react_to_keyboard;

    Property<int>* shape;

    Property<int>* broad_phase_collision_type;
    Property<int>* narrow_phase_collision_type;

    Property<bool>* draw_aabb;

    AABB aabb;

private:
    aMath::Vec3 get_pos() const { return pos->get(); }

public:

    void update(float deltaTime);
    void render();

    // float get_radius();
    AABB get_aabb() const { return aabb; }

    CollisionShape(Property<aMath::Vec3>* p_pos, Property<aMath::Vec4>* p_color, Property<bool>* p_react_to_forces, Property<bool>* p_react_to_keyboard, Property<int>* p_shape, Property<int>* p_broad_phase_collision_type, Property<int>* p_narrow_phase_collision_type, Property<bool>* p_draw_aabb);
    ~CollisionShape();

};

#endif