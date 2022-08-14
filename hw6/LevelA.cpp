#include "LevelA.h"
#include "Utility.h"

#define LEVEL_WIDTH 42
#define LEVEL_HEIGHT 8

unsigned int LEVEL_DATA[] =
{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,3,3,3,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,3,3,3,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,3,3,3,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,3,3,3,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,3,3,3,3,
    2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,1,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,3,3,3,
    2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,1,2,2,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,3,3,3,3,
    2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1,2,2,2,0,3,3,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,3,3,3,3,

};
LevelA::~LevelA()
{
    delete[] this->state.enemies;
    delete    this->state.player;
    delete    this->state.map;
    Mix_FreeChunk(this->state.jump_sfx);
}

void LevelA::initialise() {
    GLuint font_texture_id = Utility::load_texture("assets/font1.png");

    GLuint map_texture_id = Utility::load_texture("assets/t2.png");

    state.bg = new Entity();
    state.bg->set_entity_type(BG);
    state.bg->texture_id = Utility::load_texture("assets/bg1.png");
    state.bg->set_position(glm::vec3(-2.5f, 1.0f, 0.0f));

    //player
    state.player = new Entity();
    state.player->set_entity_type(PLAYER);
    state.player->set_position(glm::vec3(-3.5f, 1.0f, 0.0f));
    state.player->set_movement(glm::vec3(0.0f));
    state.player->speed = 25.0f;
    state.player->set_acceleration(glm::vec3(0.0f, 0.0f, 0.0f));
    state.player->texture_id = Utility::load_texture("assets/hook.png");

    state.player->set_height(1.8f);
    state.player->set_width(0.15f);


    state.ship = new Entity();
    state.ship->set_entity_type(SHIP);
    state.ship->set_position(glm::vec3(-3.0f, 0.0f, 0.0f));
    state.ship->set_movement(glm::vec3(0.0f));
    state.ship->speed = 5.5f;
    state.ship->set_acceleration(glm::vec3(0.0f, -9.81f, 0.0f));
    state.ship->texture_id = Utility::load_texture("assets/rickship.png");

    state.rocks = new Entity[11];
    state.rocks[0].set_entity_type(ROCK);
    state.rocks[0].texture_id = Utility::load_texture("assets/alien.png");
    state.rocks[0].set_position(glm::vec3(-2.3f, -1.15f, 0.0f));
    state.rocks[0].set_movement(glm::vec3(0.0f));
    state.rocks[0].speed = 1.0f;
    state.rocks[0].set_acceleration(glm::vec3(0.0f, 0.0f, 0.0f));
    state.rocks[0].move = false;
    state.rocks[0].s_c = 165;
    state.rocks[0].value = 50;

    state.rocks[1].set_entity_type(ROCK);
    state.rocks[1].texture_id = Utility::load_texture("assets/planet1.png");
    state.rocks[1].set_position(glm::vec3(-3.21f, -0.55f, 0.0f));
    state.rocks[1].set_movement(glm::vec3(0.0f));
    state.rocks[1].speed = 1.0f;
    state.rocks[1].set_acceleration(glm::vec3(0.0f, 0.0f, 0.0f));
    state.rocks[1].move = false;
    state.rocks[1].s_c = 110;
    state.rocks[1].value = 200;

    state.rocks[2].set_entity_type(ROCK);
    state.rocks[2].texture_id = Utility::load_texture("assets/planet2.png");
    state.rocks[2].set_position(glm::vec3(-0.65f, -0.96f, 0.0f));
    state.rocks[2].set_acceleration(glm::vec3(0.0f, 0.0f, 0.0f));
    state.rocks[2].move = false;
    state.rocks[2].s_c = 140;
    state.rocks[2].value = 220;

    state.rocks[3].set_entity_type(ROCK);
    state.rocks[3].texture_id = Utility::load_texture("assets/rock6.png");
    state.rocks[3].set_position(glm::vec3(-1.075f, -0.4f, 0.0f));
    state.rocks[3].set_acceleration(glm::vec3(0.0f, 0.0f, 0.0f));
    state.rocks[3].move = false;
    state.rocks[3].s_c = 100;
    state.rocks[3].value = 30;

    state.rocks[4].set_entity_type(ROCK);
    state.rocks[4].texture_id = Utility::load_texture("assets/rock.png");
    state.rocks[4].set_position(glm::vec3(-0.55f, -0.25f, 0.0f));
    state.rocks[4].set_acceleration(glm::vec3(0.0f, 0.0f, 0.0f));
    state.rocks[4].move = false;
    state.rocks[4].s_c = 79;
    state.rocks[4].value = 10;

    state.rocks[5].set_entity_type(ROCK);
    state.rocks[5].texture_id = Utility::load_texture("assets/alien2.png");
    state.rocks[5].set_position(glm::vec3(-1.43f, -1.35f, 0.0f));
    state.rocks[5].set_acceleration(glm::vec3(0.0f, 0.0f, 0.0f));
    state.rocks[5].move = false;
    state.rocks[5].s_c = 200;
    state.rocks[5].value = 180;
    
    state.rocks[6].set_entity_type(ROCK);
    state.rocks[6].texture_id = Utility::load_texture("assets/picklerick.png");
    state.rocks[6].set_position(glm::vec3(-3.15f, -1.20f, 0.0f));
    state.rocks[6].set_acceleration(glm::vec3(0.0f, 0.0f, 0.0f));
    state.rocks[6].move = false;
    state.rocks[6].s_c = 180;
    state.rocks[6].value = 500;

    state.rocks[6].walking[state.player->LEFT] = new int[4]{ 0, 1, 2,  3 };
    state.rocks[6].walking[state.player->RIGHT] = new int[4]{ 8, 9, 10, 11 };
    state.rocks[6].walking[state.player->UP] = new int[4]{ 2, 6, 10, 14 };
    state.rocks[6].walking[state.player->DOWN] = new int[4]{ 0, 4, 8,  12 };

    state.rocks[6].animation_indices = state.rocks[6].walking[state.player->RIGHT];
    state.rocks[6].animation_frames = 4;
    state.rocks[6].animation_index = 0;
    state.rocks[6].animation_time = 0.0f;
    state.rocks[6].animation_cols = 4;
    state.rocks[6].animation_rows = 4;
    state.rocks[6].speed = 25.0f;
    state.rocks[6].set_height(1.5f);
    state.rocks[6].set_width(0.4f);
    state.rocks[6].set_ai_type(WALKER);

    state.rocks[7].set_entity_type(ROCK);
    state.rocks[7].texture_id = Utility::load_texture("assets/rock5.png");
    state.rocks[7].set_position(glm::vec3(-3.48f, 0.11f, 0.0f));
    state.rocks[7].set_acceleration(glm::vec3(0.0f, 0.0f, 0.0f));
    state.rocks[7].move = false;
    state.rocks[7].s_c = 48;
    state.rocks[7].value = 37;

    state.rocks[8].set_entity_type(ROCK);
    state.rocks[8].texture_id = Utility::load_texture("assets/rock3.png");
    state.rocks[8].set_position(glm::vec3(-2.09f, 0.11f, 0.0f));
    state.rocks[8].set_acceleration(glm::vec3(0.0f, 0.0f, 0.0f));
    state.rocks[8].move = false;
    state.rocks[8].s_c = 43;
    state.rocks[8].value = 5;

    state.rocks[9].set_entity_type(ROCK);
    state.rocks[9].texture_id = Utility::load_texture("assets/diamond1.png");
    state.rocks[9].set_position(glm::vec3(-2.41f, -0.68f, 0.0f));
    state.rocks[9].set_acceleration(glm::vec3(0.0f, 0.0f, 0.0f));
    state.rocks[9].move = false;
    state.rocks[9].s_c = 130;
    state.rocks[9].value = 300;

    state.rocks[10].set_entity_type(ROCK);
    state.rocks[10].texture_id = Utility::load_texture("assets/diamond2.png");
    state.rocks[10].set_position(glm::vec3(-1.79f, -0.94f, 0.0f));
    state.rocks[10].set_acceleration(glm::vec3(0.0f, 0.0f, 0.0f));
    state.rocks[10].move = false;
    state.rocks[10].s_c = 143;
    state.rocks[10].value = 180;

    //Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    //
    //state.jump_sfx = Mix_LoadWAV("assets/hop.wav");
}

void LevelA::update(float delta_time) {
    this->state.player->update(delta_time, state.player, state.rocks, 11, this->state.map, 1);
    this->state.ship->update(delta_time, state.player, state.enemies, this->ENEMY_COUNT, this->state.map, 1);
    for (int i = 0; i < 11; i++) {
        this->state.rocks[i].update(delta_time, state.player, state.enemies, this->ENEMY_COUNT, this->state.map, 1);
    }
}

void LevelA::render(ShaderProgram* program){

    float vertices1[] = { -1.0f, -3.64f, -0.1f, -3.64f, -0.1f, -1.5f, -1.0f, -3.64f, -0.1f, -1.5f, -1.0f, -1.5f };
    float vertices2[] = { -4.4f, -3.0f, -1.2f, -3.0f, -1.2f, 0.2f, -4.4f, -3.0f, -1.2f, 0.2f, -4.4f, 0.2f };
    float vertices3[] = { 2.0f, -4.0f, 5.2f, -4.0f, 5.2f, -0.8f, 2.0f, -4.0f, 5.2f, -0.8f, 2.0f, -0.8f };
    float vertices4[] = { 1.4f, -2.0f, 3.4f, -2.0f, 3.4f, 0.0f, 1.4f, -2.0f, 3.4f, 0.0f, 1.4f, 0.0f };
    float vertices5[] = { 3.3f, -1.0f, 4.1f, -1.0f, 4.1f, 0.3f, 3.3f, -1.0f, 4.1f, 0.3f, 3.3f, 0.3f };
    float vertices6[] = { 0.5f, -4.14f, 2.4f, -4.14f, 2.4f, -2.0f, 0.5f, -4.14f, 2.4f, -2.0f, 0.5f, -2.0f };
    float vertices7[] = { -4.2f, -3.4f, -3.4f, -3.4f, -3.4f, -1.8f, -4.2f, -3.4f, -3.4f, -1.8f, -4.2f, -1.8f };
    float vertices8[] = { -5.2f, -0.5f, -2.5f, -0.5f, -2.5f, 1.7f, -5.2f, -0.5f, -2.5f, 1.7f, -5.2f, 1.7f };
    float vertices9[] = { -1.2f, -0.5f, 1.3f, -0.5f, 1.3f, 1.4f, -1.2f, -0.5f, 1.3f, 1.4f, -1.2f, 1.4f };
    float vertices10[] = { -1.2f, -1.5f, -0.6f, -1.5f, -0.6f, -0.9f, -1.2f, -1.5f, -0.6f, -0.9f, -1.2f, -0.9f };
    float vertices11[] = { 0.2f, -2.0f, 1.0f, -2.0f, 1.0f, -1.4f, 0.2f, -2.0f, 1.0f, -1.4f, 0.2f, -1.4f };

    float vertices_bg[] = { -7.0f, -4.0f, 5.0f, -4.0f, 5.0f, 4.0f, -7.0f, -4.0f, 5.0f, 4.0f, -7.0f, 4.0f };
    //this->state.map->render(program);
    this->state.bg->render3(program, vertices_bg);
    this->state.player->render(program);
    this->state.ship ->render2(program);
    this->state.rocks[0].render3(program,vertices1);
    this->state.rocks[1].render3(program, vertices2);
    this->state.rocks[2].render3(program, vertices3);
    this->state.rocks[3].render3(program, vertices4);
    this->state.rocks[4].render3(program, vertices5);
    this->state.rocks[5].render3(program, vertices6);
    this->state.rocks[6].render3(program, vertices7);
    this->state.rocks[7].render3(program, vertices8);
    this->state.rocks[9].render3(program, vertices10);
    this->state.rocks[10].render3(program, vertices11);
    this->state.rocks[8].render3(program, vertices9);

    Utility::draw_text(program, Utility::load_texture("assets/font1.png"), std::to_string(this->state.player->money), 0.5f, 0.01f, glm::vec3(-3.0f, 0.0f, 0.0f), true);
}
