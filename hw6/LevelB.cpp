#include "LevelB.h"
#include "Utility.h"

#define LEVEL_WIDTH 42
#define LEVEL_HEIGHT 8

LevelB::~LevelB()
{
    delete[] this->state.enemies;
    delete    this->state.player;
    delete    this->state.map;
    Mix_FreeChunk(this->state.jump_sfx);
    Mix_FreeMusic(this->state.bgm);
}

void LevelB::initialise() {
    GLuint font_texture_id = Utility::load_texture("assets/font1.png");

    GLuint map_texture_id = Utility::load_texture("assets/t2.png");

    state.bg = new Entity();
    state.bg->set_entity_type(BG);
    state.bg->texture_id = Utility::load_texture("assets/bg3.png");
    state.bg->set_position(glm::vec3(-2.5f, 1.0f, 0.0f));

    //player
    state.player = new Entity();
    state.player->set_entity_type(PLAYER);
    state.player->set_position(glm::vec3(-3.5f, 1.0f, 0.0f));
    state.player->set_movement(glm::vec3(0.0f));
    state.player->speed = 25.0f;
    state.player->set_acceleration(glm::vec3(0.0f, 0.0f, 0.0f));
    state.player->texture_id = Utility::load_texture("assets/hook3.png");

    state.player->set_height(1.8f);
    state.player->set_width(0.15f);


    state.ship = new Entity();
    state.ship->set_entity_type(SHIP);
    state.ship->set_position(glm::vec3(-3.0f, 0.0f, 0.0f));
    state.ship->set_movement(glm::vec3(0.0f));
    state.ship->speed = 5.5f;
    state.ship->set_acceleration(glm::vec3(0.0f, -9.81f, 0.0f));
    state.ship->texture_id = Utility::load_texture("assets/rickship.png");

    state.enemies = new Entity[1];
    state.enemies[0].set_entity_type(BG);
    state.enemies[0].texture_id = Utility::load_texture("assets/pillar.png");
    state.enemies[0].set_position(glm::vec3(-2.3f, -1.15f, 0.0f));

    state.rocks = new Entity[6];
    state.rocks[0].set_entity_type(ROCK);
    state.rocks[0].texture_id = Utility::load_texture("assets/morty2.png");
    state.rocks[0].set_position(glm::vec3(-2.3f, -1.15f, 0.0f));
    state.rocks[0].set_movement(glm::vec3(0.0f));
    state.rocks[0].speed = 1.0f;
    state.rocks[0].set_acceleration(glm::vec3(0.0f, 0.0f, 0.0f));
    state.rocks[0].move = false;
    state.rocks[0].s_c = 170;
    state.rocks[0].value = 1000;

    state.rocks[1].set_entity_type(ROCK);
    state.rocks[1].texture_id = Utility::load_texture("assets/morty3.png");
    state.rocks[1].set_position(glm::vec3(-1.54f, -1.05f, 0.0f));
    state.rocks[1].set_movement(glm::vec3(0.0f));
    state.rocks[1].speed = 1.0f;
    state.rocks[1].set_acceleration(glm::vec3(0.0f, 0.0f, 0.0f));
    state.rocks[1].move = false;
    state.rocks[1].s_c = 165;
    state.rocks[1].value = 1500;

    state.rocks[2].set_entity_type(ROCK);
    state.rocks[2].texture_id = Utility::load_texture("assets/rk.png");
    state.rocks[2].set_position(glm::vec3(-0.35f, -1.05f, 0.0f));
    state.rocks[2].set_acceleration(glm::vec3(0.0f, 0.0f, 0.0f));
    state.rocks[2].move = false;
    state.rocks[2].s_c = 180;
    state.rocks[2].value = 6666;

    state.rocks[2].walking[state.player->LEFT] = new int[4]{ 0, 1, 2,  3 };
    state.rocks[2].walking[state.player->RIGHT] = new int[4]{ 4, 5, 6, 7 };
    state.rocks[2].walking[state.player->UP] = new int[4]{ 2, 6, 10, 14 };
    state.rocks[2].walking[state.player->DOWN] = new int[4]{ 0, 4, 8,  12 };

    state.rocks[2].animation_indices = state.rocks[2].walking[state.player->RIGHT];
    state.rocks[2].animation_frames = 4;
    state.rocks[2].animation_index = 0;
    state.rocks[2].animation_time = 0.0f;
    state.rocks[2].animation_cols = 4;
    state.rocks[2].animation_rows = 4;
    state.rocks[2].speed = 19.0f;

    state.rocks[2].set_height(1.5f);
    state.rocks[2].set_width(0.4f);
    state.rocks[2].set_ai_type(WALKER2);

    state.rocks[3].set_entity_type(ROCK);
    state.rocks[3].texture_id = Utility::load_texture("assets/mt.png");
    state.rocks[3].set_position(glm::vec3(-1.566f, -0.78f, 0.0f));
    state.rocks[3].set_acceleration(glm::vec3(0.0f, 0.0f, 0.0f));
    state.rocks[3].move = false;
    state.rocks[3].s_c = 140;
    state.rocks[3].value = 5555;

    state.rocks[3].walking[state.player->RIGHT] = new int[4]{ 0, 1, 2,  3 };
    state.rocks[3].walking[state.player->LEFT] = new int[4]{ 4, 5, 6, 7 };
    state.rocks[3].walking[state.player->UP] = new int[4]{ 2, 6, 10, 14 };
    state.rocks[3].walking[state.player->DOWN] = new int[4]{ 0, 4, 8,  12 };

    state.rocks[3].animation_indices = state.rocks[2].walking[state.player->RIGHT];
    state.rocks[3].animation_frames = 4;
    state.rocks[3].animation_index = 0;
    state.rocks[3].animation_time = 0.0f;
    state.rocks[3].animation_cols = 4;
    state.rocks[3].animation_rows = 4;
    state.rocks[3].speed = 24.5f;

    state.rocks[3].set_height(1.5f);
    state.rocks[3].set_width(0.4f);
    state.rocks[3].set_ai_type(WALKER3);

    state.rocks[4].set_entity_type(ROCK);
    state.rocks[4].texture_id = Utility::load_texture("assets/morty5.png");
    state.rocks[4].set_position(glm::vec3(-3.516f, -1.05f, 0.0f));
    state.rocks[4].set_movement(glm::vec3(0.0f));
    state.rocks[4].speed = 1.0f;
    state.rocks[4].set_acceleration(glm::vec3(0.0f, 0.0f, 0.0f));
    state.rocks[4].move = false;
    state.rocks[4].s_c = 165;
    state.rocks[4].value = 1000;

    state.rocks[5].set_entity_type(ROCK);
    state.rocks[5].texture_id = Utility::load_texture("assets/morty4.png");
    state.rocks[5].set_position(glm::vec3(-1.208f, 0.395f, 0.0f));
    state.rocks[5].set_movement(glm::vec3(0.0f));
    state.rocks[5].speed = 1.0f;
    state.rocks[5].set_acceleration(glm::vec3(0.0f, 0.0f, 0.0f));
    state.rocks[5].move = false;
    state.rocks[5].s_c = 26;
    state.rocks[5].value = 888;

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);

    state.bgm = Mix_LoadMUS("assets/menu.mp3");

    Mix_PlayMusic(state.bgm, -1);

    Mix_VolumeMusic(MIX_MAX_VOLUME / 4.0f);
}

void LevelB::update(float delta_time) {
    this->state.player->update(delta_time, state.player, state.rocks, 6, this->state.map, 2);
    this->state.ship->update(delta_time, state.player, state.rocks, this->ENEMY_COUNT, this->state.map, 2);
    for (int i = 0; i < 6; i++) {
        this->state.rocks[i].update(delta_time, state.player, state.rocks, this->ENEMY_COUNT, this->state.map, 2);
    }
}

void LevelB::render(ShaderProgram* program) {

    float vertices1[] = { -1.4f, -3.64f, -0.1f, -3.64f, -0.1f, -1.5f, -1.4f, -3.64f, -0.1f, -1.5f, -1.4f, -1.5f };
    float vertices2[] = { 0.6f, -3.24f, 1.9f, -3.24f, 1.9f, -1.1f, 0.6f, -3.24f, 1.9f, -1.1f, 0.6f, -1.1f };
    float vertices3[] = { 1.6f, -3.24f, 2.9f, -3.24f, 2.9f, -1.1f, 1.6f, -3.24f, 2.9f, -1.1f, 1.6f, -1.1f };
    float vertices4[] = { -1.4f, -2.64f, -0.1f, -2.64f, -0.1f, -0.5f, -1.4f, -2.64f, -0.1f, -0.5f, -1.4f, -0.5f };
    float verticesP[] = { -1.11f, -4.14f, 1.26f, -4.14f, 1.26f, -0.94f, -1.11f, -4.14f, 1.26f, -0.94f, -1.11f, -0.94f };
    float vertices5[] = { -4.2f, -3.44f, -2.9f, -3.44f, -2.9f, -1.3f, -4.2f, -3.44f, -2.9f, -1.3f, -4.2f, -1.3f };
    float vertices6[] = { 1.7f, 0.94f, 2.4f, 0.94f, 2.4f, 1.8f, 1.7f, 0.94f, 2.4f, 1.8f, 1.7f, 1.8f };

    float vertices_bg[] = { -7.0f, -4.0f, 5.0f, -4.0f, 5.0f, 4.0f, -7.0f, -4.0f, 5.0f, 4.0f, -7.0f, 4.0f };
    this->state.bg->render3(program, vertices_bg);
    this->state.player->render(program);
    this->state.ship->render2(program);
    this->state.rocks[3].render3(program, vertices4);
    this->state.rocks[0].render3(program, vertices1);
    this->state.rocks[1].render3(program, vertices2);
    this->state.rocks[2].render3(program, vertices3);
    this->state.rocks[4].render3(program, vertices5);
    this->state.rocks[5].render3(program, vertices6);
    this->state.enemies[0].render3(program, verticesP);

    Utility::draw_text(program, Utility::load_texture("assets/font1.png"), "Gold:" + std::to_string(this->state.player->money), 0.5f, 0.01f, glm::vec3(-4.8f, 3.0f, 0.0f), true);
    Utility::draw_text(program, Utility::load_texture("assets/font1.png"), "Goal:10000", 0.5f, 0.01f, glm::vec3(-4.8f, 2.0f, 0.0f), true);
    Utility::draw_text(program, Utility::load_texture("assets/font1.png"), std::to_string(this->state.player->countdown), 0.5f, 0.01f, glm::vec3(4.0f, 3.5f, 0.0f), true);
    Utility::draw_text(program, Utility::load_texture("assets/font1.png"), "YOU WIN!", 0.5f, 0.01f, glm::vec3(-1.0f, 1.0f, 0.0f), this->state.player->won);
    Utility::draw_text(program, Utility::load_texture("assets/font1.png"), "YOU LOSE!", 0.5f, 0.01f, glm::vec3(-1.0f, 1.0f, 0.0f), this->state.player->lost);
}
