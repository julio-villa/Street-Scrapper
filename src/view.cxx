#include "view.hxx"
#include <iostream>

using namespace ge211;

static Color purple = {88, 0, 88};

View::View(Model const& model)
        : model_(model),
          config(model_.get_config()),
          start_screen("sprites/backgrounds/start-screen.png"),
          grid_sprite(config.icon_dims),
          player_one_cursor({config.icon_dims.width, config.icon_dims
          .height}, Color::medium_blue()),
          player_two_cursor({config.icon_dims.width, config.icon_dims
          .height}, Color::medium_red()),
          health_bar_p1({20, (config.icon_dims.width / 2) - 7 },
                        Color::medium_blue()),
          health_bar_p2({20, (config.icon_dims.width / 2) - 7 },
                        Color::medium_red()),
          font("sans.ttf", config.icon_dims.width / 2)

{
    for (unsigned int i = 0; i < config.all_characters.size(); i++) {
        std::string filename = "sprites/icons/";
        filename += config.all_characters[i];
        filename += ".png";
        icons.push_back(Image_sprite(filename));
    }

    for (int i = 0; i < config.num_backgrounds; i++) {
        std::string filename = "sprites/backgrounds/bg";
        filename += std::to_string(i);
        filename += ".png";
        backgrounds.push_back(Image_sprite(filename));
    }
}

void
View::draw(Sprite_set& set)
{
    render_game_state(set);
}

void
View::render_game_state(Sprite_set& set)
{
    if (model_.get_game_state() == Game_state::start_screen_state) {
        set.add_sprite(start_screen, {0,0});
    }
    else if (model_.get_game_state() == Game_state::character_select_state) {
        render_character_select_state(set);
    }
    else if (model_.get_game_state() == Game_state::map_select_state) {
        render_map_select_state(set);
    }
    else if (model_.get_game_state() == Game_state::match_state) {
        render_match_state(set);
    }
    else if (model_.get_game_state() == Game_state::after_match_state){
        render_after_match_state(set);
    }
}

View::Dimensions
View::initial_window_dimensions() const
{
    return config.window_dimensions;
}

std::string
View::initial_window_title() const
{
    return "Street Scrapper";
}


void
View::render_character_select_state(Sprite_set& set)
{
    // Multiplying width and spacing by 1.5 because you have one whole
    // spacing and half of another spacing
    Position current = Position(config.window_dimensions.width / 2 - config
            .icon_dims.width * 2 - config.spacing * 1.5, config
            .window_dimensions.height / 4 - config.icon_dims.height / 2);

    for (Image_sprite& sprite : icons) {
        set.add_sprite(sprite, current, config.z_values.at("image"));
        set.add_sprite(grid_sprite, current, config.z_values.at("grid"));
        current.x += sprite.dimensions().width + config.spacing;
    }

    add_player_selections(set);

    add_cursor_and_text_sprites(set);
}

void
View::render_map_select_state(Sprite_set& set)
{
    int background_icon_width = backgrounds[0].dimensions().width * config
            .map_icon_scale;
    Position current = Position(config.window_dimensions.width / 2 -
            background_icon_width * 2 - config.spacing * 1.5, config
            .window_dimensions.height / 4 - config.icon_dims.height / 2);
    for (Image_sprite& background : backgrounds) {
        set.add_sprite(background, current, config.z_values.at("background"),
                       Transform::scale(config.map_icon_scale));
        current.x += background_icon_width + config.spacing;
    }
    add_player_selections(set);

    add_cursor_and_text_sprites(set);
}

void
View::render_match_state(Sprite_set& set)
{
    add_timer_text_sprite(set);
    add_health_bars(set);

    set.add_sprite(backgrounds[model_.get_map_number()], {0,0});

    Transform player_one_transform = Transform{}.set_flip_h(model_
            .get_players()[0].should_flip())
            .set_scale(3);

    Transform player_two_transform = Transform{}.set_flip_h(model_
            .get_players()[1].should_flip())
            .set_scale(3);

    set.add_sprite(model_.get_players()[0].get_current_frame(), model_
            .get_players()[0].get_character().get_position().into<int>(), config
                           .z_values.at("image"), player_one_transform);

    for (auto& projectile : model_.get_players()[0].get_projectiles()) {
        Transform player_one_projectile_transform = Transform{}.set_flip_h
                (projectile.should_flip()).set_scale(2);
        set.add_sprite(projectile.get_projectile_sprite(), projectile
        .get_position().into<int>(), config.z_values.at("image"),
        player_one_projectile_transform);
    }

    set.add_sprite(model_.get_players()[1].get_current_frame(), model_
    .get_players()[1].get_character().get_position().into<int>(), config
    .z_values.at("image"), player_two_transform);

    for (auto& projectile : model_.get_players()[1].get_projectiles()) {
        Transform player_two_projectile_transform = Transform{}.set_flip_h
                (projectile.should_flip()).set_scale(2);
        set.add_sprite(projectile.get_projectile_sprite(), projectile
        .get_position().into<int>(), config.z_values.at("image"),
        player_two_projectile_transform);
    }
}

void
View::render_after_match_state(Sprite_set& set)
{
    set.add_sprite(model_.get_players()[0].get_character().get_icon(),
                   Position(config.window_dimensions.width / 2 - config
                   .icon_dims.width * 2 - config.spacing, config
                   .window_dimensions.height / 4), config.z_values.at
                   ("image"), Transform::scale(2));

    set.add_sprite(model_.get_players()[1].get_character().get_icon(),
                   Position(config.window_dimensions.width / 2 + config
                   .spacing, config.window_dimensions.height / 4), config
                   .z_values.at("image"), Transform::scale(2));

    add_winner_loser_text_sprites(set);
}

void
View::add_timer_text_sprite(Sprite_set& set)
{
    Text_sprite::Builder timer_text_builder(font);
    timer_text_builder.color(Color::medium_magenta());
    timer_text_builder << round(model_.get_timer());
    timer_text_sprite.reconfigure(timer_text_builder);
    set.add_sprite(timer_text_sprite, Position(config.window_dimensions.width
    / 2 - timer_text_sprite.dimensions().width / 2, 10), config.z_values.at
    ("image"));
}

void
View::add_health_bars(Sprite_set& set)
{
    Text_sprite::Builder p1_(font);
    Text_sprite::Builder p2_(font);
    p1_.color(Color::medium_blue());
    p2_.color(Color::medium_red());
    p1_ << "1P";
    p2_ << "2P";
    p1_health_sprite.reconfigure(p1_);
    p2_health_sprite.reconfigure(p2_);

    Position pos1 = {10, 25};
    Position pos2 = {config.window_dimensions.width - 10 - health_bar_p1
            .dimensions().width,25};

    set.add_sprite(p1_health_sprite,{pos1.x, pos1.y + health_bar_p1
    .dimensions().height + 10}, config.z_values.at("image"));
    set.add_sprite(p2_health_sprite, {pos2.x - p2_health_sprite.dimensions()
    .width / 2, pos2.y + health_bar_p2.dimensions().height + 10}, config
    .z_values.at("image"));

    for (int i = 0; i < model_.get_players()[0].get_health() / 5; i++) {
        set.add_sprite(health_bar_p1, pos1, config.z_values.at("image"));
        pos1.x += health_bar_p1.dimensions().width;
    }
    for (int i = 0; i < model_.get_players()[1].get_health() / 5; i++) {
        set.add_sprite(health_bar_p2, pos2, config.z_values.at("image"));
        pos2.x -= health_bar_p2.dimensions().width;
    }
}

void
View::add_player_selections(Sprite_set& set)
{
    if (model_.get_game_state() == Game_state::character_select_state) {
        if (model_.get_player_one_selected_character()) {
            set.add_sprite(icons[model_.get_player_one_cursor_pos()], Position
                    (config.window_dimensions.width / 2 - config.spacing -
                     config.icon_dims.width * 2, config.window_dimensions
                     .height / 2), config.z_values.at("image"),
                     Transform::scale(2));
            Text_sprite::Builder player_one_selects_text_builder(font);
            player_one_selects_text_builder.color(Color::medium_blue());
            player_one_selects_text_builder << "1P";
            player_one_selects_text_sprite.reconfigure
            (player_one_selects_text_builder);
            set.add_sprite(player_one_text_sprite, Position(config
            .window_dimensions.width / 2 - config.spacing - config.icon_dims
            .width - player_one_text_sprite.dimensions().width / 2,config
            .window_dimensions.height / 2 + config.icon_dims.height * 2));
        }
        if (model_.get_player_two_selected_character()) {
            set.add_sprite(icons[model_.get_player_two_cursor_pos()],
                           Position(config.window_dimensions.width / 2 +
                           config.spacing, config.window_dimensions.height /
                           2), config.z_values.at("image"), Transform::scale
                           (2));
            Text_sprite::Builder player_two_selects_text_builder(font);
            player_two_selects_text_builder.color(Color::medium_red());
            player_two_selects_text_builder << "2P";
            player_two_selects_text_sprite.reconfigure
            (player_two_selects_text_builder);
            set.add_sprite(player_two_text_sprite, Position(config
            .window_dimensions.width / 2 + config.spacing + config.icon_dims
            .width - player_one_text_sprite.dimensions().width / 2, config
            .window_dimensions.height / 2 + config.icon_dims.height * 2));
        }
        if (model_.get_player_one_selected_character() && model_
                .get_player_two_selected_character()) {
            add_press_y_continue_text_sprite(set);
        }
    }
    else if (model_.get_game_state() == Game_state::map_select_state) {
        if (model_.get_player_one_selected_map()) {
            set.add_sprite(backgrounds[model_.get_player_one_cursor_pos()],
                           Position(config.window_dimensions.width / 2 -
                           config.spacing - backgrounds[model_
                           .get_player_one_cursor_pos()].dimensions().width *
                           config.map_icon_scale * 2, config
                           .window_dimensions.height / 2), config.z_values.at
                           ("background"), Transform::scale(config
                           .map_icon_scale * 2));
            Text_sprite::Builder player_one_selects_text_builder(font);
            player_one_selects_text_builder.color(Color::medium_blue());
            player_one_selects_text_builder << "1P";
            player_one_selects_text_sprite.reconfigure
            (player_one_selects_text_builder);
            set.add_sprite(player_one_text_sprite, Position(config
            .window_dimensions.width / 2 - config.spacing -
            backgrounds[model_.get_player_one_cursor_pos()].dimensions()
            .width * config.map_icon_scale - player_one_text_sprite
            .dimensions().width / 2,config.window_dimensions.height / 2 +
            backgrounds[model_.get_player_one_cursor_pos()].dimensions()
            .height * config.map_icon_scale * 2));
        }
        if (model_.get_player_two_selected_map()) {
            set.add_sprite(backgrounds[model_.get_player_two_cursor_pos()],
                           Position(config.window_dimensions.width / 2 +
                           config.spacing, config.window_dimensions.height /
                           2),config.z_values.at("background"),
                           Transform::scale(config.map_icon_scale * 2));
            Text_sprite::Builder player_two_selects_text_builder(font);
            player_two_selects_text_builder.color(Color::medium_red());
            player_two_selects_text_builder << "2P";
            player_two_selects_text_sprite.reconfigure
            (player_two_selects_text_builder);
            set.add_sprite(player_two_text_sprite, Position(config
            .window_dimensions.width / 2 + config.spacing +
            backgrounds[model_.get_player_two_cursor_pos()].dimensions()
            .width * config.map_icon_scale - player_two_text_sprite
            .dimensions().width / 2,config.window_dimensions.height / 2 +
            backgrounds[model_.get_player_two_cursor_pos()].dimensions()
            .height * config.map_icon_scale * 2));
        }
        if (model_.get_player_one_selected_map() && model_
        .get_player_two_selected_map()) {
            add_press_y_continue_text_sprite(set);
        }
    }
}

void
View::add_press_y_continue_text_sprite(Sprite_set& set)
{
    Text_sprite::Builder press_y_continue_text_builder
            (font);
    press_y_continue_text_builder << "PRESS Y TO CONTINUE";
    press_y_continue_text_sprite.reconfigure(
            press_y_continue_text_builder);
    set.add_sprite(press_y_continue_text_sprite, Position(config
    .window_dimensions.width / 2 - press_y_continue_text_sprite.dimensions()
    .width / 2,config.window_dimensions.height / 2 -
    press_y_continue_text_sprite.dimensions().height));
}


void
View::add_cursor_and_text_sprites(Sprite_set& set)
{
    set.add_sprite(player_one_cursor, board_to_screen(model_
    .get_player_one_cursor_pos()), config.z_values.at("cursor"));
    set.add_sprite(player_two_cursor, board_to_screen(model_
    .get_player_two_cursor_pos()), config.z_values.at("cursor"));

    add_player_one_text_sprite(set);
    add_player_two_text_sprite(set);

    add_current_game_state_text_sprite(set);
}

void
View::add_player_one_text_sprite(Sprite_set& set)
{
    Text_sprite::Builder player_one_text_builder(font);
    player_one_text_builder.color(Color::medium_blue());
    player_one_text_builder << "1P";
    player_one_text_sprite.reconfigure(player_one_text_builder);
    Position physical = board_to_screen(model_.get_player_one_cursor_pos());
    physical.y -= player_one_text_sprite.dimensions().height;
    if (model_.get_player_one_cursor_pos() != model_
    .get_player_two_cursor_pos()) {
        player_one_cursor.recolor(Color::medium_blue());
        if (model_.get_game_state() == Game_state::character_select_state) {
            physical.x += config.icon_dims.width / 2 - player_one_text_sprite
                    .dimensions().width / 2;
        }
        else if (model_.get_game_state() == Game_state::map_select_state) {
            physical.x += backgrounds[model_.get_player_one_cursor_pos()]
                    .dimensions().width * config.map_icon_scale / 2 -
                    player_one_text_sprite.dimensions().width / 2;
        }
    }
    else {
        player_one_cursor.recolor(purple);
        if (model_.get_game_state() == Game_state::character_select_state) {
            physical.x = (physical.x + config.icon_dims.width / 2) -
                         player_one_text_sprite.dimensions().width;
        }
        else if (model_.get_game_state() == Game_state::map_select_state) {
            physical.x = (physical.x + backgrounds[
                    model_.get_player_one_cursor_pos()].dimensions().width *
                            config.map_icon_scale / 2) -
                                    player_one_text_sprite.dimensions().width
                                    * 1.5;
        }
    }
    set.add_sprite(player_one_text_sprite, physical);
}

void
View::add_player_two_text_sprite(Sprite_set& set)
{
    Text_sprite::Builder player_two_text_builder(font);
    player_two_text_builder.color(Color::medium_red());
    player_two_text_builder << "2P";
    player_two_text_sprite.reconfigure(player_two_text_builder);
    Position physical = board_to_screen(model_.get_player_two_cursor_pos());
    physical.y -= player_two_text_sprite.dimensions().height;
    if (model_.get_player_one_cursor_pos() != model_
    .get_player_two_cursor_pos()) {
        player_two_cursor.recolor(Color::medium_red());
        if (model_.get_game_state() == Game_state::character_select_state) {
            physical.x += config.icon_dims.width / 2 - player_two_text_sprite
                    .dimensions().width / 2;
        }
        else if (model_.get_game_state() == Game_state::map_select_state) {
            physical.x += backgrounds[model_.get_player_two_cursor_pos()]
                    .dimensions().width * config.map_icon_scale / 2 -
                    player_two_text_sprite.dimensions().width / 2;
        }
    }
    else {
        player_two_cursor.recolor(purple);
        if (model_.get_game_state() == Game_state::character_select_state) {
            physical.x = (physical.x + config.icon_dims.width / 2) +
                         player_one_text_sprite.dimensions().width * 0.25;
        }
        else if (model_.get_game_state() == Game_state::map_select_state) {
            physical.x = (physical.x + backgrounds[
                    model_.get_player_two_cursor_pos()].dimensions().width *
                            config.map_icon_scale / 2) +
                                    player_two_text_sprite.dimensions().width
                                    * 0.5;
        }
    }
    set.add_sprite(player_two_text_sprite, physical);
}

void
View::add_current_game_state_text_sprite(Sprite_set& set)
{
    Text_sprite::Builder current_game_state_text_builder(font);
    current_game_state_text_builder.color(Color::white());
    if (model_.get_game_state() == Game_state::character_select_state) {
        current_game_state_text_builder << "CHARACTER SELECT";
    }
    else if (model_.get_game_state() == Game_state::map_select_state) {
        current_game_state_text_builder << "MAP SELECT";
    }
    current_game_state_text_sprite.reconfigure(current_game_state_text_builder);
    Position physical = Position(config.window_dimensions.width / 2 -
            current_game_state_text_sprite.dimensions().width / 2,10);
    set.add_sprite(current_game_state_text_sprite, physical);
}

void
View::add_winner_loser_text_sprites(Sprite_set& set)
{
    Text_sprite::Builder winner_text_builder(font);
    winner_text_builder.color(Color::medium_green());
    winner_text_builder << "WINNER";
    winner_text_sprite.reconfigure(winner_text_builder);

    Text_sprite::Builder loser_text_builder(font);
    loser_text_builder.color(Color::medium_red());
    loser_text_builder << "LOSER";
    loser_text_sprite.reconfigure(loser_text_builder);

    Text_sprite::Builder draw_text_builder(font);
    draw_text_builder.color(Color::white());
    draw_text_builder << "DRAW";
    draw_text_sprite.reconfigure(draw_text_builder);

    Text_sprite::Builder player_one_after_match_text_builder(font);
    player_one_after_match_text_builder.color(Color::medium_blue());
    player_one_after_match_text_builder << "1P";
    player_one_after_match_text_sprite.reconfigure
    (player_one_after_match_text_builder);

    Text_sprite::Builder player_two_after_match_text_builder(font);
    player_two_after_match_text_builder.color(Color::medium_red());
    player_two_after_match_text_builder << "2P";
    player_two_after_match_text_sprite.reconfigure
    (player_two_after_match_text_builder);

    if (model_.get_winner() == player_num::one) {
        set.add_sprite(winner_text_sprite, Position(config.window_dimensions
        .width / 2 - config.icon_dims.width - config.spacing -
        winner_text_sprite.dimensions().width / 2, config.window_dimensions
        .height / 4 + config.icon_dims.height * 2));
        set.add_sprite(loser_text_sprite, Position(config.window_dimensions
        .width / 2 + config.spacing + config.icon_dims.width -
        loser_text_sprite.dimensions().width / 2, config.window_dimensions
        .height / 4 + config.icon_dims.height * 2));
    }
    else if (model_.get_winner() == player_num::two) {
        set.add_sprite(winner_text_sprite, Position(config.window_dimensions
        .width / 2 + config.icon_dims.width + config.spacing -
        winner_text_sprite.dimensions().width / 2, config.window_dimensions
        .height / 4 + config.icon_dims.height * 2));
        set.add_sprite(loser_text_sprite, Position(config.window_dimensions
        .width / 2 - config.icon_dims.width - config.spacing -
        loser_text_sprite.dimensions().width / 2, config.window_dimensions
        .height / 4 + config.icon_dims.height * 2));
    }
    else {
        set.add_sprite(draw_text_sprite, Position(config.window_dimensions
        .width / 2 - config.icon_dims.width - config.spacing -
        draw_text_sprite.dimensions().width / 2, config.window_dimensions
        .height / 4 + config.icon_dims.height * 2));
        set.add_sprite(draw_text_sprite, Position(config.window_dimensions
        .width / 2 + config.spacing + config.icon_dims.width -
        draw_text_sprite.dimensions().width / 2, config.window_dimensions
        .height / 4 + config.icon_dims.height * 2));
    }

    set.add_sprite(player_one_after_match_text_sprite, Position(config
    .window_dimensions.width / 2 - config.icon_dims.width - config.spacing -
    player_one_after_match_text_sprite.dimensions().width / 2, config
    .window_dimensions.height / 4 - player_one_after_match_text_sprite
    .dimensions().height));

    set.add_sprite(player_two_after_match_text_sprite, Position(config
    .window_dimensions.width / 2 + config.spacing + config.icon_dims.width -
    player_two_after_match_text_sprite.dimensions().width / 2, config
    .window_dimensions.height / 4 - player_two_after_match_text_sprite
    .dimensions().height));
}

Position
View::board_to_screen(int logical) const
{
    if (model_.get_game_state() == Game_state::character_select_state) {
        Position physical = Position(config.window_dimensions.width / 2 - config
                .icon_dims
                .width * 2 - config.spacing * 1.5 + (config.icon_dims.width +
                config.spacing) * logical, config.window_dimensions.height /
                4 - config.icon_dims
                .height / 2);

        return physical;
    }
    else if (model_.get_game_state() == Game_state::map_select_state) {
        int background_icon_width = backgrounds[0].dimensions().width *
                config.map_icon_scale;
        Position physical = Position(config.window_dimensions.width / 2 -
                background_icon_width * 2 - config.spacing * 1.5 +
                (background_icon_width + config.spacing) * logical,
                config.window_dimensions.height / 4 - config.icon_dims.height /
                2);

        return physical;
    }
    else {
        return {0, 0};
    }
}
