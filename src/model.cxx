#include "model.hxx"

Model::Model(Config const& config)
    : config(config),
      select_grid(Select_grid(config)),
      players(),
      timer(100),
      winner(player_num::neither)
{

}

void
Model::progress_game_state()
{
    state = get_next_state(state);
}

void
Model::reset_game()
{
    state = Game_state::character_select_state;
    player_one_selected_character = false;
    player_two_selected_character = false;
    player_one_selected_map = false;
    player_two_selected_map = false;
    winner = player_num::neither;
    timer = 100;
    players = std::vector<Player>();
}

void
Model::move_player_one_cursor(int dir, Game_state state_)
{
    select_grid.move_player_one_cursor(dir, state_);
}

void
Model::move_player_two_cursor(int dir, Game_state state_)
{
    select_grid.move_player_two_cursor(dir, state_);
}

void
Model::on_frame(double dt)
{
    if (state == Game_state::match_state) {
        check_game_end();

        update_projectiles(dt);

        timer -= dt;

        update_players(dt);
    }
}

void
Model::check_game_end()
{
    if (players[0].health <= 0) {
        winner = players[1].num;
        progress_game_state();
    }
    else if (players[1].health <= 0) {
        winner = players[0].num;
        progress_game_state();
    }
    else if (timer <= 0) {
        if (players[0].health > players[1].health) {
            winner = players[0].num;
            progress_game_state();
        }
        else if (players[0].health < players[1].health) {
            winner = players[1].num;
            progress_game_state();
        }
        else {
            winner = player_num::neither;
            progress_game_state();
        }
    }
}

void
Model::update_projectiles(double dt)
{
    for (auto& projectile : players[0].projectiles) {
        if (detect_collision(projectile)) {
            break;
        }
        projectile.update_sprite(dt);
        if (projectile.get_position().x + projectile.projectile_sprite
        .dimensions().width * 2 < 0 || projectile.get_position().x > config
                .window_dimensions.width) {
            projectile = players[0].projectiles.back();
            players[0].projectiles.pop_back();
            break;
        }
        if (detect_projectile_hit(projectile, players[1])) {
            projectile = players[0].projectiles.back();
            players[0].projectiles.pop_back();
            players[1].health -= config.projectile_damage;
            break;
        }
    }

    for (auto& projectile : players[1].projectiles) {
        projectile.update_sprite(dt);
        if (projectile.get_position().x + projectile.projectile_sprite
        .dimensions().width * 2 < 0 || projectile.get_position().x > config
        .window_dimensions.width) {
            projectile = players[1].projectiles.back();
            players[1].projectiles.pop_back();
            break;
        }
        if (detect_projectile_hit(projectile, players[0])) {
            projectile = players[1].projectiles.back();
            players[1].projectiles.pop_back();
            players[0].health -= config.projectile_damage;
            break;
        }
    }
}

void
Model::update_players(double dt)
{
    for (Player& player: players) {
        if (player.character.get_destination_x() <= 0) {
            player.is_walking_left = false;
        }
        else if (player.character.get_destination_x() + player.current_frame
        .dimensions().width * 3 >= config.window_dimensions.width) {
            player.is_walking_right = false;
        }

        std::string current_action;
        if ((player.is_walking_left && player.is_walking_right) ||
            (!player.is_walking_left && !player.is_walking_right)) {
            current_action = "idle";
        }
        else if (player.is_walking_right) {
            current_action = "walking";
            Position curr = player.character.get_position().into<int>();
            player.character.set_destination(Position(curr.x + 10, curr
                    .y).into<double>());
            player.direction = 1;
            player.character.move_to_destination(dt);
        }
        else if (player.is_walking_left) {
            current_action = "walking";
            Position curr = player.character.get_position().into<int>();
            player.character.set_destination(Position(curr.x - 10, curr
                    .y).into<double>());
            player.direction = -1;
            player.character.move_to_destination(dt);
        }
        if (player.is_jumping) {
            current_action = "jump";
            Position curr = player.character.get_position().into<int>();
            player.character.set_destination(Position(curr.x, curr.y - 50)
            .into<double>());
            player.character.move_to_destination(dt);
            if (player.character.get_position().y <= config.floor_y - config
                    .character_jump_height) {
                player.is_jumping = false;
                player.is_landing = true;
            }
        }
        else if (player.is_landing) {
            current_action = "jump";
            Position curr = player.character.get_position().into<int>();
            player.character.set_destination(Position(curr.x, curr.y + 50)
            .into<double>());
            player.character.move_to_destination(dt);
            if (player.character.get_position().y >= config.floor_y) {
                player.is_landing = false;
                player.is_grounded = true;
            }
        }
        if (player.is_blocking) {
            current_action = "block";
        }
        else if (player.is_crouching) {
            current_action = "crouch";
        }
        if (player.is_punching || player.is_kicking || player
                .is_shooting_projectile) {

            player.is_blocking = false;
            player.is_crouching = false;
            if (player.is_punching) {
                current_action = "punch";
                if (player.num == player_num::one) {
                    if (player.hits_player(players[1]) && !players[1].is_hit
                        && !players[1].is_blocking) {
                        players[1].health -= config.punch_damage;
                        players[1].is_hit = true;
                    }
                    if (player.reached_max_frames) {
                        player.reached_max_frames = false;
                        player.is_punching = false;
                        players[1].is_hit = false;
                    }
                }
                else {
                    if (player.hits_player(players[0]) && !players[0].is_hit
                        && !players[0].is_blocking) {
                        players[0].health -= config.punch_damage;
                        players[0].is_hit = true;
                    }
                    if (player.reached_max_frames) {
                        player.reached_max_frames = false;
                        player.is_punching = false;
                        players[0].is_hit = false;
                    }
                }
            }
            else if (player.is_kicking) {
                current_action = "kick";
                if (player.num == player_num::one) {
                    if (player.hits_player(players[1]) && !players[1].is_hit
                        && !players[1].is_blocking) {
                        players[1].health -= config.kick_damage;
                        players[1].is_hit = true;
                    }
                    if (player.reached_max_frames) {
                        player.reached_max_frames = false;
                        player.is_kicking = false;
                        players[1].is_hit = false;
                    }
                }
                else {
                    if (player.hits_player(players[0]) && !players[0]
                            .is_hit && !players[0].is_blocking) {
                        players[0].health -= config.kick_damage;
                        players[0].is_hit = true;
                    }
                    if (player.reached_max_frames) {
                        player.reached_max_frames = false;
                        player.is_kicking = false;
                        players[0].is_hit = false;
                    }
                }
            }
            else if (player.is_shooting_projectile) {
                current_action = "projectile";
                if (!player.shot_projectile) {
                    player.add_projectile();
                }
                player.shot_projectile = true;

                if (player.reached_max_frames)
                {
                    player.reached_max_frames = false;
                    player.is_shooting_projectile = false;
                    player.shot_projectile = false;
                }
            }
        }
        player.set_current_action(current_action);
        player.update_sprite(dt);
    }
}

void
Model::add_players(Character character)
{
    if (players.size() == 0){
        Player new_player = Player(character, player_num::one, config);
        players.push_back(new_player);
    }
    else {
        Player new_player = Player(character, player_num::two, config);
        new_player.character.set_position(new_player.character.get_position()
        .left_by(new_player.current_frame.dimensions().width * 3));
        players.push_back(new_player);
    }
}

int
Model::decide_map() const
{
    if (get_player_one_cursor_pos() == get_player_two_cursor_pos()) {
        return get_player_one_cursor_pos();
    }
    else {
        int random = rand() % 2;
        if (random == 0) {
            return get_player_one_cursor_pos();
        }
        else {
            return get_player_two_cursor_pos();
        }
    }
}

bool
Model::detect_projectile_hit(Projectile projectile, Player& other) const
{
    if ((projectile.get_position().x + projectile.projectile_sprite
    .dimensions().width * 2 < other.character.get_position().x || other
    .character.get_position().x + other.current_frame.dimensions().width
    < projectile.get_position().x) || (projectile.get_position().y +
    projectile.projectile_sprite.dimensions().height * 2 < other.character
    .get_position().y || other.character.get_position().y + other
    .current_frame.dimensions().height < projectile.get_position().y)) {
        return false;
    }
    else {
        return true;
    }
}

bool
Model::detect_collision(Projectile& projectile1)
{
    for(Projectile& projectile2 : players[1].projectiles) {
        if ((projectile1.get_position().x + projectile1.projectile_sprite
        .dimensions().width * 2 < projectile2.get_position().x ||
        projectile2
        .get_position().x + projectile2.projectile_sprite.dimensions()
        .width * 2 < projectile1.get_position().x) || (projectile1
        .get_position().y + projectile1.projectile_sprite.dimensions()
        .height * 2 < projectile2.get_position().y || projectile2
        .get_position().y + projectile2.projectile_sprite.dimensions()
        .height * 2 < projectile1.get_position().y)) {
            continue;
        }
        else {
            projectile2 = players[1].projectiles.back();
            players[1].projectiles.pop_back();

            projectile1 = players[0].projectiles.back();
            players[0].projectiles.pop_back();

            return true;
        }
    }
    return false;
}


void
Model::on_key_down(ge211::Key key)
{
    if (key == Key::code('d')) {
        players[0].is_walking_right = true;
    }
    else if (key == Key::code('a')) {
        players[0].is_walking_left = true;
    }
    else if (key == Key::code('w') && players[0].is_grounded) {
        players[0].is_jumping = true;
        players[0].is_grounded = false;

    }
    else if (key == Key::code('s') && players[0].is_grounded) {
        players[0].is_crouching = true;
    }
    else if (key == Key::code('x')) {
        players[0].is_punching = true;
    }
    else if (key == Key::code('c')) {
        players[0].is_kicking = true;
    }
    else if (key == Key::code('v')) {
        players[0].is_shooting_projectile = true;
    }
    else if (key == Key::shift()) {
        players[0].is_blocking = true;
    }
    else if (key == Key::up() && players[1].is_grounded) {
        players[1].is_jumping = true;
        players[1].is_grounded = false;
    }

    else if (key == Key::right()) {
        players[1].is_walking_right = true;
    }
    else if (key == Key::left()) {
        players[1].is_walking_left = true;
    }
    else if (key == Key::down() && players[1].is_grounded) {
        players[1].is_crouching = true;
    }
    else if (key == Key::code('j')) {
        players[1].is_punching = true;
    }
    else if (key == Key::code('k')) {
        players[1].is_kicking = true;
    }
    else if (key == Key::code('l')) {
        players[1].is_shooting_projectile = true;
    }
    else if (key == Key::code(' ')) {
        players[1].is_blocking = true;
    }
}

void
Model::on_key_up(ge211::Key key)
{
    if (key == Key::code('d')) {
        players[0].is_walking_right = false;
    }
    else if (key == Key::code('a')) {
        players[0].is_walking_left = false;
    }
    else if (key == Key::code('s')) {
        players[0].is_crouching = false;
    }
    else if (key == Key::shift()) {
        players[0].is_blocking = false;
    }
    else if (key == Key::right()){
        players[1].is_walking_right = false;
    }
    else if (key == Key::left()){
        players[1].is_walking_left = false;
    }
    else if (key == Key::down()) {
        players[1].is_crouching = false;
    }
    else if (key == Key::code(' ')) {
        players[1].is_blocking = false;
    }
}