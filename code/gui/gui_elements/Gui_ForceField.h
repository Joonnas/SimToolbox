#ifndef ST_GUI_FORCEFIELD_H
#define ST_GUI_FORCEFIELD_H

#include <string>

#include "ext/include/imgui/imgui.h"
#include "Gui_SimElement.h"

class Gui_ForceField : public Gui_SimElement {
private:
    static int ID;
    
    int id;

public:

    void update(float deltaTime);
    void renderGui();
    void render();

    Gui_ForceField();
    ~Gui_ForceField() {};

};

#endif