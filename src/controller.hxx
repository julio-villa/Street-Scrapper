#pragma once

#include "model.hxx"
#include "view.hxx"
#include "game_state.hxx"
#include "select_grid.hxx"

#include <ge211.hxx>

using Key = ge211::events::Key;

class Controller : public ge211::Abstract_game
{
public:
    Controller();

protected:
    void draw(ge211::Sprite_set& set) override;

    void on_key(Key key) override;

    void on_key_down(Key key) override;

    void on_key_up(Key key) override;

    void on_mouse_down(ge211::events::Mouse_button, ge211::Posn<int>) override;

    View::Dimensions initial_window_dimensions() const override;

    std::string initial_window_title() const override;

    void on_frame(double dt) override;

private:
    Model model_;
    Config const& config;
    View view_;

    ge211::audio::Music_track musictrack1 = ge211::Music_track
            ("music/soundtrack0.mp3", Abstract_game::mixer());
    ge211::audio::Music_track musictrack2 = ge211::Music_track
            ("music/soundtrack1.mp3", Abstract_game::mixer());

    void on_key_character_select_state(Key key);

    void on_key_map_select_state(Key key);

    void on_key_after_match_state(Key key);

    void set_soundtrack(ge211::audio::Music_track track);

    bool music_track_one_playing = true;

    bool music_track_two_playing = false;
};
