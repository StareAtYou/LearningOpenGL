#include "WinEvent.h"
#include "config.h"
#include "common.h"
#include "log.h"
#include <iostream>

static bool firstMouse = true;
static bool isEnableMouse = false;
static float lastX = SCR_WIDTH / 2.0f;
static float lastY = SCR_HEIGHT / 2.0f;

void MouseMoveEvent(ESContext *esContext, int pointX, int pointY)
{
    LOG_DEBUG("pointX: %d, pointY: %d", pointX, pointY);

    if (!isEnableMouse) {
        return;
    }

    UserData *userData = (UserData *)esContext->userData;

    float xpos = static_cast<float>(pointX);
    float ypos = static_cast<float>(pointY);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    userData->camera.ProcessMouseMovement(xoffset, yoffset);
}

void MouseWheelEvent(ESContext *esContext, int direct)
{
    LOG_DEBUG("direct: %d", direct);

    if (!isEnableMouse) {
        isEnableMouse = true;
        LOG_DEBUG("Enable mouse event on windows!");
    }

    UserData *userData = (UserData *)esContext->userData;

    userData->camera.ProcessMouseScroll(static_cast<float>(direct)/100);
}

void KeyEvent(ESContext *esContext, unsigned char key, int pointX, int pointY)
{
    LOG_DEBUG("Key: %c, pointX: %d, pointY: %d", key, pointX, pointY);

    UserData *userData = (UserData *)esContext->userData;

    float xpos = static_cast<float>(pointX);
    float ypos = static_cast<float>(pointY);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    userData->camera.ProcessMouseMovement(xoffset, yoffset);
}