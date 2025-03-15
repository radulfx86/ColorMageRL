#include "display.h"
#include "gl_headers.h"
#include "controller.h"
#include "scene_manager.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#include <emscripten/html5_webgl.h>

EM_BOOL mouse_callback(int eventType, const EmscriptenMouseEvent *e, void *userData)
{
    printf("mouse clicked at %d %d\n", e->clientX, e->clientY);
    return 0;
}

EM_BOOL touch_callback(int eventType, const EmscriptenTouchEvent *e, void *userData)
{
    printf("mouse clicked at %d %d\n", e->touches[0].clientX, e->touches[0].clientY);
    return 0;
}

EM_BOOL keydown_callback(int eventType, const EmscriptenKeyboardEvent *keyEvent, void *userData)
{
    Scene2D *scene = (Scene2D *)userData;
    switch (keyEvent->keyCode)
    {
    case 0x1B:  // escape
        scene->running = false;
        break;
    case 0x27:  // right arrow
        scene->controller->addAction(Action{Action::MOTION, Vec2i{1, 0}, {0}, true});
        break;
    case 0x25:  // left arrow
        scene->controller->addAction(Action{Action::MOTION, Vec2i{-1, 0}, {0}, true});
        break;
    case 0x26:  // up arrow
        scene->controller->addAction(Action{Action::MOTION, Vec2i{0, 1}, {0}, true});
        break;
    case 0x28:  // down arrow
        scene->controller->addAction(Action{Action::MOTION, Vec2i{0, -1}, {0}, true});
        break;
    case 0x20:  // space
        scene->controller->addAction(Action{Action::INTERACT, {0}, {0}, true});
        break;
    default:
        break;
    }
    return 0;
}

EM_BOOL keyup_callback(int eventType, const EmscriptenKeyboardEvent *keyEvent, void *userData)
{
    Scene2D *scene = (Scene2D *)userData;
    switch (keyEvent->keyCode)
    {
    case 0x1B:  // escape
        scene->running = false;
        break;
    case 0x27:  // right arrow
        scene->controller->addAction(Action{Action::MOTION, Vec2i{1, 0}, {0}, false});
        break;
    case 0x25:  // left arrow
        scene->controller->addAction(Action{Action::MOTION, Vec2i{-1, 0}, {0}, false});
        break;
    case 0x26:  // up arrow
        scene->controller->addAction(Action{Action::MOTION, Vec2i{0, 1}, {0}, false});
        break;
    case 0x28:  // down arrow
        scene->controller->addAction(Action{Action::MOTION, Vec2i{0, -1}, {0}, false});
        break;
    case 0x20:  // space
        scene->controller->addAction(Action{Action::INTERACT, {0}, {0}, false});
        break;
    default:
        break;
    }
    return 0;
}

void initGL(const char *title)
{
    emscripten_set_window_title(title);
    EmscriptenWebGLContextAttributes attrs;
    emscripten_webgl_init_context_attributes(&attrs);

    attrs.enableExtensionsByDefault = 1;
    attrs.majorVersion = 2;
    attrs.minorVersion = 0;

    emscripten_set_canvas_element_size("#canvas", 640, 480);
    EMSCRIPTEN_WEBGL_CONTEXT_HANDLE context = emscripten_webgl_create_context("#canvas", &attrs);
    if (!context)
    {
        printf("failed to create context\n");
        exit(1);
    }

    emscripten_webgl_make_context_current(context);

    glViewport(0,0,640,480);
}

void initScene(Scene2D &scene)
{
    EmscriptenWebGLContextAttributes attrs;
    emscripten_webgl_init_context_attributes(&attrs);

    attrs.enableExtensionsByDefault = 1;
    attrs.majorVersion = 2;
    attrs.minorVersion = 0;

    emscripten_set_canvas_element_size("#canvas", 640, 480);
    EMSCRIPTEN_WEBGL_CONTEXT_HANDLE context = emscripten_webgl_create_context("#canvas", &attrs);
    if (!context)
    {
        printf("failed to create context\n");
        exit(1);
    }

    emscripten_webgl_make_context_current(context);

    printf("context\n");

    printf("setup callbacks\n");

    emscripten_set_click_callback("#canvas", 0, 1, mouse_callback);
    emscripten_set_touchend_callback("#canvas", 0, 1, touch_callback);
    emscripten_set_keydown_callback("#canvas", &scene, 1, keydown_callback);
    emscripten_set_keyup_callback("#canvas", &scene, 1, keyup_callback);
}

void startMainLoop(Scene2D &scene)
{
    emscripten_cancel_main_loop();
    emscripten_set_main_loop_arg(mainloop, &scene, 0, 0);
}

uint64_t getNow()
{
    return (uint64_t) emscripten_performance_now();
}
#else
uint64_t getNow()
{
    return SDL_GetTicks64();
}
void handleInput(Scene2D &scene)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)
        {
            switch (event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                scene.running = false;
                break;
            case SDLK_F1:
                SceneManager::switchScene(scene, SceneManager::getInstance().getScene(SCENE_TITLE));
                break;
            case SDLK_RIGHT:
                scene.controller->addAction(Action{Action::MOTION, Vec2i{1,0}, {0,0}, event.type == SDL_KEYDOWN});
                break;
            case SDLK_LEFT:
                scene.controller->addAction(Action{Action::MOTION, Vec2i{-1,0}, {0,0}, event.type == SDL_KEYDOWN});
                break;
            case SDLK_UP:
                scene.controller->addAction(Action{Action::MOTION, Vec2i{0,1}, {0,0}, event.type == SDL_KEYDOWN});
                break;
            case SDLK_DOWN:
                scene.controller->addAction(Action{Action::MOTION, Vec2i{0,-1}, {0,0}, event.type == SDL_KEYDOWN});
                break;
            case SDLK_SPACE:
                scene.controller->addAction(Action{Action::INTERACT, {0,0}, {0,0}, event.type == SDL_KEYDOWN});
                break;
            case SDLK_q:
                scene.controller->addAction(Action{Action::SPECIAL, {0,0}, {0,0}, event.type == SDL_KEYDOWN});
                break;
            default:
                break;
            }
        }
        else if ( event.type == SDL_MOUSEBUTTONDOWN )
        {
            Vec2i mousePos;
            SDL_GetMouseState( &mousePos.x, &mousePos.y);
            scene.controller->addAction(Action{Action::SPECIAL, mousePos, {0,0}, true});
        }
    }
}

SDL_Window *window;

void startMainLoop(Scene2D &scene)
{
    scene.running = true;
    while ( scene.running )
    {
        handleInput(scene);
        mainloop(&scene);
        //if ( scene.tick % 10 == 1)
        SDL_GL_SwapWindow(window);
    }
}

void initGL(const char *title)
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    window = SDL_CreateWindow(title, 0, 0, 512, 512, SDL_WINDOW_OPENGL);
    SDL_GL_CreateContext(window);

    glViewport(0,0,640,480);
}

void initScene(Scene2D &scene)
{
    scene.running = false;
}

#endif


void cleanScene(Scene2D &scene)
{
    (void) scene;
}
