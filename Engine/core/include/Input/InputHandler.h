#pragma once
#include "Input/KeyboardHandler.h"
#include "Input/MouseHandler.h"

struct InputHandler {
    KeyboardHandler keyboard_handler;
    MouseHandler mouse_handler;
};

