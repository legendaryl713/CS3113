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
    //this->state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, LEVEL_DATA, map_texture_id, 1.0f, 4, 1);

    //player
    state.player = new Entity();
    state.player->set_entity_type(PLAYER);
    state.player->set_position(glm::vec3(-3.5f, 1.0f, 0.0f));
    state.player->set_movement(glm::vec3(0.0f));
    state.player->speed = 25.0f;
    state.player->set_acceleration(glm::vec3(0.0f, 0.0f, 0.0f));
    state.player->texture_id = Utility::load_texture("assets/hook.png");

    //state.player->walking[state.player->LEFT]  = new int[4]{ 4, 5, 6, 7 };
    //state.player->walking[state.player->RIGHT] = new int[4]{ 8, 9, 10,  11 };
    //state.player->walking[state.player->UP]    = new int[4]{ 12, 13, 14, 15 };
    //state.player->walking[state.player->DOWN]  = new int[4]{ 0, 1, 2,  3 };

    //state.player->animation_indices = state.player->walking[state.player->RIGHT];  // start George looking left
    //state.player->animation_frames = 4;
    //state.player->animation_index  = 0;
    //state.player->animation_time   = 0.0f;
    //state.player->animation_cols   = 4;
    //state.player->animation_rows   = 4;
    state.player->set_height(1.8f);
    state.player->set_width(0.15f);


    state.ship = new Entity();
    state.ship->set_entity_type(SHIP);
    state.ship->set_position(glm::vec3(-3.0f, 0.0f, 0.0f));
    state.ship->set_movement(glm::vec3(0.0f));
    state.ship->speed = 5.5f;
    state.ship->set_acceleration(glm::vec3(0.0f, -9.81f, 0.0f));
    state.ship->texture_id = Utility::load_texture("assets/rickship.png");

    state.rocks = new Entity[6];
    state.rocks[0].set_entity_type(ROCK);
    state.rocks[0].texture_id = Utility::load_texture("assets/alien.png");
    state.rocks[0].set_position(glm::vec3(-2.3f, -1.35f, 0.0f));
    state.rocks[0].set_movement(glm::vec3(0.0f));
    state.rocks[0].speed = 1.0f;
    state.rocks[0].set_acceleration(glm::vec3(0.0f, 0.0f, 0.0f));
    state.rocks[0].move = false;
    state.rocks[0].s_c = 190;

    state.rocks[1].set_entity_type(ROCK);
    state.rocks[1].texture_id = Utility::load_texture("assets/planet1.png");
    state.rocks[1].set_position(glm::vec3(-3.058f, -0.14f, 0.0f));
    state.rocks[1].set_movement(glm::vec3(0.0f));
    state.rocks[1].speed = 1.0f;
    state.rocks[1].set_acceleration(glm::vec3(0.0f, 0.0f, 0.0f));
    state.rocks[1].move = false;
    state.rocks[1].s_c = 80;

    state.rocks[2].set_entity_type(ROCK);
    state.rocks[2].texture_id = Utility::load_texture("assets/planet2.png");
    state.rocks[2].set_position(glm::vec3(-0.65f, -0.96f, 0.0f));
    state.rocks[2].set_acceleration(glm::vec3(0.0f, 0.0f, 0.0f));
    state.rocks[2].move = false;
    state.rocks[2].s_c = 140;

    state.rocks[3].set_entity_type(ROCK);
    state.rocks[3].texture_id = Utility::load_texture("assets/rock6.png");
    state.rocks[3].set_position(glm::vec3(-1.075f, -0.4f, 0.0f));
    state.rocks[3].set_acceleration(glm::vec3(0.0f, 0.0f, 0.0f));
    state.rocks[3].move = false;
    state.rocks[3].s_c = 75;

    state.rocks[4].set_entity_type(ROCK);
    state.rocks[4].texture_id = Utility::load_texture("assets/rock.png");
    state.rocks[4].set_position(glm::vec3(-0.55f, -0.25f, 0.0f));
    state.rocks[4].set_acceleration(glm::vec3(0.0f, 0.0f, 0.0f));
    state.rocks[4].move = false;
    state.rocks[4].s_c = 60;

    state.rocks[5].set_entity_type(ROCK);
    state.rocks[5].texture_id = Utility::load_texture("assets/alien2.png");
    state.rocks[5].set_position(glm::vec3(-1.43f, -1.35f, 0.0f));
    state.rocks[5].set_acceleration(glm::vec3(0.0f, 0.0f, 0.0f));
    state.rocks[5].move = false;
    state.rocks[5].s_c = 210;




    //Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    //
    //state.jump_sfx = Mix_LoadWAV("assets/hop.wav");
}

void LevelA::update(float delta_time) {
    this->state.player->update(delta_time, state.player, state.rocks, 6, this->state.map, 1);
    this->state.ship->update(delta_time, state.player, state.enemies, this->ENEMY_COUNT, this->state.map, 1);
    for (int i = 0; i < 6; i++) {
        this->state.rocks[i].update(delta_time, state.player, state.enemies, this->ENEMY_COUNT, this->state.map, 1);
    }
}

void LevelA::render(ShaderProgram* program){

    float vertices1[] = { -1.0f, -4.14f, -0.1f, -4.14f, -0.1f, -2.0f, -1.0f, -4.14f, -0.1f, -2.0f, -1.0f, -2.0f };
    float vertices2[] = { -4.0f, -2.0f, -0.8f, -2.0f, -0.8f, 1.2f, -4.0f, -2.0f, -0.8f, 1.2f, -4.0f, 1.2f };
    float vertices3[] = { 2.0f, -4.0f, 5.2f, -4.0f, 5.2f, -0.8f, 2.0f, -4.0f, 5.2f, -0.8f, 2.0f, -0.8f };
    float vertices4[] = { 1.4f, -2.0f, 3.4f, -2.0f, 3.4f, 0.0f, 1.4f, -2.0f, 3.4f, 0.0f, 1.4f, 0.0f };
    float vertices5[] = { 3.3f, -1.0f, 4.1f, -1.0f, 4.1f, 0.3f, 3.3f, -1.0f, 4.1f, 0.3f, 3.3f, 0.3f };
    float vertices6[] = { 0.5f, -4.14f, 2.4f, -4.14f, 2.4f, -2.0f, 0.5f, -4.14f, 2.4f, -2.0f, 0.5f, -2.0f };

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
    Utility::draw_text(program, Utility::load_texture("assets/font1.png"), std::to_string(this->state.player->get_position().x), 0.5f, 0.01f, glm::vec3(0.0f, 0.0f, 0.0f), true);
}
