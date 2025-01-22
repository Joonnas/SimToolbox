#ifndef ST_CAMERA_H
#define ST_CAMERA_H

#include "ext/include/aMath/a_math.h"
#include "Utils/macros.h"

/* CAMERA */

class Camera {
public:
    enum CameraType { CAMERA_TYPE_2D, CAMERA_TYPE_3D };
    static const float BASE_WIDTH;
    static const float BASE_HEIGHT;
    static const float BASE_ASPECT_RATIO;

private:
    static bool singleton_created;
protected:
    static Camera* cam_singleton;

protected:
    CameraType cam_type;
    aMath::Mat4 projection_matrix;
    aMath::Mat4 camera_matrix;

    aMath::Vec3 position = aMath::Vec3(0.0, 0.0, 0.0);
    aMath::Vec3 size = aMath::Vec3(1.0, 1.0, 1.0);

    aMath::Vec2 vpos = aMath::Vec2(0.0, 0.0);
    aMath::Vec2 vsize = aMath:: Vec2(1.0, 1.0);

    bool is_locked = false;


public:
    static void create_singleton() { Camera::singleton_created = true; };
    static void delete_singleton() { if (Camera::singleton_created) {delete cam_singleton; Camera::singleton_created = false;}};
    static Camera* get_camera_singleton() { return cam_singleton; };

    aMath::Mat4 get_projection_matrix() const { return projection_matrix; };
    aMath::Mat4 get_camera_matrix() const { return camera_matrix; };
    CameraType get_camera_type() const { return cam_type; };


    virtual void change_projection_matrix(aMath::Vec4 p_dimensions) = 0;
    virtual void change_camera_matrix(aMath::Vec3 p_pos, aMath::Vec3 p_size) = 0;
    virtual void update(float deltaTime) = 0;

    void set_camera_position(aMath::Vec3& p_pos) { change_camera_matrix(p_pos, size); };
    void set_camera_size(aMath::Vec3& p_size) { change_camera_matrix(position, p_size); };
    void set_viewport(aMath::Vec2& p_vpos, aMath::Vec2& p_vsize) { vpos = p_vpos; vsize = p_vsize; };

    void lock() { is_locked = true; };
    void unlock() { is_locked = false; };
};

/* END CAMERA */

/* CAMERA 2D */

class Camera2D : public Camera {
private:
    aMath::Vec4 dimensions;

public:
    static void create_singleton() {
        Camera::create_singleton();
        cam_singleton = new Camera2D();
    };

public:    
    void change_projection_matrix(aMath::Vec4 p_dimensions) {
        // if the projection space did not change, dont't recalculate the matrix
        if (p_dimensions.x == dimensions.x && p_dimensions.y == dimensions.y && p_dimensions.z == dimensions.z && p_dimensions.w == dimensions.w) return;
        dimensions = p_dimensions;
        projection_matrix = aMath::orthographic(p_dimensions.x, p_dimensions.y, p_dimensions.z, p_dimensions.w, 1.0, -1.0);
    };
    void change_camera_matrix(aMath::Vec3 p_pos, aMath::Vec3 p_size) {
        if (p_pos.x == position.x && p_pos.y == position.y && p_pos.z == position.z && p_size.x == size.x && p_size.y == size.y && p_size.z == size.z) return;
        position = p_pos;
        size = p_size;
        camera_matrix = aMath::mul(aMath::translate(position), aMath::scale(size));
    };

    void update(float deltaTime);

    Camera2D() {cam_type = Camera::CameraType::CAMERA_TYPE_2D;};
    ~Camera2D() {};
};

/* END CAMERA2D */

#endif