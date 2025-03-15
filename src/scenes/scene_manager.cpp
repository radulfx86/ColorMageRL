#include "title_scene.h"
#include "dummy_scene.h"
#include "gameover_scene.h"
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

void SceneManager::initializeScenes(Level *title, Level *level, Level *gameover)
{
    printf("creating scenes\n");
    printf("title\n");
    Scene2D &titleScreen = getScene(SCENE_TITLE);
    titleScreen.name.assign("TITLE");
    if ( title )
    {
        titleScreen.currentLevel = title;
    }
    else
    {
        titleScreen.currentLevel = new TitleScreen;
    }
    initScene(titleScreen);
    titleScreen.controller = new TitleController;
    titleScreen.bg_color[0] = 0.3;
    titleScreen.bg_color[1] = 0.1;
    titleScreen.bg_color[2] = 0.0;
    titleScreen.bg_color[3] = 1.0;

    printf("level\n");
    Scene2D &scene = getScene(SCENE_LEVEL);
    scene.name.assign("LEVEL");
    if ( level )
    {
        scene.currentLevel = level;
    }
    else
    {
        scene.currentLevel = new DummyLevel;
    }
    scene.controller = new GameController(((DummyLevel*)scene.currentLevel)->getTarget());
    scene.bg_color[0] = 0.0;
    scene.bg_color[1] = 0.2;
    scene.bg_color[2] = 0.2;
    scene.bg_color[3] = 1.0;

    printf("gameover\n");
    Scene2D &gameoverScreen = getScene(SCENE_GAMEOVER);
    if ( gameover )
    {
        gameoverScreen.currentLevel = gameover;
    }
    else
    {
        gameoverScreen.currentLevel = new GameoverScreen;
    }
    gameoverScreen.name.assign("GAMEOVER");
    gameoverScreen.bg_color[0] = 0.4;
    gameoverScreen.bg_color[1] = 0.0;
    gameoverScreen.bg_color[2] = 0.0;
    gameoverScreen.bg_color[3] = 1.0;

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

void SceneManager::incrementRound()
{
    ++round;
    for ( Scene2D &scene : scenes )
    {
        ++scene.round;
    }
}
