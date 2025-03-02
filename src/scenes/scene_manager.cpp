#include "title_scene.h"
#include "dummy_scene.h"
#include "scene_manager.h"
#include "display.h"

void SceneManager::switchScene(Scene2D &oldScene, Scene2D &newScene, bool stop)
{
    if (stop)
    {
        oldScene.running = false;
    }
    if (newScene.controller == nullptr)
    {
        newScene.controller = oldScene.controller;
    }
    startMainLoop(newScene);
}

void SceneManager::initializeScenes()
{
    printf("creating scenes\n");
    printf("title\n");
    Scene2D &titleScreen = getScene(SCENE_TITLE);
    titleScreen.name.assign("TITLE");
    titleScreen.currentLevel = new TitleScreen;
    initScene(titleScreen);
    titleScreen.controller = new TitleController;

    printf("level\n");
    Scene2D &scene = getScene(SCENE_LEVEL);
    scene.name.assign("LEVEL");
    scene.currentLevel = new DummyLevel;

    printf("gameover\n");
    Scene2D &gameoverScreen = getScene(SCENE_GAMEOVER);
    gameoverScreen.name.assign("GAMEOVER");
    printf("scenes overall:\n");
    printf("scenes: title @ %p (%s)\n\tgame @ %p (%s)\n\tgameover @ %p (%s)\n",
            &titleScreen, titleScreen.name.c_str(),
            &scene, scene.name.c_str(),
            &gameoverScreen, gameoverScreen.name.c_str());
    printf("start main loop\n");
}

void SceneManager::start(SceneTypes scene)
{
    startMainLoop(scenes[scene]);
}