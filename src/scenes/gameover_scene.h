#ifndef _GAMEOVER_SCENE_H_
#define _GAMEOVER_SCENE_H_
#include "types.h"
#include "object.h"
#include "controller.h"

class GameoverController : public Controller
{
public:
    virtual void addAction(Action action) override;
};

class GameoverScreen : public Level
{
public:
    GameoverScreen();
    virtual bool update(float delta) override;
    virtual bool draw(float delta) override;
private:
    float deltaSum;
    Text2D *info;
    Text2D *info2;
};

#endif // _GAMEOVER_SCENE_H_