#ifndef ST_BOIDS_H
#define ST_BOIDS_H

#include <vector>

#include "ext/include/GL/glew.h"
#include "ext/include/aMath/a_math.h"

#include "Utils/macros.h"
#include "Utils/properties.h"
#include "data_structures/quadtree.h"

class Boids {
private:
    struct BoidData {
        private:
            aMath::Vec3 pos;
            aMath::Vec3 velocity;
            float rotation;
        public:
            void update(float deltaTime, aMath::Vec3 forces, float p_max_speed, aMath::Vec4 bounds);
            aMath::Vec3 get_pos() const { return pos; };
            aMath::Vec3 get_velocity() const { return velocity; };
            float get_rotation() const { return rotation; };
            BoidData(aMath::Vec3 p_pos) : pos(p_pos) {};
            BoidData(aMath::Vec3 p_pos, aMath::Vec3 p_velocity, float p_rotation) : pos(p_pos), velocity(p_velocity), rotation(p_rotation) {};
    };

private:
    static float vertices[9];

private:
    unsigned int VBO;
    unsigned int VAO;
    unsigned int shaderProgram;

    float time = 0.0f;

    aMath::Vec2 pos_ul;
    aMath::Vec2 size;

    std::vector<BoidData*> boid_data;
    QuadTree<BoidData*> qtree;

    Property<aMath::Vec3>* pos;

    Property<int>* amount;
    Property<aMath::Vec2>* custom_boundary_extents;
    Property<int>* boid_perception_field;
    Property<aMath::Vec2>* bpf_rect_size;
    Property<float>* bpf_circ_radius;
    Property<float>* bpf_circ_arc_angle;
    
    Property<float>* max_speed;
    Property<float>* seperation;
    Property<float>* alignment;
    Property<float>* cohesion;

    Property<bool>* draw_quadtree;
    Property<bool>* draw_border;
    Property<bool>* draw_boid_visual_field;
    Property<bool>* draw_boid_velocity;

private:
    aMath::Vec3 seperate(BoidData* current, std::vector<BoidData*> boids_in_view);
    aMath::Vec3 align(BoidData* current, std::vector<BoidData*> boids_in_view);
    aMath::Vec3 cohese(BoidData* current, std::vector<BoidData*> boids_in_view);
    aMath::Vec3 additionalForces(BoidData* current, std::vector<BoidData*> boids_in_view);

    void prepareBoids();
    void rebuildQTree();

public:
    enum BoidPerceptionField {BOID_PERCEPTION_CENTERED_RECT, BOID_PERCEPTION_FOLLOWING_RECT, BOID_PERCEPTION_CIRCULAR_ARC};

public:

    void update(float deltaTime);
    void render();

    Boids(Property<aMath::Vec3>* p_pos, Property<int>* p_amount, Property<aMath::Vec2>* p_custom_boundary_extents, Property<int>* p_boid_perception_field, Property<aMath::Vec2>* p_bpf_rect_size, Property<float>* p_bpf_circ_radius, Property<float>* p_bpf_circ_arc_angle, Property<float>* p_max_speed, Property<float>* p_seperation, Property<float>* p_alignment, Property<float>* p_cohesion, Property<bool>* p_draw_quadtree, Property<bool>* p_draw_border, Property<bool>* p_draw_boid_visual_field, Property<bool>* p_draw_boid_velocity);
    ~Boids();

};

#endif