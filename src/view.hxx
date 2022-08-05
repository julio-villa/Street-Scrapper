#pragma once

#include "model.hxx"

class View
{
public:
    /// View dimensions will use 'int' values.
    using Dimensions = ge211::Dims<int>;

    /// Constructs a view from a model.
    explicit View(Model const& model);

    void draw(ge211::Sprite_set& set);

    Dimensions initial_window_dimensions() const;

    std::string initial_window_title() const;

    Position board_to_screen(int logical) const;

private:
    Model const& model_;

    Config const& config;

    void render_game_state(ge211::Sprite_set& set);

    ge211::Image_sprite start_screen;

    std::vector<ge211::Image_sprite> icons;

    std::vector<ge211::Image_sprite> backgrounds;

    ge211::Rectangle_sprite grid_sprite;

    ge211::Rectangle_sprite player_one_cursor;

    ge211::Rectangle_sprite player_two_cursor;

    ge211::Rectangle_sprite health_bar_p1;

    ge211::Rectangle_sprite health_bar_p2;

    ge211::Text_sprite player_one_selects_text_sprite;

    ge211::Text_sprite player_two_selects_text_sprite;

    ge211::Text_sprite press_y_continue_text_sprite;

    ge211::Text_sprite player_one_text_sprite;

    ge211::Text_sprite player_two_text_sprite;

    ge211::Text_sprite current_game_state_text_sprite;

    ge211::Text_sprite timer_text_sprite;

    ge211::Text_sprite p1_health_sprite;

    ge211::Text_sprite p2_health_sprite;

    ge211::Text_sprite winner_text_sprite;

    ge211::Text_sprite loser_text_sprite;

    ge211::Text_sprite draw_text_sprite;

    ge211::Text_sprite player_one_after_match_text_sprite;

    ge211::Text_sprite player_two_after_match_text_sprite;

    ge211::Font font;

    void render_character_select_state(ge211::Sprite_set& set);

    void render_map_select_state(ge211::Sprite_set& set);

    void render_match_state(ge211::Sprite_set& set);

    void render_after_match_state(ge211::Sprite_set& set);

    void add_timer_text_sprite(ge211::Sprite_set& set);

    void add_health_bars(ge211::Sprite_set& set);

    void add_player_selections(ge211::Sprite_set& set);

    void add_press_y_continue_text_sprite(ge211::Sprite_set& set);

    void add_cursor_and_text_sprites(ge211::Sprite_set& set);

    void add_player_one_text_sprite(ge211::Sprite_set& set);

    void add_player_two_text_sprite(ge211::Sprite_set& set);

    void add_current_game_state_text_sprite(ge211::Sprite_set& set);

    void add_winner_loser_text_sprites(ge211::Sprite_set& set);

};
