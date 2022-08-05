#include "player.hxx"


Player::Player(Character charac, player_num num, Config const& config)
    : config(config),
      character(charac),
      num(num),
      health(100),
      current_action(charac, "idle", config),
      current_frame(current_action.get_frames()[0]),
      current_duration(0.0),
      current_index(0),
      max_index(config.all_moves_frames.at(current_action.get_name()))
{
    for (unsigned int i = 0; i < config.all_moves.size(); i++) {
        moves.insert({config.all_moves[i], Action(charac, config
        .all_moves[i], config)});
    }
    if (num == player_num::one) {
        direction = 1;
    }
    else {
        direction = -1;
    }
}

void
Player::set_current_action(std::string action) {
    if (action != current_action.get_name()) {
        current_action = Action(character, action, config);
        current_index = 0;
        current_frame = current_action.get_frames()[0];
        max_index = config.all_moves_frames.at(current_action.get_name());
    }
}

void
Player::update_sprite(double dt)
{
    double update_rate = 0.5 / current_action.get_frames().size();

    current_duration += dt;
    if (current_duration > update_rate) {
        current_duration = 0.0;
        current_index ++;
        if (current_index >= max_index) {
            reached_max_frames = true;
            current_index = 0;
        }
        else {
            reached_max_frames = false;
        }
        current_frame = current_action.get_frames()[current_index];
    }
}

void
Player::add_projectile()
{
    projectiles.push_back(Projectile(character, current_frame.dimensions()
    .width, direction, config));
}

ge211::Rect<int>
Player::get_arm_collision_box() const
{
    ge211::Posn<int> pos = {0,0};
    pos.y = character.get_position().y + 15 * 3;
    if (direction == 1) {
        pos.x = character.get_position().x + 30 * 3;
    }
    else {
        pos.x = character.get_position().x - 30 * 3;
    }
    return ge211::Rect<int>(pos.x, pos.y, 23 * 3, 18 * 3);
}

bool
Player::hits_player(Player& other) const {
    ge211::Rect<int> hitbox = get_arm_collision_box();
    if (direction == 1) {
        if ((hitbox.top_right().x < other.character.get_position().x || other
        .character.get_position().x + other.current_frame.dimensions().width
                < hitbox.top_left().x) || (hitbox.bottom_right().y < other
                .character.get_position().y || other.character.get_position()
                .y + other.current_frame.dimensions().height < hitbox
                .top_left().y)) {
            return false;
        }
        else {
            return true;
        }
    }
    else {
        if ((other.character.get_position().x + other.current_frame
        .dimensions().width < hitbox.top_left().x || hitbox.top_right().x <
        other.character.get_position().x) || (hitbox.bottom_right().y <
        other.character.get_position().y || other.character.get_position().y
        + other.current_frame.dimensions().height < hitbox.top_left().y)) {
            return false;
        }
        else {
            return true;
        }
    }
}


bool
Player::should_flip() const {
    if (direction == 1) {
        return false;
    }
    else {
        return true;
    }
}
