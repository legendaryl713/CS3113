#include "LevelC.h"
#include "Utility.h"

#define LEVEL_WIDTH 28
#define LEVEL_HEIGHT 12

unsigned int LEVEL_DATA2[] =
{
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,0,0,0,0,0,0,0,0,0,3,
    3, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,0,0,0,0,0,0,0,0,0,0,3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0,0,0,0,0,0,0,0,0,0,0,3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,0,1,0,0,0,0,0,0,0,3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,1,0,0,0,0,0,0,0,0,0,3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,0,0,0,0,0,0,0,0,0,0,3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,0,0,0,0,0,0,0,0,0,0,3,
    3, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0,0,0,0,0,0,0,0,0,0,0,3,
    3, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,0,0,0,0,0,0,0,0,0,3,
    3, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1,1,1,1,1,1,1,0,0,0,3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,0,0,0,0,0,0,0,0,1,3,
    3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1,1,1,1,1,1,1,1,1,1,3,

};
LevelC::~LevelC()
{
    delete[] this->state.enemies;
    delete    this->state.player;
    delete    this->state.map;
    Mix_FreeChunk(this->state.jump_sfx);
    Mix_FreeMusic(this->state.bgm);
}

void LevelC::initialise() {
    GLuint font_texture_id = Utility::load_texture("assets/font1.png");

    state.bg = new Entity();
    state.bg->set_entity_type(BG);
    state.bg->texture_id = Utility::load_texture("assets/walls.png");
    state.bg->set_position(glm::vec3(-2.5f, -1.0f, 0.0f));

    GLuint map_texture_id = Utility::load_texture("assets/t3.png");
    this->state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, LEVEL_DATA2, map_texture_id, 1.0f, 4, 1);

    //player
    state.player = new Entity();
    state.player->set_entity_type(PLAYER);
    state.player->set_position(glm::vec3(2.0f, 55.0f, 0.0f));
    state.player->set_movement(glm::vec3(0.0f));
    state.player->speed = 2.5f;
    state.player->set_acceleration(glm::vec3(0.0f, -9.81f, 0.0f));
    state.player->texture_id = Utility::load_texture("assets/rk.png");

    state.player->walking[state.player->LEFT] = new int[4]{ 0,1,2,3 };
    state.player->walking[state.player->RIGHT] = new int[4]{ 4,5,6,7 };
    state.player->walking[state.player->UP] = new int[4]{ 12, 13, 14, 15 };
    state.player->walking[state.player->DOWN] = new int[4]{ 0, 1, 2,  3 };

    state.player->animation_indices = state.player->walking[state.player->RIGHT];  // start George looking left
    state.player->animation_frames = 4;
    state.player->animation_index = 0;
    state.player->animation_time = 0.0f;
    state.player->animation_cols = 4;
    state.player->animation_rows = 4;
    state.player->set_height(0.8f);
    state.player->set_width(0.55f);

    state.player2 = new Entity();
    state.player2->set_entity_type(PLAYER);
    state.player2->set_position(glm::vec3(2.0f, 55.0f, 0.0f));
    state.player2->set_movement(glm::vec3(0.0f));
    state.player2->speed = 2.5f;
    state.player2->set_acceleration(glm::vec3(0.0f, -9.81f, 0.0f));
    state.player2->texture_id = Utility::load_texture("assets/mt.png");

    state.player2->walking[state.player->LEFT] = new int[4]{ 0,1,2,3 };
    state.player2->walking[state.player->RIGHT] = new int[4]{ 4,5,6,7 };
    state.player2->walking[state.player->UP] = new int[4]{ 12, 13, 14, 15 };
    state.player2->walking[state.player->DOWN] = new int[4]{ 0, 1, 2,  3 };

    state.player2->animation_indices = state.player->walking[state.player->RIGHT];  // start George looking left
    state.player2->animation_frames = 4;
    state.player2->animation_index = 0;
    state.player2->animation_time = 0.0f;
    state.player2->animation_cols = 4;
    state.player2->animation_rows = 4;
    state.player2->set_height(0.8f);
    state.player2->set_width(0.55f);

    // Jumping
    state.player->jumping_power = 6.3f;
    state.player2->jumping_power = 5.3f;

    state.rocks = new Entity;
    state.rocks->set_entity_type(ROCK);
    state.rocks->texture_id = Utility::load_texture("assets/portal.png");

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);

    state.jump_sfx = Mix_LoadWAV("assets/bounce.wav");

    state.bgm = Mix_LoadMUS("assets/level_c.mp3");

    Mix_PlayMusic(state.bgm, -1);

    Mix_VolumeMusic(MIX_MAX_VOLUME / 4.0f);
}

void LevelC::update(float delta_time) {
    this->state.player->update2(delta_time, state.player, state.enemies, 1, this->state.map, 3, NULL);
    this->state.player2->update2(delta_time, state.player2, state.enemies, 1, this->state.map, 3, NULL);
}

void LevelC::render(ShaderProgram* program){
    float vertices_bg[] = { -5.0f, -14.0f, 7.0f, -14.0f, 7.0f, -1.0f, -5.0f, -14.0f, 7.0f, -1.0f, -5.0f, -1.0f };
    float vertices_bg1[] = {7.0f, -14.0f, 19.0f, -14.0f, 19.0f, -1.0f, 7.0f, -14.0f, 19.0f, -1.0f, 7.0f, -1.0f };
    float vertices_bg2[] = { 19.0f, -14.0f, 31.0f, -14.0f, 31.0f, -1.0f, 19.0f, -14.0f, 31.0f, -1.0f, 19.0f, -1.0f };
    float vertices_bg3[] = { -5.0f, -1.0f, 7.0f, -1.0f, 7.0f, 12.0f, -5.0f, -1.0f, 7.0f, 12.0f, -5.0f, 12.0f };
    float vertices_bg4[] = { 7.0f, -1.0f, 19.0f, -1.0f, 19.0f, 12.0f, 7.0f, -1.0f, 19.0f, 12.0f, 7.0f, 12.0f };
    float vertices_bg5[] = { 19.0f, -1.0f, 31.0f, -1.0f, 31.0f, 12.0f, 19.0f, -1.0f, 31.0f, 12.0f, 19.0f, 12.0f };

    float vertices1[] = { 5.4f, -9.34f, 6.7f, -9.34f, 6.7f, -7.2f, 5.4f, -9.34f, 6.7f, -7.2f, 5.4f, -7.2f };
    float vertices2[] = { 0.5f, -5.48f, 2.5f, -5.48f, 2.5f, -4.5f, 0.5f, -5.48f, 2.5f, -4.5f, 0.5f, -4.5f };
    float vertices3[] = { 0.7f, -0.88f, 2.3f, -0.88f, 2.3f, 3.7f, 0.7f, -0.88f, 2.3f, 3.7f, 0.5f, 3.7f };
    this->state.bg->render3(program, vertices_bg);
    this->state.bg->render3(program, vertices_bg1);
    this->state.bg->render3(program, vertices_bg2);
    this->state.bg->render3(program, vertices_bg3);
    this->state.bg->render3(program, vertices_bg4);
    this->state.bg->render3(program, vertices_bg5);
    this->state.map->render(program);
    this->state.player->render(program);
    this->state.player2->render(program);
    this->state.rocks->render3(program, vertices3);
}