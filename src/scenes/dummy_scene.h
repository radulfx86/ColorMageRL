#ifndef _DUMMY_SCENE_H_
#define _DUMMY_SCENE_H_
#include "types.h"
#include "object.h"

class DummyLevel : public Level
{
public:
    DummyLevel();
    virtual bool update(float delta) override;
    virtual bool draw(float delta) override;
private:
    Text2D *info;
    float deltaSum;
    Text2D *info2;
    int frames;
};
#endif // _DUMMY_SCENE_H_