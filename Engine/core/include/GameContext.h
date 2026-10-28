#pragma once
#include "EngineAssets.h"
#include "AssetManager.h"
#include "Input/MouseHandler.h"
#include "Input/KeyboardHandler.h"


struct GameContext {

    // Global assets
    AssetManager asset_manager;
    EngineAssets engine_assets;

    // Input
    MouseHandler mouse_handler;
    KeyboardHandler keyboard_handler;
};
