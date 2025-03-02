#include <stdio.h>
#include <stdlib.h>

#include "gl_headers.h"
#include "types.h"
#include "display.h"
#include "scene_manager.h"
#include "object.h"
#include "object_factory.h"
#include "controller.h"

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

int main()
{
    printf("main\n");
    printf("initialize openGL\n");
    initGL();

    SceneManager::getInstance().initializeScenes();

    SceneManager::getInstance().start();

    return 0;
}
