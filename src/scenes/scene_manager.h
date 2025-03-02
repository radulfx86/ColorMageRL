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

    void initializeScenes();

    void start(SceneTypes scene = SCENE_TITLE);

private:
    SceneManager() {}
    std::array<Scene2D, NUM_SCENES> scenes;
};


#endif // _SCENE_MANAGER_H_