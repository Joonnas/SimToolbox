#ifndef ST_GUI_PARTICLEEMITTER_H
#define ST_GUI_PARTICLEEMITTER_H

#include <string>

// #include "utils/properties.h"
#include "Gui_SimElement.h"
#include "physics/ParticleEmitter.h"

class Gui_ParticleEmitter : public Gui_SimElement {
private:
    static int ID;

private:
    int id;

    // emitter
    Property<bool> emitting = true;
    Property<bool> oneshot = false;
    Property<int> particles_amount = 8;

        // emission shapes
    Property<int> emission_shape = 0;
    Property<float> sphere_radius = 0.2;
    Property<aMath::Vec2> rect_extents = aMath::Vec2(0.2, 0.2);
    
    Property<float> explosiveness = 0.0;
    Property<aMath::Vec3> initial_velocity = aMath::Vec3();
    Property<float> spread = 0.0;
    Property<aMath::Vec3> constant_acceleration = aMath::Vec3();

    // particles
    Property<float> particle_lifetime = 1.0f;
    Property<float> lifetime_randomness = 0.0;
    Property<bool> use_custom_script = false;
    ImVector<char> code_string;

    ParticleEmitter* pe;

public:
    void update(float deltaTime);
    void renderGui();
    void render();

    Gui_ParticleEmitter();
    // Gui_ParticleEmitter(std::string p_name);
    ~Gui_ParticleEmitter() {
        delete pe;
    };

};

#endif