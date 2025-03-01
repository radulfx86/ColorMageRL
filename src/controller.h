#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_
#include "types.h"


class Controller
{
public:
    Controller()
    {

    }
    virtual void addAction(Action action)
    {
        (void) action;
    }
    virtual void update(float delta)
    {
        (void) delta;
    }
};

#endif // _CONTROLLER_H_