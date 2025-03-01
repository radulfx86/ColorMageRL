#include <stdio.h>
#include <stdlib.h>

#include "gl_headers.h"
#include "types.h"
#include "display.h"
#include "scene_manager.h"


void mainloop(void *userData)
{
    Scene2D *scene = (Scene2D*) userData;
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
    Scene2D &titleScreen = SceneManager::getInstance().getScene(SCENE_TITLE);
    Scene2D &gameoverScreen = SceneManager::getInstance().getScene(SCENE_GAMEOVER);
    initScene(titleScreen);
    titleScreen.controller = new TitleController;

    startMainLoop(titleScreen);

    return 0;
}
