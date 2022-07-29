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
    2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,1,2,2,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,3,3,3,3,
    2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1,2,2,2,0,3,3,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,3,3,3,3,

};
LevelA::~LevelA()
{
    delete [] this->state.enemies;
    delete    this->state.player;
    delete    this->state.map;
    Mix_FreeChunk(this->state.jump_sfx);
    Mix_FreeMusic(this->state.bgm);
}

void LevelA::initialise(){
    GLuint font_texture_id = Utility::load_texture("assets/font1.png");

    GLuint map_texture_id = Utility::load_texture("assets/t2.png");
    this->state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, LEVEL_DATA, map_texture_id, 1.0f, 4, 1);
    
    //player
    state.player = new Entity();
    state.player->set_entity_type(PLAYER);
    state.player->set_position(glm::vec3(1.0f, 0.0f, 0.0f));
    state.player->set_movement(glm::vec3(0.0f));
    state.player->speed = 2.5f;
    state.player->set_acceleration(glm::vec3(0.0f, -9.81f, 0.0f));
    state.player->texture_id = Utility::load_texture("assets/warrior.png");

    state.player->walking[state.player->LEFT]  = new int[4]{ 4, 5, 6, 7 };
    state.player->walking[state.player->RIGHT] = new int[4]{ 8, 9, 10,  11 };
    state.player->walking[state.player->UP]    = new int[4]{ 12, 13, 14, 15 };
    state.player->walking[state.player->DOWN]  = new int[4]{ 0, 1, 2,  3 };

    state.player->animation_indices = state.player->walking[state.player->RIGHT];  // start George looking left
    state.player->animation_frames = 4;
    state.player->animation_index  = 0;
    state.player->animation_time   = 0.0f;
    state.player->animation_cols   = 4;
    state.player->animation_rows   = 4;
    state.player->set_height(0.8f);
    state.player->set_width(0.55f);
    
    // Jumping
    state.player->jumping_power = 5.3f;
    
    //enemies
    Utility::load_texture("assets/dragon.png");
    
    state.enemies = new Entity[3];
    state.enemies[0].set_entity_type(ENEMY);
    state.enemies[0].set_ai_type(WALKER);
    state.enemies[0].set_ai_state(IDLE);
    state.enemies[0].texture_id = Utility::load_texture("assets/ghost.png");
    state.enemies[0].set_position(glm::vec3(10.5f, -5.5f, 0.0f));
    state.enemies[0].set_movement(glm::vec3(0.0f));
    state.enemies[0].speed = 1.0f;
    state.enemies[0].set_acceleration(glm::vec3(0.0f, -9.81f, 0.0f));

    state.enemies[0].walking[state.enemies[0].LEFT] = new int[3]{ 3,4,5 };
    state.enemies[0].walking[state.enemies[0].RIGHT] = new int[3]{ 6,7,8 };
    state.enemies[0].walking[state.enemies[0].UP] = new int[3]{ 9,10,11 };
    state.enemies[0].walking[state.enemies[0].DOWN] = new int[3]{ 0,1,2 };

    state.enemies[0].animation_indices = state.enemies[0].walking[state.enemies[0].RIGHT];
    
    state.enemies[0].animation_frames = 3;
    state.enemies[0].animation_index = 0;
    state.enemies[0].animation_time = 0.0f;
    state.enemies[0].animation_cols = 3;
    state.enemies[0].animation_rows = 4;
    state.enemies[0].set_height(1.1f);
    state.enemies[0].set_width(0.6f);




    state.enemies[1].set_entity_type(ENEMY);
    state.enemies[1].set_ai_type(GUARD);
    state.enemies[1].set_ai_state(IDLE);
    state.enemies[1].texture_id = Utility::load_texture("assets/dragon.png");
    state.enemies[1].set_position(glm::vec3(22.0f, 0.0f, 0.0f));
    state.enemies[1].set_movement(glm::vec3(0.0f));
    state.enemies[1].speed = 1.0f;
    state.enemies[1].set_acceleration(glm::vec3(0.0f, -9.81f, 0.0f));

    state.enemies[1].walking[state.enemies[0].LEFT] = new int[3]{ 3,4,5 };
    state.enemies[1].walking[state.enemies[0].RIGHT] = new int[3]{ 6,7,8 };
    state.enemies[1].walking[state.enemies[0].UP] = new int[3]{ 9,10,11 };
    state.enemies[1].walking[state.enemies[0].DOWN] = new int[3]{ 0,1,2 };

    state.enemies[1].animation_indices = state.enemies[0].walking[state.enemies[0].RIGHT];

    state.enemies[1].animation_frames = 3;
    state.enemies[1].animation_index = 0;
    state.enemies[1].animation_time = 0.0f;
    state.enemies[1].animation_cols = 3;
    state.enemies[1].animation_rows = 4;
    state.enemies[1].set_height(0.95f);
    state.enemies[1].set_width(1.0f);


    state.enemies[2].set_entity_type(ENEMY);
    state.enemies[2].set_ai_type(SENTINEL);
    state.enemies[2].set_ai_state(IDLE);
    state.enemies[2].texture_id = Utility::load_texture("assets/boss.png");
    state.enemies[2].set_position(glm::vec3(35.0f, -5.0f, 0.0f));
    state.enemies[2].set_movement(glm::vec3(0.0f));
    state.enemies[2].speed = 0.0f;
    state.enemies[2].set_acceleration(glm::vec3(0.0f, -9.81f, 0.0f));

    state.enemies[2].set_height(0.8f);
    state.enemies[2].set_width(0.5f);

    //rocket stuff
    state.rocket = new Entity();
    state.rocket->set_entity_type(ROCKET);
    state.rocket->set_ai_type(BOMBER);
    state.rocket->set_ai_state(IDLE);
    state.rocket->set_position(glm::vec3(35.0f, -5.0f, 0.0f));
    state.rocket->set_movement(glm::vec3(0.0f));
    state.rocket->speed = 2.63f;
    state.rocket->set_acceleration(glm::vec3(0.0f, 0.0f, 0.0f));
    state.rocket->texture_id = Utility::load_texture("assets/rocket.png");


    state.rocket->set_height(0.2f);
    state.rocket->set_width(0.85f);

    
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    
    state.jump_sfx = Mix_LoadWAV("assets/hop.wav");
}

void LevelA::update(float delta_time) { 
    this->state.player->update(delta_time, state.player, state.enemies, this->ENEMY_COUNT, this->state.map);
    this->state.enemies[0].update(delta_time, state.player, state.enemies, this->ENEMY_COUNT, this->state.map);
    this->state.enemies[1].update(delta_time, state.player, state.enemies, this->ENEMY_COUNT, this->state.map);
    this->state.enemies[2].update(delta_time, state.player, state.enemies, this->ENEMY_COUNT, this->state.map);
    this->state.rocket->update(delta_time, state.player, NULL, 0, this->state.map);
}

void LevelA::render(ShaderProgram *program)
{
    this->state.map->render(program);
    this->state.player->render(program);
    this->state.enemies[0].render(program);
    glm::vec3 v = this->state.player->get_position();
    v.y += 4.0f;
    v.x -= 1.0f;
    this->state.enemies[1].render(program);
    this->state.rocket->render2(program);
    this->state.enemies[2].render(program);
    Utility::draw_text(program, Utility::load_texture("assets/font1.png"), "YOU WIN!", 0.5f, 0.01f, v, this->state.player->won);
    Utility::draw_text(program, Utility::load_texture("assets/font1.png"), "YOU LOST!", 0.5f, 0.01f, v, this->state.player->lost);
}
