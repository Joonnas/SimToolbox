#ifndef ST_PARTICLEEMITTER_H
#define ST_PARTICLEEMITTER_H

#include <vector>

#include "ext/include/GL/glew.h"
#include "ext/include/aMath/a_math.h"

#include "Utils/macros.h"
#include "utils/properties.h"

class ParticleEmitter {
private:
    struct Particle {
        private:
            aMath::Vec3 pos;
            aMath::Vec3 velocity;
            float lifetime = 1.0f;
            float delta = 0.0f;
            bool alive = false;
        public:
            void update(float deltaTime, aMath::Vec3 res_force);
            aMath::Vec3 get_pos() const { return pos; };
            void set_pos(aMath::Vec3 p_pos) { pos = p_pos + (delta * velocity); };
            aMath::Vec3 get_velocity() const { return velocity; };
            void set_velocity(aMath::Vec3 p_vel) { velocity = p_vel; };
            void set_lifetime(float p_lifetime) { lifetime = p_lifetime - delta; };
            bool is_alive() const { return alive; };
            void set_alive(bool p_alive) { alive = p_alive; };
            void set_delta(float p_delta) { delta = p_delta; };

            Particle() {};
            Particle(aMath::Vec3 initial_pos, aMath::Vec3 initial_velocity, float p_lifetime) : pos(initial_pos), velocity(initial_velocity), lifetime(p_lifetime) {};
    };

private:
    static float vertices[18];

private:
    unsigned int VBO;
    unsigned int VAO;
    unsigned int shaderProgram;

    std::vector<Particle*> particles;
    float time = 0.0f;
    float cycle_start_time = 0.0f;
    int cycle = 0;

    // General settings
    Property<aMath::Vec3>* pos;

    // Emitter settings
    Property<bool>* emitting;
    Property<bool>* oneshot;
    Property<int>* amount;

    Property<int>* emission_shape;
    Property<float>* sphere_radius;
    Property<aMath::Vec2>* rect_extents;

    Property<float>* explosiveness;
    Property<aMath::Vec3>* initial_velocity;
    Property<float>* spread;
    Property<aMath::Vec3>* constant_acceleration;

    // Particle settings
    Property<float>* particle_lifetime;
    Property<float>* lifetime_randomness;

private:
    void prepare_particles();
    void spawnParticle(int i, float delta);
    void reset();

public:
    enum EmissionShapes {SHAPE_POINT, SHAPE_SPHERE, SHAPE_RECT};

public:
    

    void update(float deltaTime);
    void render();

    ParticleEmitter(Property<aMath::Vec3>* p_pos, Property<bool>* p_emitting, Property<bool>* p_oneshot, Property<int>* p_amount, Property<int>* p_emission_shape, Property<float>* p_sphere_radius, Property<aMath::Vec2>* p_rect_extents, Property<float>* p_explosiveness, Property<aMath::Vec3>* p_initial_velocity, Property<float>* p_spread, Property<aMath::Vec3>* p_constant_acceleration, Property<float>* p_particle_lifetime, Property<float>* p_lifetime_randomness);
    ~ParticleEmitter();

};

#endif