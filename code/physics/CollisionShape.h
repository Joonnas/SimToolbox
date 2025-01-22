#ifndef ST_COLLISIONSHAPE_H
#define ST_COLLISIONSHAPE_H

#include "ext/include/GL/glew.h"
#include "ext/include/aMath/a_math.h"

#include "Utils/macros.h"
#include "Utils/properties.h"
#include "data_structures/quadtree.h"

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


    aMath::Vec3 get_pos() const { return pos->get(); }

public:

    void update(float deltaTime);
    void render();

    CollisionShape(Property<aMath::Vec3>* p_pos, Property<aMath::Vec4>* p_color, Property<bool>* p_react_to_forces, Property<bool>* p_react_to_keyboard, Property<int>* p_shape, Property<int>* p_broad_phase_collision_type, Property<int>* p_narrow_phase_collision_type);
    ~CollisionShape();

};

#endif