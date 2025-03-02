#ifndef _TITLE_SCENE_H_
#define _TITLE_SCENE_H_
#include "types.h"
#include "object.h"
#include "controller.h"

class TitleController : public Controller
{
public:
    virtual void addAction(Action action) override;
};

class TitleScreen : public Level
{
public:
    TitleScreen();
    virtual bool update(float delta) override;
    virtual bool draw(float delta) override;
private:
    float deltaSum;
    Text2D *info;
    Text2D *info2;
};

#endif // _TITLE_SCENE_H_