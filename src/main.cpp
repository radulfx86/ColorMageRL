#include <stdio.h>
#include <stdlib.h>

#include "gl_headers.h"
#include "types.h"
#include "display.h"
#include "scene_manager.h"
#include "object.h"
#include "object_factory.h"
#include "title_scene.h"
#include "dummy_scene.h"



void mainloop(void *userData)
{
    Scene2D *scene = (Scene2D *)userData;
    printf("%s(Scene: @ %p name: \"%s\")\n", __func__, scene, scene->name.c_str());
    if (scene->currentLevel == nullptr)
    {
        return;
    }
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.2,0.1,0.0,1.0);

    uint64_t now = getNow();
    if (scene->last == 0)
    {
        scene->last = now;
    }
    float delta = (now - scene->last) / 1000.0;
    scene->controller->update(delta);
    scene->currentLevel->update(delta);
    scene->currentLevel->draw(delta);

    printf("FPS: %f\n", 1.0 / delta);

    scene->last = now;
}

class GameController : public Controller
{
public:
    GameController(Object2D &obj) : target(obj)
    {
    }
    virtual void addAction(Action action) override
    {
        switch (action.type)
        {
        case Action::MOTION:
            target.pos = target.pos + action.value_f;
            break;
        case Action::ATTACK:
            break;
        case Action::INTERACT:
            break;
        case Action::SPECIAL:
            break;
        default:
            break;
        }
    }
private:
    Object2D &target;
};


int main()
{
    printf("main\n");
    printf("initialize openGL\n");
    initGL();
    printf("creating scenes\n");
    printf("title\n");
    Scene2D &titleScreen = SceneManager::getInstance().getScene(SCENE_TITLE);
    titleScreen.name.assign("TITLE");
    titleScreen.currentLevel = new TitleScreen;
    initScene(titleScreen);
    titleScreen.controller = new TitleController;
    printf("level\n");
    Scene2D &scene = SceneManager::getInstance().getScene(SCENE_LEVEL);
    scene.name.assign("LEVEL");
    scene.currentLevel = new DummyLevel;
    printf("gameover\n");
    Scene2D &gameoverScreen = SceneManager::getInstance().getScene(SCENE_GAMEOVER);
    gameoverScreen.name.assign("GAMEOVER");
    printf("scenes overall:\n");
    printf("scenes: title @ %p (%s)\n\tgame @ %p (%s)\n\tgameover @ %p (%s)\n",
        &titleScreen, titleScreen.name.c_str(),
        &scene, scene.name.c_str(),
        &gameoverScreen, gameoverScreen.name.c_str()
    );
    printf("start main loop\n");

    startMainLoop(titleScreen);

    return 0;
}
