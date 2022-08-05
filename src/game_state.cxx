#include "game_state.hxx"

Game_state
get_next_state(Game_state state) {
    switch (state) {
    case Game_state::start_screen_state:
        return Game_state::character_select_state;
    case Game_state::character_select_state:
        return Game_state::map_select_state;
    case Game_state::map_select_state:
        return Game_state::match_state;
    case Game_state::match_state:
        return Game_state::after_match_state;
    case Game_state::after_match_state:
        return Game_state::character_select_state;
    default:
        return Game_state::start_screen_state;
    }
}