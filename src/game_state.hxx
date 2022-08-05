#pragma once

enum class Game_state {
    start_screen_state,
    character_select_state,
    map_select_state,
    match_state,
    after_match_state
};

Game_state get_next_state(Game_state state);