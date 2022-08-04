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
#include "stb_image.h"

const int WINDOW_WIDTH = 1000, WINDOW_HEIGHT = 760;
const float BG_RED = 1.0f, BG_BLUE = 0.713f, BG_GREEN = 0.9f, BG_OPACITY = 1.0f;
const int TRIANGLE_RED = 0.6, TRIANGLE_BLUE = 0.8, TRIANGLE_GREEN = 1.0, TRIANGLE_OPACITY = 1.0;
const int VIEWPORT_X = 0,

VIEWPORT_Y = 0,
VIEWPORT_WIDTH = WINDOW_WIDTH,
VIEWPORT_HEIGHT = WINDOW_HEIGHT;

int chamber_frame_counter = 0;
float chamber_move = 0.0f;
bool chamber_rotate_left = true;
float chamber_angle = 0.0f;

const char V_SHADER_PATH[] = "shaders/vertex_textured.glsl",
F_SHADER_PATH[] = "shaders/fragment_textured.glsl";

SDL_Window* display_window;
bool GameIsRunning = true;

ShaderProgram program;
glm::mat4 view_matrix, model_matrix, model_matrix2, projection_matrix;

float previous_ticks = 0.0f;
float previous_ticks2 = 0.0f;

GLuint player_texture_id, player_texture_id2;
const char PLAYER_SPRITE[] = "chamber2.png";

const char PLAYER_SPRITE2[] = "bullet.png";

GLuint load_texture(const char* filepath) {

    //step1: load image file, quit if image didn't load
    int image_width, image_height, number_of_components;
    unsigned char* image = stbi_load(filepath, &image_width, &image_height, &number_of_components, STBI_rgb_alpha);
    if (image == NULL) {
        std::cout << "can't load the pic";
        //assert(false);
    }

    //step 2: generate and bind the image to a texture ID
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

    //step3: setting our texture filter parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    //step4: release our image file from memory and return texture id
    stbi_image_free(image);

    return textureID;
}

void initialize()
{
    SDL_Init(SDL_INIT_VIDEO);
    display_window = SDL_CreateWindow("Project 1: 3-body problem", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL);

    SDL_GLContext context = SDL_GL_CreateContext(display_window);
    SDL_GL_MakeCurrent(display_window, context);

#ifdef _WINDOWS
    glewInit();
#endif

    glViewport(VIEWPORT_X, VIEWPORT_Y, VIEWPORT_WIDTH, VIEWPORT_HEIGHT);

    program.Load(V_SHADER_PATH, F_SHADER_PATH);

    view_matrix = glm::mat4(1.0f);
    model_matrix = glm::mat4(1.0f);
    model_matrix2 = glm::mat4(1.0f);
    projection_matrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

    program.SetProjectionMatrix(projection_matrix);
    program.SetViewMatrix(view_matrix);

    player_texture_id = load_texture(PLAYER_SPRITE);

    player_texture_id2 = load_texture(PLAYER_SPRITE2);

    glUseProgram(program.programID);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(BG_RED, BG_BLUE, BG_GREEN, BG_OPACITY);
}

void process_input() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            GameIsRunning = false;
        }
    }
}

void update() {
    chamber_frame_counter++;
    if (chamber_frame_counter >= 2000) {
        chamber_frame_counter = 0;
        chamber_rotate_left = !chamber_rotate_left;
    }
    if (chamber_rotate_left == true) {
        model_matrix = glm::mat4(1.0f);
        float ticks = (float)SDL_GetTicks() / 1000.0f;
        float delta_time = ticks - previous_ticks;
        previous_ticks = ticks;
        chamber_move += 2.5f * delta_time;
        model_matrix = glm::translate(model_matrix, glm::vec3(chamber_move, 0.0f, 0.0f));
    }
    else {
        model_matrix = glm::mat4(1.0f);
        float ticks = (float)SDL_GetTicks() / 1000.0f;
        float delta_time = ticks - previous_ticks;
        previous_ticks = ticks;
        chamber_move -= 2.5f * delta_time;
        model_matrix = glm::translate(model_matrix, glm::vec3(chamber_move, 0.0f, 0.0f));
    }
    
    model_matrix2 = glm::mat4(1.0f);
    float ticks2 = (float)SDL_GetTicks() / 1000.0f;
    float delta_time2 = ticks2 - previous_ticks2;
    previous_ticks2 = ticks2;
    chamber_angle += 60.0f * delta_time2;
    model_matrix2 = glm::rotate(model_matrix2, glm::radians(chamber_angle), glm::vec3(0.0f, 0.0f, 1.0f));
}

void render() {
    glClear(GL_COLOR_BUFFER_BIT);

    program.SetModelMatrix(model_matrix);
    
    //the man
    float vertices1[] = {
        -5.0f, -1.7f, -3.0f, -1.7f, -3.0f, 2.1f,
        -5.0f, -1.7f, -3.0f, 2.1f, -5.0f, 2.1f
    };

    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices1);
    glEnableVertexAttribArray(program.positionAttribute);

    float texture_coordinates[] = {
        0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
    };
    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texture_coordinates);
    glEnableVertexAttribArray(program.texCoordAttribute);
    glBindTexture(GL_TEXTURE_2D, player_texture_id);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(program.positionAttribute);
    glDisableVertexAttribArray(program.texCoordAttribute);

    //the bullet
    program.SetModelMatrix(model_matrix2);

    float vertices2[] = {
        -3.0f, -2.5f, -2.5f, -2.5f, -2.5f, -1.5f,
        -3.0f, -2.5f, -2.5f, -1.5f, -3.0f, -1.5f
    };
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices2);
    glEnableVertexAttribArray(program.positionAttribute);

    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texture_coordinates);
    glEnableVertexAttribArray(program.texCoordAttribute);

    glBindTexture(GL_TEXTURE_2D, player_texture_id2);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(program.positionAttribute);
    glDisableVertexAttribArray(program.texCoordAttribute);

    SDL_GL_SwapWindow(display_window);
}

void shutdown() { SDL_Quit(); }

int main(int argc, char* argv[]) {
    initialize();

    while (GameIsRunning) {
        process_input();
        update();
        render();
    }

    shutdown();
    return 0;
}