#include "dummy_scene.h"
#include <sstream>
#include "object_factory.h"
#include "scene_manager.h"
#include "ecs.h"
#include "tools.h"
#include <set>

EntityManager &em = EntityManager::getInstance();

DummyLevel::DummyLevel() : deltaSum(0)
{
    info = new Text2D;
    info2 = new Text2D;
    float yellow[] = {1.0, 1.0, 0.0};
    float red[] = {1.0, 0.0, 0.0};
    info = ObjectFactory::getText(Vec2{-2, 0}, "Dummy Level", yellow);
    info2 = ObjectFactory::getText(Vec2{-4, -3}, "test", red);
    info->setPosition(Vec2{0,0});

    EntityID levelData = em.newEntity("leve_data");
    LevelData *currentLevelData = new LevelData;
    currentLevelData->width = 100;
    currentLevelData->height = 100;
    currentLevelData->id = 0;
    currentLevelData->data = new bool[currentLevelData->width * currentLevelData->height];
    em.addComponent<LevelData*>(levelData, currentLevelData);

    player = em.newEntity("player");
    Object2D *playerGraphics = new Object2D;
    GLuint program = createShader(loadText("shaders/simple.vs").c_str(), loadText("shaders/simple.fs").c_str());
    playerGraphics->tex = loadTexture("assets/images/characters.png");
    ObjectFactory::createObject(*playerGraphics, program);

    em.addComponent<Object2D*>(player, playerGraphics);
    Bounds *playerBounds = new Bounds;
    playerBounds->pos = {0,0};
    playerBounds->size = {1,1};
    em.addComponent<Bounds*>(player, playerBounds);

    ActionQueue *playerActions = new ActionQueue;
    playerActions->maxActions = 1;
    playerActions->skip = false;
    em.addComponent<ActionQueue*>(player, playerActions);

    /// TODO check why the system does not work with the bounds !!!
    Components drawingComponents;
    drawingComponents.set(em.getComponentID<Object2D *>());
    //drawingComponents.set(em.getComponentID<Bounds>());
    drawingSystem = em.addSystem(drawingComponents, "drawing");
    em.updateSystem(drawingSystem);

    Components actionsComponents;
    actionsComponents.set(em.getComponentID<ActionQueue*>());
    actionsSystem = em.addSystem(actionsComponents, "actions");

    em.showAll();



}

void DummyLevel::handleAction(EntityID eid, Action action)
{
    switch (action.type)
    {
    case Action::MOTION:
    {
        Object2D *target = em.getComponent<Object2D*>(eid);
        Bounds *bounds = em.getComponent<Bounds*>(eid);
        if ( target )
        {
            Vec2 delta{(float)action.value_i.x, (float)action.value_i.y};
            printf("handleAction(%d)\n", eid);
            bounds->pos = bounds->pos + delta;   
            target->setPosition(target->pos + delta);
        }
        break;
    }
    case Action::ATTACK:
        break;
    case Action::INTERACT:
        break;
    case Action::SPECIAL:
        break;
    default:
        break;
    }
}

bool DummyLevel::update(float delta)
{
    deltaSum += delta;
    if ( deltaSum > 1.0 )
    {
        deltaSum -= 1.0;
        std::stringstream sstr;
        sstr << "round: " << SceneManager::getInstance().getCurrentRound();
        info2->setText(sstr.str());
        frames = 0;
    }

    // player actions
    ActionQueue *playerActions = em.getComponent<ActionQueue*>(player);
    if ( ((int)playerActions->elements.size() >= playerActions->maxActions)
        || playerActions->skip )
    {
        for ( int idxAction = 0;
            idxAction < playerActions->maxActions && not playerActions->elements.empty();
            ++idxAction, playerActions->elements.pop() )
        {
            handleAction(player, playerActions->elements.front());
        }
        printf("elements left: %lu\n", playerActions->elements.size());
        for (auto entity : em.getSystemEntities(actionsSystem))
        {
            ActionQueue *actions = em.getComponent<ActionQueue*>(entity);

            for ( int idxAction = 0;
                    not actions->elements.empty();
                    actions->elements.pop(), ++idxAction )
            {
                handleAction(entity, actions->elements.front());
            }
        }
        SceneManager::getInstance().incrementRound();
    }


    ++frames;
    // nothing to to   
    return false;
}

struct cmpObjects
{
    bool operator()(Object2D *a, Object2D *b) const
    {
        return a->pos.y > b->pos.y;
    }
};

bool DummyLevel::draw(float delta)
{
    std::multiset<Object2D*, cmpObjects> drawSet;
    for (auto entity : em.getSystemEntities(drawingSystem))
    {
        Bounds *b = em.getComponent<Bounds *>(entity);
        Object2D *t = em.getComponent<Object2D *>(entity);
        printf("draw entity %d\n", entity);
        if ( t )
        {
            printf("DBG entity#%d bounds at %p\n", entity, b);
            printf("\tpos at %f %f\n", b->pos.x, b->pos.y);
            /// TODO enable this again -> set the position of the bounds correctly
            t->setPosition(b->pos);
            t->updateAnimation(delta);
            drawSet.insert(t);
            // s.getComponent<Object2D *>(entity)->update(delta_s);
            printf("entity %d pos at %f %f\n", entity, b->pos.x, b->pos.y);
            // s.getComponent<Tile *>(entity)->printPointers();
        }
        else
        {
            printf("UNKOWN OBJECT FOR ENTITY %d\n", entity);
        }
    }
    drawSet.insert(info);
    drawSet.insert(info2);
    unsigned int cnt = 0;
    for ( Object2D *o : drawSet )
    {
        printf("drawing object %u at %f %f\n", cnt, o->pos.x, o->pos.y );
        ++cnt;
        o->draw();
    }
    return true;
}