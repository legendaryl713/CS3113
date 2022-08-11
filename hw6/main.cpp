#define GL_SILENCE_DEPRECATION
#define GL_GLEXT_PROTOTYPES 1
#define FIXED_TIMESTEP 0.0166666f
#define LEVEL1_WIDTH 14
#define LEVEL1_HEIGHT 8
#define LEVEL1_LEFT_EDGE 5.0f

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#include <SDL_mixer.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "cmath"
#include <ctime>
#include <vector>
#include "Entity.h"
#include "Map.h"
#include "Utility.h"
#include "Scene.h"
#include "LevelA.h"
//#include "LevelB.h"
//#include "LevelC.h"
//#include "MainMenu.h"

/**
 CONSTANTS
 */

Mix_Music* music;

bool not_menu = false;

const int WINDOW_WIDTH = 1200,
WINDOW_HEIGHT = 800;

const float BG_RED = 0.1922f,
BG_BLUE = 0.549f,
BG_GREEN = 0.9059f,
BG_OPACITY = 1.0f;

const int VIEWPORT_X = 0,
VIEWPORT_Y = 0,
VIEWPORT_WIDTH = WINDOW_WIDTH,
VIEWPORT_HEIGHT = WINDOW_HEIGHT;

const char V_SHADER_PATH[] = "shaders/vertex_textured.glsl",
F_SHADER_PATH[] = "shaders/fragment_textured.glsl";

const float MILLISECONDS_IN_SECOND = 1000.0;

/**
 VARIABLES
 */
Scene* current_scene;
//MainMenu* menu;
LevelA* level_a;
//LevelB* level_b;
//LevelC* level_c;

SDL_Window* display_window;
bool game_is_running = true;

ShaderProgram program;
glm::mat4 view_matrix, projection_matrix;

float previous_ticks = 0.0f;
float accumulator = 0.0f;

void switch_to_scene(Scene* scene)
{
    current_scene = scene;
    current_scene->initialise();
}

void initialise()
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    display_window = SDL_CreateWindow("Hello, Scenes!",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        SDL_WINDOW_OPENGL);

    SDL_GLContext context = SDL_GL_CreateContext(display_window);
    SDL_GL_MakeCurrent(display_window, context);

#ifdef _WINDOWS
    glewInit();
#endif

    glViewport(VIEWPORT_X, VIEWPORT_Y, VIEWPORT_WIDTH, VIEWPORT_HEIGHT);

    program.Load(V_SHADER_PATH, F_SHADER_PATH);

    view_matrix = glm::mat4(1.0f);
    projection_matrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

    program.SetProjectionMatrix(projection_matrix);
    program.SetViewMatrix(view_matrix);

    /*Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    music = Mix_LoadMUS("assets/music.mp3");

    Mix_PlayMusic(music, -1);

    Mix_VolumeMusic(MIX_MAX_VOLUME / 4.0f);*/

    glUseProgram(program.programID);

    glClearColor(BG_RED, BG_BLUE, BG_GREEN, BG_OPACITY);

    //menu = new MainMenu();
    level_a = new LevelA();
    //level_b = new LevelB();
    //level_c = new LevelC();
    switch_to_scene(level_a);

    // enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void process_input() {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {

        case SDL_QUIT:

        case SDL_WINDOWEVENT_CLOSE:
            game_is_running = false;
            break;

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case SDLK_q:
                // Quit the game with a keystroke
                game_is_running = false;
                break;
            case SDLK_SPACE:
                if (current_scene->state.player->move) {
                    current_scene->state.player->moment_to_go = current_scene->state.player->frame_counter;
                    current_scene->state.player->move = false;
                }
                break;
            default:
                break;
            }

        default:
            break;
        }
    }
    // VERY IMPORTANT: If nothing is pressed, we don't want to go anywhere
    //if (current_scene == menu) {
    //    SDL_Event event;
    //    while (SDL_PollEvent(&event))
    //    {
    //        switch (event.type) {
    //            // End game
    //        case SDL_QUIT:
    //        case SDL_WINDOWEVENT_CLOSE:
    //            game_is_running = false;
    //            break;

    //        case SDL_KEYDOWN:
    //            switch (event.key.keysym.sym) {
    //            case SDLK_q:
    //                // Quit the game with a keystroke
    //                game_is_running = false;
    //                break;

    //            case SDLK_RETURN:
    //                // start game
    //                not_menu = true;
    //                break;

    //            default:
    //                break;
    //            }

    //        default:
    //            break;
    //        }
    //    }
    //}
    //else {
    //    current_scene->state.player->set_movement(glm::vec3(0.0f));

    //    SDL_Event event;
    //    while (SDL_PollEvent(&event))
    //    {
    //        switch (event.type) {
    //            // End game
    //        case SDL_QUIT:
    //        case SDL_WINDOWEVENT_CLOSE:
    //            game_is_running = false;
    //            break;

    //        case SDL_KEYDOWN:
    //            switch (event.key.keysym.sym) {
    //            case SDLK_q:
    //                // Quit the game with a keystroke
    //                game_is_running = false;
    //                break;

    //            case SDLK_SPACE:
    //                // Jump
    //                if (current_scene->state.player->collided_bottom)
    //                {
    //                    current_scene->state.player->is_jumping = true;
    //                    Mix_PlayChannel(-1, current_scene->state.jump_sfx, 0);
    //                }
    //                break;

    //            default:
    //                break;
    //            }

    //        default:
    //            break;
    //        }
    //    }

    //    const Uint8* key_state = SDL_GetKeyboardState(NULL);

    //    if (key_state[SDL_SCANCODE_A]) //&& !current_scene->state.player->lost && !current_scene->state.player->won
    //    {
    //        current_scene->state.player->movement.x = -1.0f;
    //        current_scene->state.player->animation_indices = current_scene->state.player->walking[current_scene->state.player->LEFT];
    //    }
    //    else if (key_state[SDL_SCANCODE_D] ) //&& !current_scene->state.player->lost && !current_scene->state.player->won
    //    {
    //        current_scene->state.player->movement.x = 1.0f;
    //        current_scene->state.player->animation_indices = current_scene->state.player->walking[current_scene->state.player->RIGHT];
    //    }

    //    if (glm::length(current_scene->state.player->movement) > 1.0f)
    //    {
    //        current_scene->state.player->movement = glm::normalize(current_scene->state.player->movement);
    //    }
    //    if (glm::length(current_scene->state.enemies[0].movement) > 1.0f) {
    //        current_scene->state.enemies[0].movement = glm::normalize(current_scene->state.enemies[0].movement);
    //    }
    //}
}

void update() {

    float ticks = (float)SDL_GetTicks() / MILLISECONDS_IN_SECOND;
    float delta_time = ticks - previous_ticks;
    previous_ticks = ticks;

    delta_time += accumulator;

    if (delta_time < FIXED_TIMESTEP)
    {
        accumulator = delta_time;
        return;
    }

    while (delta_time >= FIXED_TIMESTEP) {
        current_scene->update(FIXED_TIMESTEP);

        delta_time -= FIXED_TIMESTEP;
    }

    accumulator = delta_time;


    // Prevent the camera from showing anything outside of the "edge" of the level
    view_matrix = glm::mat4(1.0f);

    /*if (current_scene == menu) {
        if (not_menu)switch_to_scene(level_a);
        return;
    }*/

    /*if (current_scene->state.player->get_position().x > LEVEL1_LEFT_EDGE) {
        view_matrix = glm::translate(view_matrix, glm::vec3(-current_scene->state.player->get_position().x, 3.75, 0));
    }
    else {
        view_matrix = glm::translate(view_matrix, glm::vec3(-5, 3.75, 0));
    }*/

    //if (current_scene == level_a && current_scene->state.player->get_position().x > current_scene->state.enemies[2].get_position().x + 1.0f && current_scene->state.player->get_position().y < current_scene->state.enemies[2].get_position().y - 1.0f && current_scene->state.player->lvl1_won) {
    //    switch_to_scene(level_b);
    //    level_b->state.player->change_life(level_a->state.player->lives);
    //}
    //if (current_scene == level_b && current_scene->state.player->get_position().x > current_scene->state.enemies[2].get_position().x + 6.0f && current_scene->state.player->lvl2_won) {
    //    switch_to_scene(level_c);
    //    level_c->state.player->change_life(level_b->state.player->lives);
    //}
}

void render() {
    program.SetViewMatrix(view_matrix);

    glClear(GL_COLOR_BUFFER_BIT);

    current_scene->render(&program);

    SDL_GL_SwapWindow(display_window);
}

void shutdown() {
    Mix_FreeMusic(music);

    SDL_Quit();

    //delete menu;

    delete level_a;

    //delete level_b;
    //delete level_c;
}

int main(int argc, char* argv[])
{
    initialise();

    while (game_is_running)
    {
        process_input();
        update();
        render();
    }

    shutdown();
    return 0;
}
