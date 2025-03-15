#ifndef _SCENE_MANAGER_H_
#define _SCENE_MANAGER_H_
#include "types.h"
#include <array>

// singleton scene manager
class SceneManager
{
public:
    static SceneManager &getInstance()
    {
        static SceneManager sceneManager;
        return sceneManager;
    }
    Scene2D &getScene(SceneTypes id)
    {
        return scenes[id];
    }

    static void switchScene(Scene2D &oldScene, Scene2D &newScene, bool stop = false);

    void initializeScenes(Level *title = 0, Level *level = 0, Level *gameover = 0);

    void start(SceneTypes scene = SCENE_TITLE);

    void incrementRound();

    int getCurrentRound() const
    {
        return round;
    }

private:
    SceneManager() : round(0) {}
    std::array<Scene2D, NUM_SCENES> scenes;
    int round;
};


#endif // _SCENE_MANAGER_H_