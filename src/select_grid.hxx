#pragma once

#include "character.hxx"
#include "game_state.hxx"
#include "config.hxx"

#include <ge211.hxx>

class Select_grid {

friend class Model;

friend class Controller;

#ifdef CS211_TESTING
    friend struct Test_access;
#endif

public:
    Select_grid(Config config);

protected:
    void move_player_one_cursor(int dir, Game_state state);

    void move_player_two_cursor(int dir, Game_state state);

    void reset_player_cursors();

private:
    Config config;

    int player_one_cursor_pos;

    int player_two_cursor_pos;

};