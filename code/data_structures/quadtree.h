#ifndef ST_QUADTREE_H
#define ST_QUADTREE_H

#include <vector>

#include "ext/include/aMath/a_math.h"
#include "Utils/draw.h"

template<class T>
class QuadTree {
public:
    static const int MAX_COUNT = 4;


private:
    QuadTree* ul = nullptr;
    QuadTree* ur = nullptr;
    QuadTree* bl = nullptr;
    QuadTree* br = nullptr;

    T data[MAX_COUNT];
    aMath::Vec2 data_positions[MAX_COUNT];

    aMath::Vec2 position;
    aMath::Vec2 size;

    bool divided = false;
    int data_count = 0;

private:
    bool point_in_rectangle(aMath::Vec2 p_position) {
        // basic AABB
        return (p_position.x > position.x && p_position.x < (position.x + size.x) && p_position.y > position.y && p_position.y < (position.y + size.y));
    };
    bool point_in_rectangle2(aMath::Vec2 p_rect_position, aMath::Vec2 p_rect_size, aMath::Vec2 p_point_position) {
        return (p_point_position.x > p_rect_position.x && p_point_position.x < (p_rect_position.x + p_rect_size.x) && p_point_position.y > p_rect_position.y && p_point_position.y < (p_rect_position.y + p_rect_size.y));
    }
    bool rectangle_in_rectangle(aMath::Vec2 p_position, aMath::Vec2 p_size) {
        return (p_position.x < (position.x + size.x) && (p_position.x + p_size.x) > position.x && p_position.y < (position.y + size.y) && (p_position.y + p_size.y) > position.y);
    };
    bool _insert(aMath::Vec2 p_position, T p_data) {

        if (!divided && data_count == QuadTree::MAX_COUNT) {
            divide();
        }

        if (divided) {
            if (ul->insert(p_position, p_data)) return true;
            if (ur->insert(p_position, p_data)) return true;
            if (bl->insert(p_position, p_data)) return true;
            if (br->insert(p_position, p_data)) return true;
        } else {
            data[data_count] = p_data;
            data_positions[data_count] = p_position;
            data_count += 1;
            return true;
        }

        return false;

    };

public:

    void divide() {
        aMath::Vec2 new_size = aMath::Vec2(size.x / 2.0, size.y / 2.0);
        ul = new QuadTree(position, new_size);
        ur = new QuadTree(aMath::Vec2(position.x + new_size.x, position.y), new_size);
        bl = new QuadTree(aMath::Vec2(position.x, position.y + new_size.y), new_size);
        br = new QuadTree(position + new_size, new_size);
        divided = true;
    };
    bool insert(aMath::Vec2 p_position, T p_data) { if (point_in_rectangle(p_position)) return _insert(p_position, p_data); else return false; };

    std::vector<T> get_in_rectangle(aMath::Vec2 p_position, aMath::Vec2 p_size) {
        std::vector<T> ret_vec;
        if (rectangle_in_rectangle(p_position, p_size)) {

            for (int i = 0; i < data_count; i++) {
                if (point_in_rectangle2(p_position, p_size, data_positions[i])) ret_vec.push_back(data[i]);
            }

            if (divided) {
                std::vector<T> temp1 = ul->get_in_rectangle(p_position, p_size);
                ret_vec.insert(ret_vec.end(), temp1.begin(), temp1.end());
                
                std::vector<T> temp2 = ur->get_in_rectangle(p_position, p_size);
                ret_vec.insert(ret_vec.end(), temp2.begin(), temp2.end());
                
                std::vector<T> temp3 = bl->get_in_rectangle(p_position, p_size);
                ret_vec.insert(ret_vec.end(), temp3.begin(), temp3.end());
                
                std::vector<T> temp4 = br->get_in_rectangle(p_position, p_size);
                ret_vec.insert(ret_vec.end(), temp4.begin(), temp4.end());
            }
            
        }

        return ret_vec;
    };


    void debug_draw() {
        DrawPen::get_drawpen_singleton()->set_draw_color(aMath::Color_RGBA(1.0, 0.0, 0.0, 1.0));
        DrawPen::get_drawpen_singleton()->set_stroke_size(0.25);
        DrawPen::get_drawpen_singleton()->drawRect(position, size);
        if (ul != nullptr) ul->debug_draw();
        if (ur != nullptr) ur->debug_draw();
        if (bl != nullptr) bl->debug_draw();
        if (br != nullptr) br->debug_draw();
    };


    QuadTree(aMath::Vec2 p_position, aMath::Vec2 p_size) : position(p_position), size(p_size) {};
    ~QuadTree() {
        if (ul != nullptr) delete ul;
        if (ur != nullptr) delete ur;
        if (bl != nullptr) delete bl;
        if (br != nullptr) delete br;

        // delete[] data;
    };
};

#endif