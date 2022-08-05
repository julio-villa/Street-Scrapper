#pragma once

#include <ge211.hxx>

#include "character.hxx"
#include "player.hxx"
#include "game_state.hxx"
#include "select_grid.hxx"
#include "config.hxx"

using Key = ge211::events::Key;

/// Model positions will use 'int' coordinates.
using Position = ge211::Posn<int>;

class Model
{
/// Controller class is a friend of Model and has access to protected.
friend class Controller;

#ifdef CS211_TESTING
    friend struct Test_access;
#endif


public:
    /// Constructor for a model.
    explicit Model(Config const& config = Config());

    const Config& get_config() const
    {
        return config;
    }

    /// Returns the current game_state of the model.
    Game_state get_game_state() const
    {
        return state;
    }

    /// Returns if player one selected a character.
    bool get_player_one_selected_character() const
    {
        return player_one_selected_character;
    }

    /// Returns if player two selected a character.
    bool get_player_two_selected_character() const
    {
        return player_two_selected_character;
    }

    bool get_player_one_selected_map() const
    {
        return player_one_selected_map;
    }

    bool get_player_two_selected_map() const
    {
        return player_two_selected_map;
    }

    /// Returns the position of player one's select cursor.
    int get_player_one_cursor_pos() const
    {
        return select_grid.player_one_cursor_pos;
    }

    /// Returns the position of player two's select cursor.
    int get_player_two_cursor_pos() const
    {
        return select_grid.player_two_cursor_pos;
    }

    /// Called on every frame to move characters to new position.
    void on_frame(double dt);

    void add_players(Character character);

    int decide_map() const;

    const std::vector<Player>& get_players() const
    {
        return players;
    }

    int get_map_number() const
    {
        return map_number;
    }

    double get_timer() const
    {
        return timer;
    }

    player_num get_winner() const
    {
        return winner;
    }

    bool detect_collision(Projectile& projectile);

protected:
    /// Progresses the game to its next game state.
    void progress_game_state();

    void reset_game();

    void move_player_one_cursor(int dir, Game_state state_);

    void move_player_two_cursor(int dir, Game_state state_);

    /// Sets player_one_selected_character to true.
    void player_one_selects_character()
    {
        player_one_selected_character = true;
    }

    /// Sets player_two_selected_character to true.
    void player_two_selects_character()
    {
        player_two_selected_character = true;
    }

    /// Sets player_one_selected_character to false.
    void player_one_unselects_character()
    {
        player_one_selected_character = false;
    }

    /// Sets player_two_selected_character to false.
    void player_two_unselects_character()
    {
        player_two_selected_character = false;
    }

    void player_one_selects_map()
    {
        player_one_selected_map = true;
    }

    void player_two_selects_map()
    {
        player_two_selected_map = true;
    }

    void player_one_unselects_map()
    {
        player_one_selected_map = false;
    }

    void player_two_unselects_map()
    {
        player_two_selected_map = false;
    }

    void on_key_down(ge211::Key key);

    void on_key_up(ge211::Key key);

private:
    Config const config;

    Select_grid select_grid;

    int map_number = 0;

    std::vector<Player> players;

    double timer;

    Game_state state = Game_state::start_screen_state;

    bool player_one_selected_character = false;

    bool player_two_selected_character = false;

    bool player_one_selected_map = false;

    bool player_two_selected_map = false;

    player_num winner;

    void check_game_end();

    void update_projectiles(double dt);

    void update_players(double dt);

    bool detect_projectile_hit(Projectile projectile, Player& other) const;


};
