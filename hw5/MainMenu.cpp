#include "MainMenu.h"
#include "Utility.h"

#define LEVEL_WIDTH 42
#define LEVEL_HEIGHT 8

//unsigned int LEVEL_DATA1[] =
//{
//    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,3,3,3,
//    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,3,3,3,
//    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,3,3,3,
//    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,3,3,3,
//    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,3,3,3,3,
//    2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,1,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,3,3,3,
//    2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,1,2,2,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,3,3,3,3,
//    2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1,2,2,2,0,3,3,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,3,3,3,3,
//
//};
MainMenu::~MainMenu()
{
    delete[] this->state.enemies;
    delete    this->state.player;
    delete    this->state.map;
    Mix_FreeChunk(this->state.jump_sfx);
}

void MainMenu::initialise() {
    GLuint font_texture_id = Utility::load_texture("assets/font1.png");
    this->state.player = NULL;
    this->state.enemies = NULL;
    this->state.rocket = NULL;
}

void MainMenu::update(float delta_time) {

}

void MainMenu::render(ShaderProgram* program)
{
    Utility::draw_text(program, Utility::load_texture("assets/font1.png"), "PRESS ENTER TO START", 0.5f, 0.001f, glm::vec3(-4.8f, 0.0f, 0.0f), true);
}