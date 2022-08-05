#include "select_grid.hxx"

Select_grid::Select_grid(Config config)
    : config(config),
      player_one_cursor_pos(0),
      player_two_cursor_pos(0)
{

}

void
Select_grid::move_player_one_cursor(int dir, Game_state state_)
{
    player_one_cursor_pos += dir;
    if (state_ == Game_state::character_select_state) {
        if (player_one_cursor_pos < 0) {
            player_one_cursor_pos = config.all_characters.size() - 1;
        }
        else if ((unsigned int)player_one_cursor_pos > config.all_characters
        .size() - 1) {
            player_one_cursor_pos = 0;
        }
    }
    else if (state_ == Game_state::map_select_state) {
        if (player_one_cursor_pos < 0) {
            player_one_cursor_pos = config.num_backgrounds - 1;
        }
        else if ((unsigned int)player_one_cursor_pos > 3) {
            player_one_cursor_pos = 0;
        }
    }
}

void
Select_grid::move_player_two_cursor(int dir, Game_state state_)
{
    player_two_cursor_pos += dir;
    if (state_ == Game_state::character_select_state) {
        if (player_two_cursor_pos < 0) {
            player_two_cursor_pos = config.all_characters.size() - 1;
        }
        else if ((unsigned int)player_two_cursor_pos > config.all_characters.size() - 1) {
            player_two_cursor_pos = 0;
        }
    }
    else if (state_ == Game_state::map_select_state) {
        if (player_two_cursor_pos < 0) {
            player_two_cursor_pos = config.num_backgrounds - 1;
        }
        else if ((unsigned int)player_two_cursor_pos > 3) {
            player_two_cursor_pos = 0;
        }
    }
}

void
Select_grid::reset_player_cursors()
{
    player_one_cursor_pos = 0;
    player_two_cursor_pos = 0;
}