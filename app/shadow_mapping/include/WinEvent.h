#ifndef __WIN_EVENT_H_
#define __WIN_EVENT_H_

#include "Common/esUtil.h"

void MouseMoveEvent(ESContext *esContext, int pointX, int pointY);

void MouseWheelEvent(ESContext *esContext, int direct);

void KeyEvent(ESContext *esContext, unsigned char key, int pointX, int pointY);

#endif
