#include <stdio.h>
#include <stdlib.h>

#include "gl_headers.h"
#include "types.h"
#include "display.h"
#include "scene_manager.h"


void mainloop(void *userData)
{
    Scene2D *scene = (Scene2D*) userData;
    printf("%s(Scene: @ %p name: \"%s\")\n", __func__, scene, scene->name.c_str());
}

class TitleController : public Controller
{
public:
    virtual void addAction(Action action) override
    {
        printf("TITLE CONTROLLER %s\n", __func__);
        (void)action;
        Scene2D &title = SceneManager::getInstance().getScene(SCENE_TITLE);
        Scene2D &level = SceneManager::getInstance().getScene(SCENE_LEVEL);
        switchScene(title, level);
    }
};

int main()
{
    printf("main\n");
    Scene2D &scene = SceneManager::getInstance().getScene(SCENE_LEVEL);
    scene.name.assign("LEVEL");
    Scene2D &titleScreen = SceneManager::getInstance().getScene(SCENE_TITLE);
    titleScreen.name.assign("TITLE");
    Scene2D &gameoverScreen = SceneManager::getInstance().getScene(SCENE_GAMEOVER);
    gameoverScreen.name.assign("GAMEOVER");
    printf("scenes: title @ %p (%s)\n\tgame @ %p (%s)\n\tgameover @ %p (%s)\n",
        &titleScreen, titleScreen.name.c_str(),
        &scene, scene.name.c_str(),
        &gameoverScreen, gameoverScreen.name.c_str()
    );
    initScene(titleScreen);
    titleScreen.controller = new TitleController;

    startMainLoop(titleScreen);

    return 0;
}
