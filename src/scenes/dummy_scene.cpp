#include "dummy_scene.h"
#include <sstream>
#include "object_factory.h"
#include "scene_manager.h"

DummyLevel::DummyLevel() : deltaSum(0)
{
    info = new Text2D;
    info2 = new Text2D;
    float yellow[] = {1.0, 1.0, 0.0};
    float red[] = {1.0, 0.0, 0.0};
    info = ObjectFactory::getText(Vec2{-2, 0}, "Dummy Level", yellow);
    info2 = ObjectFactory::getText(Vec2{-4, -3}, "test", red);
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
    info->draw();
    info2->draw();
    return true;
}