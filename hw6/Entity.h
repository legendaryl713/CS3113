#pragma once
#include "Map.h"

enum EntityType { PLATFORM, PLAYER, ENEMY, SHIP, ROCK, BG};
enum AIType { WALKER };
enum AIState { WALKING, IDLE, ATTACKING, MISS };

class Entity
{
private:
    bool is_active = true;
    EntityType entity_type;
    AIType ai_type;
    AIState ai_state;

    int* animation_right = NULL; // move to the right
    int* animation_left = NULL; // move to the left
    int* animation_up = NULL; // move upwards
    int* animation_down = NULL; // move downwards

    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acceleration;

    float width = 0.8f;
    float height = 0.8f;

public:
    //bool grabbed = false;
    int money = 0;
    int value = 0;

    int frame_counter = 0;
    bool go_left = false;
    int scale_counter = 0;
    int shrink_counter = 0;
    int s_c = 0;
    int time_to_shrink = false;
    bool move = true;
    int moment_to_go = 0;

    int bounce = false;
    float lives = 0.0f;
    float lives2 = 0.0f;
    bool won = false;
    bool lvl2_won = false;
    bool lvl1_won = false;
    bool lvl3_won = false;
    bool lost = false;
    bool stop = false;
    // Static attributes
    static const int SECONDS_PER_FRAME = 4;
    static const int LEFT = 0,
        RIGHT = 1,
        UP = 2,
        DOWN = 3;

    // Existing
    GLuint texture_id;
    glm::mat4 model_matrix;

    // Translating
    float speed;
    glm::vec3 movement;

    // Animating
    int ai_frames = 0;
    float ai_walk_left = 1.0f;
    int** walking = new int* [4]{ animation_left, animation_right, animation_up, animation_down};
    int* animation_indices = NULL;
    int animation_frames = 0;
    int animation_index = 0;
    float animation_time = 0.0f;
    int animation_cols = 0;
    int animation_rows = 0;

    // Jumping
    bool is_jumping = false;
    float jumping_power = 0;

    // Colliding
    bool collided_top = false;
    bool collided_bottom = false;
    bool collided_left = false;
    bool collided_right = false;

    // Methods
    Entity();
    ~Entity();

    void draw_sprite_from_texture_atlas(ShaderProgram* program, GLuint texture_id, int index);
    void update(float delta_time, Entity* player, Entity* objects, int object_count, Map* map, int level);
    void render(ShaderProgram* program);
    void render2(ShaderProgram* program);
    void render3(ShaderProgram* program, float vertices[]);
    void activate_ai(Entity* player);
    void ai_walker();
    void he_win() { lvl1_won = true; };
    void he_didnt_win() { lvl1_won = false; };
    void he_win2() { lvl2_won = true; };
    void he_didnt_win2() { lvl2_won = false; };
    void he_win3() { lvl3_won = true; };
    void he_didnt_win3() { lvl3_won = false; };
    void change_life(float life) { lives = life; };
    void change_life2(float life2) { lives2 = life2; };
    void change_money(int cash) { money = cash; };
    void change_value(int val) { value = val; };

    //void transfer_s_c(Entity* entity);
    void const check_collision_y(Entity* collidable_entities, int collidable_entity_count);
    void const check_collision_x(Entity* collidable_entities, int collidable_entity_count);
    void const check_collision_y(Map* map);
    void const check_collision_x(Map* map);

    //bool const check_collision(Entity* other) const;
    bool const check_collision(Entity* other) const;

    void activate() { is_active = true; };
    void deactivate() { is_active = false; };

    EntityType const get_entity_type()  const { return entity_type; };
    AIType     const get_ai_type()      const { return ai_type; };
    AIState    const get_ai_state()     const { return ai_state; };
    glm::vec3  const get_position()     const { return position; };
    glm::vec3  const get_movement()     const { return movement; };
    glm::vec3  const get_velocity()     const { return velocity; };
    glm::vec3  const get_acceleration() const { return acceleration; };
    int        const get_width()        const { return width; };
    int        const get_height()       const { return height; };

    void const set_entity_type(EntityType new_entity_type) { entity_type = new_entity_type; };
    void const set_ai_type(AIType new_ai_type) { ai_type = new_ai_type; };
    void const set_ai_state(AIState new_state) { ai_state = new_state; };
    void const set_position(glm::vec3 new_position) { position = new_position; };
    void const set_movement(glm::vec3 new_movement) { movement = new_movement; };
    void const set_velocity(glm::vec3 new_velocity) { velocity = new_velocity; };
    void const set_acceleration(glm::vec3 new_acceleration) { acceleration = new_acceleration; };
    void const set_width(float new_width) { width = new_width; };
    void const set_height(float new_height) { height = new_height; };
};
