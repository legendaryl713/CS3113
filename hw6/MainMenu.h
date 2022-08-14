#include "Scene.h"

class MainMenu : public Scene {
public:
    int ENEMY_COUNT = 0;
    bool started = false;

    ~MainMenu();

    void initialise() override;
    void update(float delta_time) override;
    void render(ShaderProgram* program) override;
};