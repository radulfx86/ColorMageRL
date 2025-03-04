#include <stdio.h>
#include <stdlib.h>

#include "gl_headers.h"
#include "types.h"
// for initGL
#include "display.h"
// for scene setup
#include "scene_manager.h"
// for controller
#include "controller.h"

#include "ecs.h"
#include "state_machine.h"

void mainloop(void *userData)
{
    Scene2D *scene = (Scene2D *)userData;
    printf("%s(Scene: @ %p name: \"%s\")\n", __func__, scene, scene->name.c_str());
    if (scene->currentLevel == nullptr)
    {
        return;
    }
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(scene->bg_color[0], scene->bg_color[1], scene->bg_color[2], scene->bg_color[3]);

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

#include "log.h"

void testArea()
{
    EntityManager &em = EntityManager::getInstance();
    (void)em;
    // log(std::cout, 2, "eins", "zwei") << 3 << 4 << "five\n";
    LOG("hello world", 1, 2, 3);
    EntityID player = em.newEntity("player");

    LOG("player entity id: ", player);

    em.showAll();

    /*

    StateMachine stm({"idle", "moving"});
    State idleState(-1);
    State movingState(-1);
    stm.setState(&idleState, "idle", {"moving"});
    stm.setState(&movingState, "moving", {"idle"});

    stm.log();

    */

    StateMachineManager smm;
    State initialState;
    int idleId = smm.registerState(
            StateInfo{0, stateDefaultTrue,
      stateDefaultTrue,
             stateDefaultTrue,
             stateDefaultTrue});
    int movingId = smm.registerState(
            StateInfo{0, stateDefaultTrue,
             stateDefaultTrue,
             0,
             0
            });
    initialState.stateID = smm.connectStates({idleId, movingId});
    (void) initialState;
}

int main()
{
    testArea();
    printf("main\n");
    printf("initialize openGL\n");
    initGL();



    SceneManager::getInstance().initializeScenes();

    SceneManager::getInstance().start();

    return 0;
}
