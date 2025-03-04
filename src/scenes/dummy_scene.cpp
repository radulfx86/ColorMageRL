#include "dummy_scene.h"
#include <sstream>
#include "object_factory.h"
#include "scene_manager.h"
#include "ecs.h"

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


    EntityID player = em.newEntity("player");
    Object2D *playerGraphics = new Object2D;
    GLuint program = createShader(loadText("shaders/simple.vs").c_str(), loadText("shaders/simple.fs").c_str());
    playerGraphics->tex = loadTexture("assets/images/characters.png");
    ObjectFactory::createObject(*playerGraphics, program);

    em.addComponent<Object2D*>(player, playerGraphics);
    Bounds *playerBounds = new Bounds;
    playerBounds->pos = {0,0};
    playerBounds->size = {1,1};
    em.addComponent<Bounds*>(player, playerBounds);


    Components drawingComponents;
    drawingComponents.set(em.getComponentID<Object2D *>());
    //drawingComponents.set(em.getComponentID<Bounds>());
    
    drawingSystem = em.addSystem(drawingComponents, "drawing");
    em.updateSystem(drawingSystem);

    em.showAll();

}

bool DummyLevel::update(float delta)
{
    deltaSum += delta;
    if ( deltaSum > 1.0 )
    {
        deltaSum -= 1.0;
        std::stringstream sstr;
        sstr << "FPS: " << frames;
        info2->setText(sstr.str());
        frames = 0;
    }



    ++frames;
    // nothing to to   
    return false;
}

bool DummyLevel::draw(float delta)
{
    (void) delta;
    for (auto entity : em.getSystemEntities(drawingSystem))
    {
        Bounds *b = em.getComponent<Bounds *>(entity);
        Object2D *t = em.getComponent<Object2D *>(entity);
        printf("draw entity %d\n", entity);
        if ( t )
        {
            printf("DBG entity#%d bounds at %p\n", entity, b);
            printf("\tpos at %f %f\n", b->pos.x, b->pos.y);
            t->setPosition(b->pos);
            // s.getComponent<Object2D *>(entity)->update(delta_s);
            t->updateAnimation(delta);
            t->draw();
            printf("entity %d pos at %f %f\n", entity, b->pos.x, b->pos.y);
            // s.getComponent<Tile *>(entity)->printPointers();
        }
        else
        {
            printf("UNKOWN OBJECT FOR ENTITY %d\n", entity);
        }
    }
    info->draw();
    info2->draw();
    return true;
}