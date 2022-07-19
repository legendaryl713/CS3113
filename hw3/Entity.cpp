#define GL_SILENCE_DEPRECATION
#define STB_IMAGE_IMPLEMENTATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "Entity.h"
#include <vector>

Entity::Entity(){
    position = glm::vec3(0.0f);
    velocity = glm::vec3(0.0f);
    acceleration = glm::vec3(0.0f);
    movement = glm::vec3(0.0f);
    speed = 0;
    model_matrix = glm::mat4(1.0f);
}

Entity::~Entity(){
}

void Entity::update(float delta_time, Entity* collidable_entities, int collidable_entity_count, bool moving, bool& touches_rock, bool& touches_platform){

    bool collided = false;
    
    for (int i = 0; i < collidable_entity_count; i++)
    {
        Entity* collidable_entity = &collidable_entities[i];

        if (check_collision(collidable_entity)){
            collided = true;
            float y_distance = fabs(position.y - collidable_entity->position.y);
            float y_overlap = fabs(y_distance - (height / 2.0f) - (collidable_entity->height / 2.0f));
            if (velocity.y > 0) {
                //position.y -= y_overlap;
                velocity.y = 0;
            }
            else if (velocity.y < 0) {
                //position.y += y_overlap;
                velocity.y = 0;
            }
            if (collidable_entity->get_position().x == 0.2f) {
                touches_platform = true;
            }
            else {
                touches_rock = true;
            }
        }
    }
    
    
    velocity.x = movement.x * speed;
    acceleration += movement * 3.0f;
    velocity += collided == false ? acceleration * delta_time : glm::vec3(0.0f);
    position += moving ? velocity * delta_time : glm::vec3(0.0f);
    std::cout << delta_time;
    model_matrix = glm::mat4(1.0f);
    model_matrix = glm::translate(model_matrix, position);
}

void Entity::render(ShaderProgram* program){
    if (!is_active) return;

    program->SetModelMatrix(model_matrix);

    float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float tex_coords[] = { 0.0,  1.0, 1.0,  1.0, 1.0, 0.0,  0.0,  1.0, 1.0, 0.0,  0.0, 0.0 };

    glBindTexture(GL_TEXTURE_2D, texture_id);

    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, tex_coords);
    glEnableVertexAttribArray(program->texCoordAttribute);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}

void Entity::render2(ShaderProgram* program) {
    program->SetModelMatrix(model_matrix);

    float vertices[] = { -0.8, -0.8, 0.8, -0.8, 0.8, 0.95, -0.8, -0.8, 0.8, 0.95, -0.8, 0.95 };
    float tex_coords[] = { 0.0,  1.0, 1.0,  1.0, 1.0, 0.0,  0.0,  1.0, 1.0, 0.0,  0.0, 0.0 };

    glBindTexture(GL_TEXTURE_2D, texture_id);

    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, tex_coords);
    glEnableVertexAttribArray(program->texCoordAttribute);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}

bool const Entity::check_collision(Entity* other) const{
    float x_distance = fabs(position.x - other->position.x) - ((width + other->width) / 2.0f);
    float y_distance = fabs(position.y - other->position.y) - ((height + other->height) / 2.0f);
    return x_distance < -0.8f && y_distance < -0.34f;
}
