#define GL_SILENCE_DEPRECATION

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

Entity::Entity(){
    position     = glm::vec3(0.0f);
    velocity     = glm::vec3(0.0f);
    acceleration = glm::vec3(0.0f);
    
    movement = glm::vec3(0.0f);
    
    speed = 0;
    model_matrix = glm::mat4(1.0f);
}

Entity::~Entity(){
    delete [] animation_up;
    delete [] animation_down;
    delete [] animation_left;
    delete [] animation_right;
    delete [] walking;
}

void Entity::draw_sprite_from_texture_atlas(ShaderProgram *program, GLuint texture_id, int index){
    float u_coord = (float) (index % animation_cols) / (float) animation_cols;
    float v_coord = (float) (index / animation_cols) / (float) animation_rows;

    float width = 1.0f / (float) animation_cols;
    float height = 1.0f / (float) animation_rows;
    
    float tex_coords[] ={
        u_coord, v_coord + height, u_coord + width, v_coord + height, u_coord + width, v_coord,
        u_coord, v_coord + height, u_coord + width, v_coord, u_coord, v_coord
    };
    
    float vertices[] ={
        -0.5, -0.5, 0.5, -0.5,  0.5, 0.5,
        -0.5, -0.5, 0.5,  0.5, -0.5, 0.5
    };
    
    // Step 4: And render
    glBindTexture(GL_TEXTURE_2D, texture_id);
    
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);
    
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, tex_coords);
    glEnableVertexAttribArray(program->texCoordAttribute);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}

void Entity::activate_ai(Entity *player){
    switch (ai_type){

        case WALKER:
            ai_walker();
            break;
            
        case GUARD:
            ai_guard(player);
            break;

        case BOMBER:
            ai_bomber(player);
            break;

        case SENTINEL:
            ai_sentinel();

        default:
            break;
    }
}

void Entity::ai_sentinel() {
    movement = glm::vec3(0.0f, 0.0f, 0.0f);
}

void Entity::ai_walker(){
    if (ai_frames >= 150) {
        ai_walk_left *= -1.0f;
        ai_frames = 0;
        movement = glm::vec3(ai_walk_left, 0.0f, 0.0f);
        if (animation_indices == walking[RIGHT]) {
            animation_indices = walking[LEFT];
        }
        else {
            animation_indices = walking[RIGHT];
        }
    }
    ai_frames++;
}

void Entity::ai_guard(Entity *player){
    switch (ai_state) {
        case IDLE:
            if (glm::distance(position, player->position) < 3.0f) ai_state = WALKING;
            break;
            
        case WALKING:
            if (position.x > player->get_position().x) {
                animation_indices = walking[LEFT];
                movement = glm::vec3(-1.0f, 0.0f, 0.0f);
            } else if (position.x < player->get_position().x) {
                animation_indices = walking[RIGHT];
                movement = glm::vec3(1.0f, 0.0f, 0.0f);
            }
            break;
            
            
        default:
            break;
    }
}

void Entity::ai_bomber(Entity* player){
    switch (ai_state) {
    case IDLE:
        activate();
        if (glm::distance(position, player->position) < 4.1f) ai_state = ATTACKING;
        break;

    case ATTACKING:
        activate();
        movement = glm::vec3(-1.3f, 0.0f, 0.0f);
        if (player->position.x - position.x > 10.0f) {
            ai_state = MISS;
        }
        break;

    case MISS:
        movement = glm::vec3(0.0f, 0.0f, 0.0f);
        break;

    default:
        break;
    }
}

void Entity::update(float delta_time, Entity *player, Entity *objects, int object_count, Map *map){
    for (int i = 0; i < object_count; i++) {
        if (objects[i].entity_type == ENEMY && objects[i].stop) {
            for (int k = 0; k < object_count; k++) {
                objects[k].stop = true;
            }
        }
    }
    if (!is_active) return;
    if (entity_type == ENEMY && stop) { return; };
    if (entity_type == PLAYER && lost) { return; };
 
    collided_top    = false;
    collided_bottom = false;
    collided_left   = false;
    collided_right  = false;
    
    if (entity_type == ENEMY || entity_type == ROCKET) activate_ai(player);
    
    if (animation_indices != NULL){
        if (glm::length(movement) != 0)
        {
            animation_time += delta_time;
            float frames_per_second = (float) 1 / SECONDS_PER_FRAME;
            
            if (animation_time >= frames_per_second)
            {
                animation_time = 0.0f;
                animation_index++;
                
                if (animation_index >= animation_frames)
                {
                    animation_index = 0;
                }
            }
        }
    }
    
    // Our character moves from left to right, so they need an initial velocity
    velocity.x = movement.x * speed;
    
    // Now we add the rest of the gravity physics
    velocity += acceleration * delta_time;
    
    if (entity_type == ENEMY || entity_type == PLAYER) {
        check_collision_y(objects, object_count);
        position.y += velocity.y * delta_time;
        check_collision_y(map);

        position.x += velocity.x * delta_time;
        check_collision_x(objects, object_count);
        check_collision_x(map);
    }
    else {
        position.x += velocity.x * delta_time;
        if (check_collision(player)) {
            deactivate();
            player->lost = true;
        }
    }

    // Jump
    if (is_jumping){
        // STEP 1: Immediately return the flag to its original false state
        is_jumping = false;
        
        // STEP 2: The player now acquires an upward velocity
        velocity.y += jumping_power;
    }


    model_matrix = glm::mat4(1.0f);
    model_matrix = glm::translate(model_matrix, position);

    he_win();
    for (int i = 0; i < object_count; i++) {
        if (objects[i].entity_type == ENEMY) {
            if (objects[i].is_active == true) {
                he_lose();
            }
        }
    }
}

void const Entity::check_collision_y(Entity *collidable_entities, int collidable_entity_count){
    for (int i = 0; i < collidable_entity_count; i++){
        Entity *collidable_entity = &collidable_entities[i];
        if (check_collision(collidable_entity)){
            float y_distance = fabs(position.y - collidable_entity->position.y);
            float y_overlap = fabs(y_distance - (height / 2.0f) - (collidable_entity->height / 2.0f));
            if (velocity.y > 0 && abs(position.x - collidable_entity->position.x) <0.5f) {
                velocity.y    = 0;
                collided_top  = true;
            } else if (velocity.y < 0 && abs(position.x-collidable_entity->position.x)< 0.5f && abs(position.y - collidable_entity->position.y) > 0.4f) {
                velocity.y       = 0;
                collided_bottom  = true;
                collidable_entity->is_active = false;
            }
        }
    }
}

void const Entity::check_collision_x(Entity *collidable_entities, int collidable_entity_count){
    for (int i = 0; i < collidable_entity_count; i++){
        Entity *collidable_entity = &collidable_entities[i];
        
        if (check_collision(collidable_entity)){
            float x_distance = fabs(position.x - collidable_entity->position.x);
            float x_overlap = fabs(x_distance - (width / 2.0f) - (collidable_entity->width / 2.0f));
            if (collidable_entity->position.x > position.x && abs(position.y - collidable_entity->position.y) < 0.2f) { 
                velocity.x      = 0;
                collided_right  = true;
                collidable_entity->collided_left = true;
                collidable_entity->stop = true;
                lost = true;
            } else if (collidable_entity->position.x < position.x && abs(position.y - collidable_entity->position.y) < 0.2f) { 
                velocity.x     = 0;
                collided_left  = true;
                collidable_entity->collided_right = true;
                lost = true;
                collidable_entity->stop = true;
            }
        }
    }
}

void const Entity::check_collision_y(Map *map){
    // Probes for tiles
    glm::vec3 top = glm::vec3(position.x, position.y + (height / 2), position.z);
    glm::vec3 top_left = glm::vec3(position.x - (width / 2), position.y + (height / 2), position.z);
    glm::vec3 top_right = glm::vec3(position.x + (width / 2), position.y + (height / 2), position.z);
    
    glm::vec3 bottom = glm::vec3(position.x, position.y - (height / 2), position.z);
    glm::vec3 bottom_left = glm::vec3(position.x - (width / 2), position.y - (height / 2), position.z);
    glm::vec3 bottom_right = glm::vec3(position.x + (width / 2), position.y - (height / 2), position.z);
    
    float penetration_x = 0;
    float penetration_y = 0;
    
    if (map->is_solid(top, &penetration_x, &penetration_y) && velocity.y > 0){
        position.y -= penetration_y;
        velocity.y = 0;
        collided_top = true;
    }
    else if (map->is_solid(top_left, &penetration_x, &penetration_y) && velocity.y > 0){
        position.y -= penetration_y;
        velocity.y = 0;
        collided_top = true;
    }
    else if (map->is_solid(top_right, &penetration_x, &penetration_y) && velocity.y > 0){
        position.y -= penetration_y;
        velocity.y = 0;
        collided_top = true;
    }
    
    if (map->is_solid(bottom, &penetration_x, &penetration_y) && velocity.y < 0){
    position.y += penetration_y;
    velocity.y = 0;
    collided_bottom = true;
    }
    else if (map->is_solid(bottom_left, &penetration_x, &penetration_y) && velocity.y < 0){
        position.y += penetration_y;
        velocity.y = 0;
        collided_bottom = true;
    }
    else if (map->is_solid(bottom_right, &penetration_x, &penetration_y) && velocity.y < 0){
    position.y += penetration_y;
    velocity.y = 0;
        collided_bottom = true;
        
    }
}

void const Entity::check_collision_x(Map *map){
    // Probes for tiles
    glm::vec3 left = glm::vec3(position.x - (width / 2), position.y, position.z);
    glm::vec3 right = glm::vec3(position.x + (width / 2), position.y, position.z);
    
    float penetration_x = 0;
    float penetration_y = 0;
    
    if (map->is_solid(left, &penetration_x, &penetration_y) && velocity.x < 0){
        position.x += penetration_x;
        velocity.x = 0;
        collided_left = true;
}
    if (map->is_solid(right, &penetration_x, &penetration_y) && velocity.x > 0){
        position.x -= penetration_x;
        velocity.x = 0;
        collided_right = true;
    }
}

void Entity::render(ShaderProgram *program){
    if (!is_active) return;
    
    program->SetModelMatrix(model_matrix);
    
    if (animation_indices != NULL){
        draw_sprite_from_texture_atlas(program, texture_id, animation_indices[animation_index]);
        return;
    }
    
    float vertices[]   = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float tex_coords[] = {  0.0,  1.0, 1.0,  1.0, 1.0, 0.0,  0.0,  1.0, 1.0, 0.0,  0.0, 0.0 };
    
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
    if (!is_active) return;

    program->SetModelMatrix(model_matrix);

    if (animation_indices != NULL){
        draw_sprite_from_texture_atlas(program, texture_id, animation_indices[animation_index]);
        return;
    }

    float vertices[] = { -0.14, -0.14, 0.14, -0.14, 0.14, 0.14, -0.14, -0.14, 0.14, 0.14, -0.14, 0.14 };
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

bool const Entity::check_collision(Entity *other) const{

    if (other == this) return false;
    
    if (!is_active || !other->is_active) return false;
    
    float x_distance = fabs(position.x - other->position.x) - ((width  + other->width)  / 2.0f);
    float y_distance = fabs(position.y - other->position.y) - ((height + other->height) / 2.0f);
    
    return x_distance < -0.09f && y_distance < -0.3f;
}
