#ifndef ST_PROPERTIES_H
#define ST_PROPERTIES_H

#include <functional>
#include "Utils/macros.h"

template<class T>
class Property {
private:
    T value;
    T last_value;
    std::function<void(T)> onChange;
public:

    Property() {};
    Property(T p_value = T(), std::function<void(T)> p_onChange = nullptr) : value(p_value), last_value(p_value), onChange(p_onChange) {};

    T get() const { return value; };
    void set(T p_value) {
        if (p_value == value) return;
        value = p_value;
        if (onChange) {
            onChange(value);
        }
    }

    void set_onChange(std::function<void(T)> p_onChange) { onChange = p_onChange; };
    void remove_onChange() { onChange = nullptr; };

    void sync() {
        if (value != last_value) {
            last_value = value;
            if (onChange) { onChange(value); }
        }
    };

    operator T() const { return get(); };
    T* operator&() { return &value; };
    Property<T>* getPointer() { return this; };

    T& operator=(const T& a) { set(a); return *value; };
};

#endif