#include "types.h"
#include "object.h"
#include "title_scene.h"
#include "object_factory.h"
#include "scene_manager.h"
#include <sstream>

void TitleController::addAction(Action action)
{
    printf("TITLE CONTROLLER %s\n", __func__);
    (void)action;
    Scene2D &title = SceneManager::getInstance().getScene(SCENE_TITLE);
    Scene2D &level = SceneManager::getInstance().getScene(SCENE_LEVEL);
    SceneManager::switchScene(title, level);
}

TitleScreen::TitleScreen() : deltaSum(0)
{
    info = new Text2D;
    info2 = new Text2D;
    float yellow[] = {1.0, 1.0, 0.0};
    float red[] = {1.0, 0.0, 0.0};
    info = ObjectFactory::getText(Vec2{-2, 0}, "hello world", yellow);
    info2 = ObjectFactory::getText(Vec2{-4, -3}, "test", red);
    info->setText("COLOR MAGE");
}

bool TitleScreen::update(float delta)
{
    (void) delta;
    deltaSum += delta;
    if ( deltaSum > 10.0 )
    {
        deltaSum -= 1.0;
        info->setText("asdf MEH asdf");
    }
    std::stringstream sstr;
    sstr << "delta: " << delta;
    info2->setText(sstr.str());
    // nothing to to   
    return false;
}

bool TitleScreen::draw(float delta)
{
    (void) delta;
    info->draw();
    info2->draw();
    return true;
}