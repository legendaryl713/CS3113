#include "MainMenu.h"
#include "Utility.h"

#define LEVEL_WIDTH 42
#define LEVEL_HEIGHT 8

MainMenu::~MainMenu()
{
    delete[] this->state.enemies;
    delete    this->state.player;
    delete    this->state.map;
    Mix_FreeChunk(this->state.jump_sfx);
    Mix_FreeMusic(this->state.bgm);
}

void MainMenu::initialise() {
    GLuint font_texture_id = Utility::load_texture("assets/font1.png");

    GLuint font2_texture_id = Utility::load_texture("assets/font2.png");
    GLuint font3_texture_id = Utility::load_texture("assets/font3.png");
    GLuint font4_texture_id = Utility::load_texture("assets/font4.png");
    GLuint font5_texture_id = Utility::load_texture("assets/font5.png");

    state.bg = new Entity();
    state.bg->set_entity_type(BG);
    state.bg->texture_id = Utility::load_texture("assets/bg2.png");
    state.bg->set_position(glm::vec3(-2.5f, 1.0f, 0.0f));

    state.rocks = new Entity[4];
    state.rocks[0].texture_id = Utility::load_texture("assets/font2.png");
    state.rocks[1].texture_id = Utility::load_texture("assets/font3.png");
    state.rocks[2].texture_id = Utility::load_texture("assets/font4.png");
    state.rocks[3].texture_id = Utility::load_texture("assets/font5.png");

    this->state.player = NULL;
    this->state.enemies = NULL;

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);

    state.bgm = Mix_LoadMUS("assets/menu.mp3");

    Mix_PlayMusic(state.bgm, -1);

    Mix_VolumeMusic(MIX_MAX_VOLUME / 4.0f);
}

void MainMenu::update(float delta_time) {

}

void MainMenu::render(ShaderProgram* program){
    float vertices_bg[] = { -7.0f, -4.0f, 5.0f, -4.0f, 5.0f, 4.0f, -7.0f, -4.0f, 5.0f, 4.0f, -7.0f, 4.0f };
    float vertices_font1[] = { -3.0f, 1.68f, 3.0f, 1.68f, 3.0f, 2.7f, -3.0f, 1.68f, 3.0f, 2.7f, -3.0f, 2.7f };
    float vertices_font2[] = { -4.0f, 0.48f, -1.2f, 0.48f, -1.2f, 1.1f, -4.0f, 0.48f, -1.2f, 1.1f, -4.0f, 1.1f };
    float vertices_font3[] = { -0.5f, 0.2f, 4.4f, 0.2f, 4.4f, 1.5f, -0.5f, 0.2f, 4.4f, 1.5f, -0.5f, 1.5f };
    float vertices_font4[] = { -0.5f, -1.7f, 4.4f, -1.7f, 4.4f, -1.3f, -0.5f, -1.7f, 4.4f, -1.3f, -0.5f, -1.3f };

    this->state.bg->render3(program, vertices_bg);
    state.rocks[0].render3(program, vertices_font1);
    state.rocks[1].render3(program, vertices_font2);
    state.rocks[2].render3(program, vertices_font3);
    state.rocks[3].render3(program, vertices_font4);
}