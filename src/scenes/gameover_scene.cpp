#include "types.h"
#include "object.h"
#include "gameover_scene.h"
#include "object_factory.h"
#include "scene_manager.h"
#include <sstream>

void GameoverController::addAction(Action action)
{
    printf("GAMEOVER CONTROLLER %s\n", __func__);
    (void)action;
    Scene2D &gameover = SceneManager::getInstance().getScene(SCENE_GAMEOVER);
    Scene2D &level = SceneManager::getInstance().getScene(SCENE_LEVEL);
    SceneManager::switchScene(gameover, level);
}

GameoverScreen::GameoverScreen() : deltaSum(0)
{
    info = new Text2D;
    info2 = new Text2D;
    float red[] = {1.0, 0.0, 0.0};
    info = ObjectFactory::getText(Vec2{-2, 2}, "GAME OVER", red);
    info2 = ObjectFactory::getText(Vec2{-4, -3}, "test", red);
    info->setText("COLOR MAGE");
}

bool GameoverScreen::update(float delta)
{
    (void) delta;
    deltaSum += delta;
    if ( deltaSum > 0.2 )
    {
        deltaSum -= 0.2;
        info->setPosition(info->pos + Vec2{0.0f,0.1f});
    }
    std::stringstream sstr;
    sstr << "delta: " << delta;
    info2->setText(sstr.str());
    // nothing to to   
    return false;
}

bool GameoverScreen::draw(float delta)
{
    (void) delta;
    info->draw();
    info2->draw();
    return true;
}