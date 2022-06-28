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
#include "cmath"

const int WINDOW_WIDTH = 1000,WINDOW_HEIGHT = 760;
const float BG_RED = 0.68f,BG_BLUE = 0.85f,BG_GREEN = 0.9f,BG_OPACITY = 1.0f;
const int VIEWPORT_X = 0,

VIEWPORT_Y = 0,
VIEWPORT_WIDTH = WINDOW_WIDTH,
VIEWPORT_HEIGHT = WINDOW_HEIGHT;

const char V_SHADER_PATH[] = "shaders/vertex_textured.glsl",
F_SHADER_PATH[] = "shaders/fragment_textured.glsl";

SDL_Window* display_window;
bool GameIsRunning = true;

ShaderProgram program;
glm::mat4 view_matrix, model_matrix_left_paddle, projection_matrix, model_matrix_right_paddle, model_matrix_ball;

float previous_ticks = 0.0f;

GLuint player_texture_id, player_texture_id2, player_texture_id3;
const char PLAYER_SPRITE[] = "goku.png";
const char PLAYER_SPRITE2[] = "vegeta.png";
const char PLAYER_SPRITE3[] = "ball.png";

glm::vec3 player_position = glm::vec3(-3.8f, 0.0f, 0.0f);
glm::vec3 player_movement = glm::vec3(0.0f, 0.0f, 0.0f);

glm::vec3 player_position2 = glm::vec3(3.8f, 0.0f, 0.0f);
glm::vec3 player_movement2 = glm::vec3(0.0f, 0.0f, 0.0f);

glm::vec3 ball_position = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 ball_movement = glm::vec3(0.0f, 0.0f, 0.0f);

float paddle_height = 2.0f;
float paddle_width = 0.2f;
float ball_height = 0.6f;
float ball_width = 0.4f;


glm::vec3 player_orientation = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 player_rotation = glm::vec3(0.0f, 0.0f, 0.0f);

GLuint load_texture(const char* filepath) {

    //step1: load image file, quit if image didn't load
    int image_width, image_height, number_of_components;
    unsigned char* image = stbi_load(filepath, &image_width, &image_height, &number_of_components, STBI_rgb_alpha);
    if (image == NULL) {
        std::cout << "can't load the pic";
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
    display_window = SDL_CreateWindow("Project 2: Pong problem",SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT,SDL_WINDOW_OPENGL);

    SDL_GLContext context = SDL_GL_CreateContext(display_window);
    SDL_GL_MakeCurrent(display_window, context);

#ifdef _WINDOWS
    glewInit();
#endif

    glViewport(VIEWPORT_X, VIEWPORT_Y, VIEWPORT_WIDTH, VIEWPORT_HEIGHT);

    program.Load(V_SHADER_PATH, F_SHADER_PATH);

    view_matrix = glm::mat4(1.0f);  
    model_matrix_left_paddle = glm::mat4(1.0f);
    model_matrix_right_paddle = glm::mat4(1.0f);
    model_matrix_ball = glm::mat4(1.0f);
    projection_matrix = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, -5.0f, 5.0f);

    program.SetProjectionMatrix(projection_matrix);
    program.SetViewMatrix(view_matrix);

    player_texture_id = load_texture(PLAYER_SPRITE);

    player_texture_id2 = load_texture(PLAYER_SPRITE2);

    player_texture_id3 = load_texture(PLAYER_SPRITE3);

    glUseProgram(program.programID);

    glClearColor(BG_RED, BG_BLUE, BG_GREEN, BG_OPACITY);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void process_input(){
    
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
                //start the game
            case SDLK_f:
                ball_movement.x = 1.0f;
                ball_movement.y = 1.0f;
                break;

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

    if (key_state[SDL_SCANCODE_UP])
    {
        player_movement2.y = 1.0f;
    }
    else if (key_state[SDL_SCANCODE_DOWN])
    {
        player_movement2.y = -1.0f;
    }
    else if (key_state[SDL_SCANCODE_W]) {
        player_movement.y = 1.0f;
    }
    else if (key_state[SDL_SCANCODE_S]) {
        player_movement.y = -1.0f;
    }

    if (glm::length(player_movement) > 1.0f)
    {
        player_movement = glm::normalize(player_movement);
    }
    if (glm::length(player_movement2) > 1.0f)
    {
        player_movement2 = glm::normalize(player_movement2);
    }
    
}


void update() { 
    
    
    float ticks = (float)SDL_GetTicks() / 1000.0f; 
    float delta_time = ticks - previous_ticks;
    previous_ticks = ticks;

    //make sure paddles don't go out of the window
    if (player_position.y > 4.0f || player_position.y < -4.0f) {
        player_movement.y = -player_movement.y;
    }
    if (player_position2.y > 4.0f || player_position2.y < -4.0f) {
        player_movement2.y = -player_movement2.y;
    }

    //make sure ball bounces when touches the window
    if (ball_position.y > 4.5f || ball_position.y < -4.5f) {
        ball_movement.y = -ball_movement.y;
    }

    //make sure game stops when someone wins
    if (ball_position.x > 4.5f || ball_position.x < -4.5f) {
        ball_movement.x = 0;
        ball_movement.y = 0;
    }

    player_position += player_movement * 3.0f * delta_time;

    model_matrix_left_paddle = glm::mat4(1.0f);
    model_matrix_left_paddle = glm::translate(model_matrix_left_paddle, player_position);
    player_movement.y = 0.0f;

    
    player_position2 += player_movement2 * 3.0f * delta_time;

    model_matrix_right_paddle = glm::mat4(1.0f);
    model_matrix_right_paddle = glm::translate(model_matrix_right_paddle, player_position2);
    player_movement2.y = 0.0f;
    
    ball_position += ball_movement * 2.0f * delta_time;
    model_matrix_ball = glm::mat4(1.0f);
    model_matrix_ball = glm::translate(model_matrix_ball, ball_position);

    //check collision between ball and the paddle
    if (fabs(ball_position.x-player_position2.x)< 0.08f && fabs(ball_position.y - player_position2.y)< 1.3f) {
        ball_movement.x = -ball_movement.x;
    }
    if (fabs(ball_position.x - player_position.x) < 0.08f && fabs(ball_position.y - player_position.y) < 1.3f) {
        ball_movement.x = -ball_movement.x;
    }
    
    
}

void render() {
    
    glClear(GL_COLOR_BUFFER_BIT);
    
    float texture_coordinates[] = {
        0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
    };

    float vertices[] =
    {
        -1.1f, -1.1f, -0.25f, -1.1f, -0.25f, 1.1f,
        -1.1f, -1.1f, -0.25f, 1.1f, -1.1f, 1.1f
    };

    program.SetModelMatrix(model_matrix_left_paddle);

    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program.positionAttribute);

    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texture_coordinates);
    glEnableVertexAttribArray(program.texCoordAttribute);

    glBindTexture(GL_TEXTURE_2D, player_texture_id2);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableVertexAttribArray(program.positionAttribute);
    glDisableVertexAttribArray(program.texCoordAttribute);

    program.SetModelMatrix(model_matrix_right_paddle);

    float vertices2[] =
    {
        0.3f, -1.0f, 0.8f, -1.0f, 0.8f, 1.0f,
        0.3f, -1.0f, 0.8f, 1.0f, 0.3f, 1.0f
    };

    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices2);
    glEnableVertexAttribArray(program.positionAttribute);

    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texture_coordinates);
    glEnableVertexAttribArray(program.texCoordAttribute);

    glBindTexture(GL_TEXTURE_2D, player_texture_id);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableVertexAttribArray(program.positionAttribute);
    glDisableVertexAttribArray(program.texCoordAttribute);


    program.SetModelMatrix(model_matrix_ball);

    float vertices3[] =
    {
        -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f,
        -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f
    };

    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices3);
    glEnableVertexAttribArray(program.positionAttribute);

    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texture_coordinates);
    glEnableVertexAttribArray(program.texCoordAttribute);

    glBindTexture(GL_TEXTURE_2D, player_texture_id3);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableVertexAttribArray(program.positionAttribute);
    glDisableVertexAttribArray(program.texCoordAttribute);  

    SDL_GL_SwapWindow(display_window);
}

void shutdown() { SDL_Quit(); }

int main(int argc, char* argv[])
{
    initialize();

    while (GameIsRunning)
    {
        process_input();

        update();

        render();
    }

    shutdown();
    return 0;
}