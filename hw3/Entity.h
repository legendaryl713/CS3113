#pragma once
#ifndef ENTITY_CLASS
#define ENTITY_CLASS


class Entity{
public:
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acceleration;

    bool is_active = true;

    float width = 1;
    float height = 1;

    float speed;
    glm::vec3 movement;

    GLuint texture_id = NULL;

    glm::mat4 model_matrix;

    Entity();
    ~Entity();

    void update(float delta_time, Entity* collidable_entities, int collidable_intity_count, bool moving, bool& touches_rock, bool& touches_platform);
    void render(ShaderProgram* program);
    void render2(ShaderProgram* program);
    bool const check_collision(Entity* other) const;

    glm::vec3 const get_position()     const { return position; };
    glm::vec3 const get_movement()     const { return movement; };
    glm::vec3 const get_velocity()     const { return velocity; };
    glm::vec3 const get_acceleration() const { return acceleration; };

    float const get_width()  const { return width; }
    float const get_height() const { return height; }

    void const set_position(glm::vec3 new_position) { position = new_position; };
    void const set_movement(glm::vec3 new_movement) { movement = new_movement; };
    void const set_velocity(glm::vec3 new_velocity) { velocity = new_velocity; };
    void const set_acceleration(glm::vec3 new_acceleration) { acceleration = new_acceleration; };
    void const set_width(float new_width) { width = new_width; };
    void const set_height(float new_height) { height = new_height; };
};

#endif