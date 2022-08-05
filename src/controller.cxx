#include "controller.hxx"
#include <iostream>

Controller::Controller()
        : model_(),
          config(model_.get_config()),
          view_(model_)
{
    set_soundtrack(musictrack1);
}

void
Controller::draw(ge211::Sprite_set& set)
{
    view_.draw(set);
}

void
Controller::on_frame(double dt)
{
    model_.on_frame(dt);
    if (model_.state == Game_state::match_state && music_track_one_playing) {
        music_track_one_playing = false;
        music_track_two_playing = true;
        set_soundtrack(musictrack2);
    }
    else if (model_.state == Game_state::after_match_state &&
    music_track_two_playing) {
        music_track_two_playing = false;
        music_track_one_playing = true;
        set_soundtrack(musictrack1);
    }
}

void
Controller::set_soundtrack(ge211::audio::Music_track track)
{
    if (ge211::Abstract_game::mixer().get_music_state() ==
    ge211::Mixer::State::playing) {
        ge211::Abstract_game::mixer().pause_music();
        ge211::Abstract_game::mixer().attach_music(track);
        ge211::Abstract_game::mixer().play_music(track, true);
    }
    else {
        ge211::Abstract_game::mixer().attach_music(track);
        ge211::Abstract_game::mixer().play_music(track, true);
    }
}

void
Controller::on_key(Key key)
{
    if (model_.state == Game_state::start_screen_state) {
        model_.progress_game_state();
    }
    else if (model_.state == Game_state::character_select_state) {
        on_key_character_select_state(key);
    }
    else if (model_.state == Game_state::map_select_state) {
        on_key_map_select_state(key);
    }
    else if (model_.state == Game_state::after_match_state) {
        on_key_after_match_state(key);
    }
}

void
Controller::on_key_character_select_state (Key key)
{
    if (!model_.player_one_selected_character) {
        // If key is d, move respective player's cursor to the right
        if (key == Key::code('d')) {
            model_.move_player_one_cursor(1, model_.state);
        }
        // If key is a, move respective player's cursor to the left
        else if (key == Key::code('a')) {
            model_.move_player_one_cursor(-1, model_.state);
        }
        // If key is x, player selects character at current cursor pos
        else if (key == Key::code('x')) {
            model_.player_one_selects_character();
        }
    }
    else {
        if (key == Key::code('c')) {
            model_.player_one_unselects_character();
        }
    }
    if (!model_.player_two_selected_character) {
        // If key is right arrow, move respective player's cursor to the right
        if (key == Key::right()) {
            model_.move_player_two_cursor(1, model_.state);
        }
        // If key is left arrow, move respective player's cursor to the left
        else if (key == Key::left()) {
            model_.move_player_two_cursor(-1, model_.state);
        }
        // If key is j, player selects character at current cursor pos
        else if (key == Key::code('j')) {
            model_.player_two_selects_character();
        }
    }
    else {
        if (key == Key::code('k')) {
            model_.player_two_unselects_character();
        }
    }
    if (key == Key::code('y') && model_
            .player_one_selected_character && model_
                .player_two_selected_character) {
        std::string name1 = config.all_characters[model_
                                                  .get_player_one_cursor_pos()];
        std::string name2 = config.all_characters[model_
                                                  .get_player_two_cursor_pos()];
        Character charac1 = Character(name1, Position(config
        .window_dimensions.width * 0.25, config.floor_y), config);
        Character charac2 = Character(name2, Position(config
        .window_dimensions.width * 0.75, config.floor_y), config);
        model_.add_players(charac1);
        model_.add_players(charac2);
        model_.select_grid.reset_player_cursors();
        model_.progress_game_state();
    }
}

void
Controller::on_key_map_select_state(Key key) {
    if (!model_.player_one_selected_map) {
        // If key is d, move respective player's cursor to the right
        if (key == Key::code('d')) {
            model_.move_player_one_cursor(1, model_.state);
        }
        // If key is a, move respective player's cursor to the left
        else if (key == Key::code('a')) {
            model_.move_player_one_cursor(-1, model_.state);
        }
        // If key is x, player selects map at current cursor pos
        else if (key == Key::code('x')) {
            model_.player_one_selects_map();
        }
    }
    else {
        if (key == Key::code('c')) {
            model_.player_one_unselects_map();
        }
    }
    if (!model_.player_two_selected_map) {
        // If key is right arrow, move respective player's cursor to the right
        if (key == Key::right()) {
            model_.move_player_two_cursor(1, model_.state);
        }
        // If key is left arrow, move respective player's cursor to the left
        else if (key == Key::left()) {
            model_.move_player_two_cursor(-1, model_.state);
        }
        // If key is j, player selects map at current cursor pos
        else if (key == Key::code('j')) {
            model_.player_two_selects_map();
        }
    }
    else {
        if (key == Key::code('k')) {
            model_.player_two_unselects_map();
        }
    }
    if (key == Key::code('y') && model_.player_one_selected_map && model_
    .player_two_selected_map) {
        model_.map_number = model_.decide_map();
        model_.select_grid.reset_player_cursors();
        model_.progress_game_state();
    }
}

void
Controller::on_key_after_match_state(Key key)
{
    if (key == Key::code('x') || key == Key::code('j')) {
        model_.reset_game();
    }
}

void
Controller::on_key_down(Key key)
{
    if(model_.state == Game_state::match_state) {
        model_.on_key_down(key);
    }
}

void
Controller::on_key_up(Key key)
{
    if (model_.state == Game_state::match_state) {
        model_.on_key_up(key);
    }
}


void
Controller::on_mouse_down(ge211::Mouse_button mouse_button,
                          ge211::Posn<int> pos)
{
    if (model_.state == Game_state::start_screen_state) {
        model_.progress_game_state();
    }
}

View::Dimensions
Controller::initial_window_dimensions() const
{
    return view_.initial_window_dimensions();
}

std::string
Controller::initial_window_title() const
{
    return view_.initial_window_title();
}
