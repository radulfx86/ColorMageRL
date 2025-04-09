#ifndef _ANIMATIONS_H_
#define _ANIMATIONS_H_
#include "ecs.h"
#include "log.h"
#include "types.h"
#include "object.h"

typedef bool (*AnimationUpdater)(Object2D *tgt, MotionParameters_t *params, float delta);

class AnimationSystem : public System{
public:
    AnimationSystem() : System(EntityManager::getInstance().newSystem("animations"))
    {
        components.set(EntityManager::getInstance().getComponentID<MotionParameters_t *>());
        components.set(EntityManager::getInstance().getComponentID<Object2D *>());
        components.set(EntityManager::getInstance().getComponentID<AnimationUpdater>());
    }

    virtual void update(float deltaTimeS) override
    {
        for ( EntityID eid : EntityManager::getInstance().getSystemEntities(id) )
        {
            AnimationUpdater updater = EntityManager::getInstance().getComponent<AnimationUpdater>(eid);
            MotionParameters_t *params = EntityManager::getInstance().getComponent<MotionParameters_t*>(eid);
            Object2D *tgt = EntityManager::getInstance().getComponent<Object2D*>(eid);
            LOG("updating AnimationsSystem for eid ", eid, " object at ", tgt);
            updater(tgt, params, deltaTimeS);
        }
    }
};

#endif // _ANIMATIONS_H_