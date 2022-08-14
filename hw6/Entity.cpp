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
#include "Utility.h"

Entity::Entity() {
    position = glm::vec3(0.0f);
    velocity = glm::vec3(0.0f);
    acceleration = glm::vec3(0.0f);

    movement = glm::vec3(0.0f);

    speed = 0;
    model_matrix = glm::mat4(1.0f);
}

Entity::~Entity() {
    delete[] animation_up;
    delete[] animation_down;
    delete[] animation_left;
    delete[] animation_right;
    delete[] walking;
}

void Entity::draw_sprite_from_texture_atlas(ShaderProgram* program, GLuint texture_id, int index) {
    float u_coord = (float)(index % animation_cols) / (float)animation_cols;
    float v_coord = (float)(index / animation_cols) / (float)animation_rows;

    float width = 1.0f / (float)animation_cols;
    float height = 1.0f / (float)animation_rows;

    float tex_coords[] = {
        u_coord, v_coord + height, u_coord + width, v_coord + height, u_coord + width, v_coord,
        u_coord, v_coord + height, u_coord + width, v_coord, u_coord, v_coord
    };

    float vertices[] = {
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

void Entity::draw_sprite_from_texture_atlas2(ShaderProgram* program, GLuint texture_id, int index, float vertices[]) {
    float u_coord = (float)(index % animation_cols) / (float)animation_cols;
    float v_coord = (float)(index / animation_cols) / (float)animation_rows;

    float width = 1.0f / (float)animation_cols;
    float height = 1.0f / (float)animation_rows;

    float tex_coords[] = {
        u_coord, v_coord + height, u_coord + width, v_coord + height, u_coord + width, v_coord,
        u_coord, v_coord + height, u_coord + width, v_coord, u_coord, v_coord
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

void Entity::activate_ai(Entity* player) {
    switch (ai_type) {

    case WALKER:
        ai_walker();
        break;
    case WALKER2:
        ai_walker2();
        break;
    case WALKER3:
        ai_walker3();
        break;
    default:
        break;
    }
}

void Entity::ai_walker() {
    if (ai_frames >= 220) {
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
    model_matrix = glm::translate(model_matrix, glm::vec3(ai_walk_left, 0.0f, 0.0f));
}

void Entity::ai_walker2() {
    if (ai_frames >= 450) {
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
    model_matrix = glm::translate(model_matrix, glm::vec3(ai_walk_left, ai_walk_left / -6.0f, 0.0f));
}

void Entity::ai_walker3() {
    if (ai_frames >= 400) {
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
    model_matrix = glm::translate(model_matrix, glm::vec3(ai_walk_left, 0.0f, 0.0f));
}

void Entity::update(float delta_time, Entity* player, Entity* objects, int object_count, Map* map, int level) {
    if (animation_indices != NULL)
    {
        if (glm::length(movement) != 0)
        {
            animation_time += delta_time;
            float frames_per_second = (float)1 / SECONDS_PER_FRAME;

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

    velocity.x = movement.x * speed;
    velocity.y = movement.y * speed;
    if (entity_type == PLAYER) {
        times++;
        if (times % 70 == 0 && times >= 70) {
            countdown--;
        }
        if (countdown <= 0) {
            lost = true;
        }
        if (money >= 1000 && level == 1) {
            won = true;
        }
        else if (money >= 10000 && level == 2) {
            won = true;
        }
        position.x += move ? velocity.x * delta_time : 0;
        if (!move) {
            position.y += move ? 0 : velocity.y * delta_time;
            if (frame_counter == moment_to_go) {
                s_c++;
                movement.y = -0.025f;
                model_matrix = glm::translate(model_matrix, glm::vec3(0.0f, -0.025f, 0.0f));
                scale_counter++;
                frame_counter--;

                //grab
                check_collision_y(objects, object_count);

                if (scale_counter >= 240 || collided_top) {
                    lives++;
                    frame_counter++;
                    scale_counter = 0;
                    time_to_shrink = true;
                }
            }
            else if (time_to_shrink) {
                movement.y = 0.025f;
                model_matrix = glm::translate(model_matrix, glm::vec3(0.0f, 0.025f, 0.0f));
                shrink_counter++;
                frame_counter--;
                if (collided_top == false && shrink_counter >= 240) {

                    frame_counter++;
                    shrink_counter = 0;
                    time_to_shrink = false;
                    move = true;
                    s_c = 0;
                }
                else if (collided_top && shrink_counter >= s_c) {

                    frame_counter++;
                    shrink_counter = 0;
                    time_to_shrink = false;
                    move = true;
                    s_c = 0;
                    collided_top = false;
                }
            }
        }

        frame_counter++;
        if (frame_counter >= 370) {
            go_left = !go_left;
            frame_counter = 0;
        }
        if (go_left && move) {
            movement.x = -0.02f;
            model_matrix = glm::translate(model_matrix, glm::vec3(-0.02f, 0.0f, 0.0f));
        }
        else if (!go_left && move) {
            movement.x = 0.02f;
            model_matrix = glm::translate(model_matrix, glm::vec3(0.02f, 0.0f, 0.0f));
        }
    }

    else if (entity_type == SHIP) {
        if (!player->move) {
            move = false;
            frame_counter--;
        }
        else if (player->move) {
            move = true;
        }

        frame_counter++;
        if (frame_counter >= 370) {
            go_left = !go_left;
            frame_counter = 0;
        }
        if (go_left && move) {
            model_matrix = glm::translate(model_matrix, glm::vec3(-0.02f, 0.0f, 0.0f));
        }
        else if (!go_left && move) {
            model_matrix = glm::translate(model_matrix, glm::vec3(0.02f, 0.0f, 0.0f));
        }
    }

    else if (entity_type == ROCK) {
        if (player->collided_top && move == false && abs(player->get_position().x - get_position().x) < 0.1f && abs(player->get_position().y - get_position().y) < 0.5f) {
            move = true;
        }
        if (move) {
            ai_type = DEAD;
            animation_frames = 0;
            model_matrix = glm::translate(model_matrix, glm::vec3(0.0f, 0.025f, 0.0f));
            frame_counter++;
            if (frame_counter >= s_c && is_active) {
                player->add_money(value);
                deactivate();
                model_matrix = glm::translate(model_matrix, glm::vec3(100.0f, 100.0f, 0.0f));
            }
        }
        if (ai_type == WALKER || ai_type == WALKER2 || ai_type == WALKER3) {
            position.x += move ? 0 : velocity.x * delta_time;
            activate_ai(player);
        }
    }
}

void Entity::update2(float delta_time, Entity* player, Entity* objects, int object_count, Map* map, int level, Entity* platform) {
    if (entity_type == PLAYER) {
        collided_top = false;
        collided_bottom = false;
        collided_left = false;
        collided_right = false;

        if (animation_indices != NULL) {
            if (glm::length(movement) != 0)
            {
                animation_time += delta_time;
                float frames_per_second = (float)1 / SECONDS_PER_FRAME;

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
        if (is_jumping) {
            is_jumping = false;

            velocity.y += jumping_power;
        }

        if (fabs(position.x - 1.65) < 0.1f && fabs(position.y + 0.1) < 0.1f) {
            won = true;
        }

        float ticks = (float)SDL_GetTicks() / 1000;
        delta_time = ticks - previous_ticks;
        previous_ticks = ticks;

        velocity.x = movement.x * speed;
        velocity += acceleration * delta_time;

        position.y += collided_bottom ? 0 : velocity.y * delta_time;
        check_collision_y(map);

        position.x += velocity.x * delta_time;
        check_collision_x(map);

        model_matrix = glm::mat4(1.0f);
        model_matrix = glm::translate(model_matrix, position);
    }
}

void const Entity::check_collision_y(Entity* collidable_entities, int collidable_entity_count) {
    for (int i = 0; i < collidable_entity_count; i++) {
        Entity* collidable_entity = &collidable_entities[i];
        if (check_collision(collidable_entity)) {
            float y_distance = fabs(position.y - collidable_entity->position.y);
            float y_overlap = fabs(y_distance - (height / 2.0f) - (collidable_entity->height / 2.0f));
            collided_top = true;
            collidable_entity->collided_bottom = true;
        }
    }
}

void const Entity::check_collision_x(Entity* collidable_entities, int collidable_entity_count) {
    for (int i = 0; i < collidable_entity_count; i++) {
        Entity* collidable_entity = &collidable_entities[i];

        if (check_collision(collidable_entity)) {
            float x_distance = fabs(position.x - collidable_entity->position.x);
            float x_overlap = fabs(x_distance - (width / 2.0f) - (collidable_entity->width / 2.0f));
            if (collidable_entity->position.x > position.x && abs(position.y - collidable_entity->position.y) < 0.2f) {
                lives == 1 ? position.x += 0.0f : position.x -= 3.2f;
                collided_right = true;
                collidable_entity->collided_left = true;
                bounce = true;
                lives -= 1;
            }
            else if (collidable_entity->position.x < position.x && abs(position.y - collidable_entity->position.y) < 0.2f) {
                lives == 1 ? position.x += 0.0f : position.x -= 3.2f;
                collided_left = true;
                collidable_entity->collided_right = true;
                lives -= 1;
                bounce = true;
            }
        }
    }
}

void const Entity::check_collision_y(Map* map) {
    glm::vec3 top = glm::vec3(position.x, position.y + (height / 2), position.z);
    glm::vec3 top_left = glm::vec3(position.x - (width / 2), position.y + (height / 2), position.z);
    glm::vec3 top_right = glm::vec3(position.x + (width / 2), position.y + (height / 2), position.z);

    glm::vec3 bottom = glm::vec3(position.x, position.y - (height / 2), position.z);
    glm::vec3 bottom_left = glm::vec3(position.x - (width / 2), position.y - (height / 2), position.z);
    glm::vec3 bottom_right = glm::vec3(position.x + (width / 2), position.y - (height / 2), position.z);

    float penetration_x = 0;
    float penetration_y = 0;

    if (map->is_solid(top, &penetration_x, &penetration_y) && velocity.y > 0) {
        position.y -= penetration_y;
        velocity.y = 0;
        collided_top = true;
    }
    else if (map->is_solid(top_left, &penetration_x, &penetration_y) && velocity.y > 0) {
        position.y -= penetration_y;
        velocity.y = 0;
        collided_top = true;
    }
    else if (map->is_solid(top_right, &penetration_x, &penetration_y) && velocity.y > 0) {
        position.y -= penetration_y;
        velocity.y = 0;
        collided_top = true;
    }

    if (map->is_solid(bottom, &penetration_x, &penetration_y) && velocity.y < 0) {
        position.y += penetration_y;
        velocity.y = 0;
        collided_bottom = true;
    }
    else if (map->is_solid(bottom_left, &penetration_x, &penetration_y) && velocity.y < 0) {
        position.y += penetration_y;
        velocity.y = 0;
        collided_bottom = true;
    }
    else if (map->is_solid(bottom_right, &penetration_x, &penetration_y) && velocity.y < 0) {
        position.y += penetration_y;
        velocity.y = 0;
        collided_bottom = true;

    }
}

void const Entity::check_collision_y2(Entity* collidable_entities, int collidable_entity_count) {
    for (int i = 0; i < collidable_entity_count; i++) {
        Entity* collidable_entity = &collidable_entities[i];
        if (check_collision2(collidable_entity)) {
            lives++;
            velocity.y = 0;
            collided_bottom = true;
            going_up = true;
            collidable_entity->down = false;
        }
    }
}

void const Entity::check_collision_x(Map* map) {
    glm::vec3 left = glm::vec3(position.x - (width / 2), position.y, position.z);
    glm::vec3 right = glm::vec3(position.x + (width / 2), position.y, position.z);

    float penetration_x = 0;
    float penetration_y = 0;

    if (map->is_solid(left, &penetration_x, &penetration_y) && velocity.x < 0) {
        position.x += penetration_x;
        velocity.x = 0;
        collided_left = true;
    }
    if (map->is_solid(right, &penetration_x, &penetration_y) && velocity.x > 0) {
        position.x -= penetration_x;
        velocity.x = 0;
        collided_right = true;
    }
}

void Entity::render(ShaderProgram* program) {
    if (!is_active) return;

    program->SetModelMatrix(model_matrix);

    if (animation_indices != NULL) {
        draw_sprite_from_texture_atlas(program, texture_id, animation_indices[animation_index]);
        return;
    }

    float vertices[] = { -4.0f, 1.5f, -3.0f, 1.5f, -3.0f, 12.3f, -4.0f, 1.5f, -3.0f, 12.3f, -4.0f, 12.3f };
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
    if (!is_active) return;

    program->SetModelMatrix(model_matrix);

    if (animation_indices != NULL) {
        draw_sprite_from_texture_atlas(program, texture_id, animation_indices[animation_index]);
        return;
    }

    float vertices[] = { -4.9f, 2.14f, -2.1f, 2.14f, -2.1f, 3.91f, -4.9f, 2.14f, -2.1f, 3.91f, -4.9f, 3.91f };
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

void Entity::render3(ShaderProgram* program, float vertices[]) {
    if (!is_active) return;

    program->SetModelMatrix(model_matrix);

    if (animation_indices != NULL) {
        draw_sprite_from_texture_atlas2(program, texture_id, animation_indices[animation_index], vertices);
        return;
    }

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

bool const Entity::check_collision(Entity* other)  const{

    if (other == this) return false;

    if (!is_active || !other->is_active) return false;

    float x_distance = fabs(position.x - other->position.x);
    float y_distance = fabs(position.y - other->position.y);

    return x_distance < 0.12f && y_distance < 0.45f;
}

bool const Entity::check_collision2(Entity* other) const {

    if (other == this) return false;

    if (!is_active || !other->is_active) return false;

    float x_distance = fabs(position.x - other->position.x);
    float y_distance = fabs(position.y - other->position.y);

    return x_distance < 0.5f && y_distance < 0.5f;
}