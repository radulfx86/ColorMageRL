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
        /*
        bool registerScene(int id, Scene2D &scene)
        {
            scenes[id] = scene;
        }*/

    private:
        SceneManager() {}
        std::array<Scene2D, NUM_SCENES> scenes;
};

#endif // _SCENE_MANAGER_H_