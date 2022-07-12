#define GL_SILENCE_DEPRECATION
#define STB_IMAGE_IMPLEMENTATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#define FIXED_TIMESTEP 0.0166666f
#define ROCK_COUNT 18

#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "stb_image.h"
#include "cmath"
#include <ctime>
#include <vector>
#include "Entity.h"
#include <SDL_mixer.h>


struct GameState{
    Entity* player;
    Entity* rocks;
};

const int WINDOW_WIDTH = 1000,
          WINDOW_HEIGHT = 760;

const float BG_RED = 0.68f,
            BG_BLUE = 0.85f,
            BG_GREEN = 0.9f,
            BG_OPACITY = 1.0f;

const int VIEWPORT_X = 0,
          VIEWPORT_Y = 0,
          VIEWPORT_WIDTH = WINDOW_WIDTH,
          VIEWPORT_HEIGHT = WINDOW_HEIGHT;

const int FONTBANK_SIZE = 16;

const char V_SHADER_PATH[] = "shaders/vertex_textured.glsl",
F_SHADER_PATH[] = "shaders/fragment_textured.glsl";

SDL_Window* display_window;
bool GameIsRunning = true;
bool moving = false;
bool touches_rock = false;
bool touches_platform = false;

Mix_Music* music;
Mix_Chunk* explosion;
Mix_Chunk* victory;

ShaderProgram program;
glm::mat4 view_matrix, projection_matrix;

float previous_ticks = 0.0f;
float accumulator = 0.0f;

const char PLAYER_SPRITE[] = "assets/spaceship.png";
const char PLAYER_SPRITE2[] = "assets/rock.png";
const char PLAYER_SPRITE3[] = "assets/platform.png";
const char FONT[] = "assets/font.png";
GLuint font_texture_id;

GameState state;

GLuint load_texture(const char* filepath) {
    int image_width, image_height, number_of_components;
    unsigned char* image = stbi_load(filepath, &image_width, &image_height, &number_of_components, STBI_rgb_alpha);
    if (image == NULL) {
        std::cout << "can't load the pic";
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    stbi_image_free(image);

    return textureID;
}

void DrawText(ShaderProgram* program, GLuint font_texture_id, std::string text, float screen_size, float spacing, glm::vec3 position, bool touches) {
    if (!touches) return;
    float width = 1.0f / FONTBANK_SIZE;
    float height = 1.0f / FONTBANK_SIZE;

    std::vector<float> vertices;
    std::vector<float> texture_coordinates;

    for (int i = 0; i < text.size(); i++) {

        int spritesheet_index = (int)text[i];
        float offset = (screen_size + spacing) * i;

        float u_coordinate = (float)(spritesheet_index % FONTBANK_SIZE) / FONTBANK_SIZE;
        float v_coordinate = (float)(spritesheet_index / FONTBANK_SIZE) / FONTBANK_SIZE;

        vertices.insert(vertices.end(), {
            offset + (-0.5f * screen_size), 0.5f * screen_size,
            offset + (-0.5f * screen_size), -0.5f * screen_size,
            offset + (0.5f * screen_size), 0.5f * screen_size,
            offset + (0.5f * screen_size), -0.5f * screen_size,
            offset + (0.5f * screen_size), 0.5f * screen_size,
            offset + (-0.5f * screen_size), -0.5f * screen_size,
            });

        texture_coordinates.insert(texture_coordinates.end(), {
            u_coordinate, v_coordinate,
            u_coordinate, v_coordinate + height,
            u_coordinate + width, v_coordinate,
            u_coordinate + width, v_coordinate + height,
            u_coordinate + width, v_coordinate,
            u_coordinate, v_coordinate + height,
            });
    }

    glm::mat4 model_matrix = glm::mat4(1.0f);
    model_matrix = glm::translate(model_matrix, position);

    program->SetModelMatrix(model_matrix);
    glUseProgram(program->programID);

    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices.data());
    glEnableVertexAttribArray(program->positionAttribute);
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texture_coordinates.data());
    glEnableVertexAttribArray(program->texCoordAttribute);

    glBindTexture(GL_TEXTURE_2D, font_texture_id);
    glDrawArrays(GL_TRIANGLES, 0, (int)(text.size() * 6));

    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}

void initialize(){
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    display_window = SDL_CreateWindow("Project 3: The Spaceship",SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT,SDL_WINDOW_OPENGL);

    SDL_GLContext context = SDL_GL_CreateContext(display_window);
    SDL_GL_MakeCurrent(display_window, context);

#ifdef _WINDOWS
    glewInit();
#endif

    glViewport(VIEWPORT_X, VIEWPORT_Y, VIEWPORT_WIDTH, VIEWPORT_HEIGHT);

    program.Load(V_SHADER_PATH, F_SHADER_PATH);

    //music stuff
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    music = Mix_LoadMUS("assets/music.mp3");
    explosion = Mix_LoadWAV("assets/explosion.wav");
    victory = Mix_LoadWAV("assets/victory.wav");

    Mix_PlayMusic(music, -1);

    Mix_VolumeMusic(MIX_MAX_VOLUME / 2.0f);

    view_matrix = glm::mat4(1.0f);  
    projection_matrix = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, -5.0f, 5.0f);

    program.SetProjectionMatrix(projection_matrix);
    program.SetViewMatrix(view_matrix);

    glUseProgram(program.programID);
    glClearColor(BG_RED, BG_BLUE, BG_GREEN, BG_OPACITY);

    state.player = new Entity();
    state.player->set_position(glm::vec3(0.0f, 4.5f,0.0f));
    state.player->set_movement(glm::vec3(0.0f));
    state.player->speed = 1.0f;
    state.player->set_acceleration(glm::vec3(0.0f, -0.121f, 0.0f));
    state.player->texture_id = load_texture(PLAYER_SPRITE);

    state.player->set_height(1.0f);
    state.player->set_width(1.0f);
    
    //initialize platforms
    state.rocks = new Entity[ROCK_COUNT];
    for (int i = 0; i < 6; i++){
        state.rocks[i].texture_id = load_texture(PLAYER_SPRITE2);
        state.rocks[i].set_position(glm::vec3(-4.8f + i * 0.8, -4.85f, 0.0f));
        state.rocks[i].update(0.0f, NULL, 0, moving,touches_rock, touches_platform);
    }
    for (int i = 6; i < 12; i++) {
        state.rocks[i].texture_id = load_texture(PLAYER_SPRITE2);
        state.rocks[i].set_position(glm::vec3(1.15f + (i-6) * 0.8, -4.85f, 0.0f));
        state.rocks[i].update(0.0f, NULL, 0, moving, touches_rock, touches_platform);
    }

    state.rocks[12].texture_id = load_texture(PLAYER_SPRITE2);
    state.rocks[12].set_position(glm::vec3(-1.8f, 3.0f, 0.0f));
    state.rocks[12].update(0.0f, NULL, 0, moving, touches_rock, touches_platform);

    state.rocks[13].texture_id = load_texture(PLAYER_SPRITE2);
    state.rocks[13].set_position(glm::vec3(-0.2f, 1.1f, 0.0f));
    state.rocks[13].update(0.0f, NULL, 0, moving, touches_rock, touches_platform);

    state.rocks[14].texture_id = load_texture(PLAYER_SPRITE2);
    state.rocks[14].set_position(glm::vec3(1.4f, 2.3f, 0.0f));
    state.rocks[14].update(0.0f, NULL, 0, moving, touches_rock, touches_platform);

    state.rocks[15].texture_id = load_texture(PLAYER_SPRITE2);
    state.rocks[15].set_position(glm::vec3(1.1f, -2.8f, 0.0f));
    state.rocks[15].update(0.0f, NULL, 0, moving, touches_rock, touches_platform);

    state.rocks[16].texture_id = load_texture(PLAYER_SPRITE2);
    state.rocks[16].set_position(glm::vec3(-1.3f, -1.5f, 0.0f));
    state.rocks[16].update(0.0f, NULL, 0, moving,touches_rock, touches_platform);

    for (int i = 0; i < 17; i++) {
        state.rocks[i].set_height(0.3f);
        state.rocks[i].set_width(1.5f);
    }

    state.rocks[17].texture_id = load_texture(PLAYER_SPRITE3);
    state.rocks[17].set_position(glm::vec3(0.2f, -4.85f, 0.0f));
    state.rocks[17].update(0.0f, NULL, 0, moving, touches_rock, touches_platform);
    state.rocks[17].set_height(0.3f);
    state.rocks[17].set_width(1.5f);

    font_texture_id = load_texture(FONT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void process_input(){
    state.player->set_movement(glm::vec3(0.0f));

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type) {
        // End game
        case SDL_QUIT:
        case SDL_WINDOWEVENT_CLOSE:
            GameIsRunning = false;
            break;

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case SDLK_q:
                // Quit the game with a keystroke
                GameIsRunning = false;
                break;

            default:
                break;
            }

        default:
            break;
        }
    }

    const Uint8* key_state = SDL_GetKeyboardState(NULL);

    if (touches_platform || touches_rock) {
        return;
    }

    if (key_state[SDL_SCANCODE_D]){
        state.player->movement.x = 3.0f;
        moving = true;
    }
    if (key_state[SDL_SCANCODE_A]){
        state.player->movement.x = -3.0f;
        moving = true;
    }
    
    
    if (glm::length(state.player->movement) > 1.0f){
        state.player->movement = glm::normalize(state.player->movement);
    }
}


void update() { 
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float delta_time = ticks - previous_ticks;
    previous_ticks = ticks;

    delta_time += accumulator;

    if (delta_time < FIXED_TIMESTEP)
    {
        accumulator = delta_time;
        return;
    }

    while (delta_time >= FIXED_TIMESTEP) {
        state.player->update(FIXED_TIMESTEP, state.rocks, ROCK_COUNT, moving, touches_rock, touches_platform);
        delta_time -= FIXED_TIMESTEP;
    }

    accumulator = delta_time;    
}

void render() {
    
    glClear(GL_COLOR_BUFFER_BIT);

    state.player->render(&program);
    for (int i = 0; i < ROCK_COUNT-1; i++) state.rocks[i].render(&program);
    state.rocks[17].render2(&program);

    if (touches_rock) {
        Mix_PlayChannel(-1, explosion, 0);
    }
    else if (touches_platform) {
        Mix_PlayChannel(-1, victory, 0);
    }

    DrawText(&program, font_texture_id, "YOU LOSE!", 0.5f, 0.01f, glm::vec3(-1.8f, 0.0f, 0.0f), touches_rock);
    DrawText(&program, font_texture_id, "YOU WIN!", 0.5f, 0.01f, glm::vec3(-1.8f, 0.0f, 0.0f),touches_platform);

    SDL_GL_SwapWindow(display_window);

}

void shutdown() { 
    Mix_FreeMusic(music);
    Mix_FreeChunk(explosion);
    Mix_FreeChunk(victory);
    SDL_Quit(); 
}

int main(int argc, char* argv[]){
    initialize();

    while (GameIsRunning){
        process_input();

        update();

        render();
    }

    shutdown();
    return 0;
}
