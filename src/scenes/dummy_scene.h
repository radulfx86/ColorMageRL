#ifndef _DUMMY_SCENE_H_
#define _DUMMY_SCENE_H_
#include "types.h"
#include "object.h"
#include "controller.h"
#include "state_machine.h"
#include "io.h"
#include "shaders.h"


typedef struct LevelData
{
    int id;
    int width;
    int height;
    bool *data;
} LevelData;

class DummyLevel : public Level
{
public:
    DummyLevel();
    virtual bool update(float delta) override;
    virtual bool draw(float delta) override;
    EntityID getTarget() { return player; }
    void handleAction(EntityID eid, Action action);
private:
    EntityID player;
    Text2D *info;
    float deltaSum;
    Text2D *info2;
    int frames;
    StateMachineManager statesManager;
    SystemID drawingSystem;
    SystemID actionsSystem;
    int activeLevelData;
};

class GameController : public Controller
{
public:
    GameController(EntityID target) : target(target)
    {
    }
    virtual void addAction(Action action) override
    {
        /// key up
        if ( not action.value_b )
        {
            ActionQueue *queue = EntityManager::getInstance().getComponent<ActionQueue*>(target);
            queue->elements.push(action);
        }
    }

private:
    EntityID target;
};

#endif // _DUMMY_SCENE_H_