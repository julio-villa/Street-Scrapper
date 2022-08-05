#pragma once

#include "character.hxx"
#include "action.hxx"
#include "config.hxx"
#include "projectile.hxx"

#include <unordered_map>

enum class player_num {
    one,
    two,
    neither
};

class Player
{

friend class Controller;
friend class Model;
#ifdef CS211_TESTING
    friend struct Test_access;
#endif

public:

    Player(Character character, player_num num, Config const& config);

    int get_health() const
    {
        return health;
    }

    const Character& get_character() const
    {
        return character;
    }

    const Action& get_current_action() const
    {
        return current_action;
    }

    const ge211::Image_sprite& get_current_frame() const
    {
        return current_frame;
    }

    void set_current_action(std::string action);

    ge211::Rect<int> get_arm_collision_box() const;

    bool hits_player(Player& other) const;

    bool should_flip() const;

    const std::vector<Projectile>& get_projectiles() const
    {
        return projectiles;
    }



protected:
    void update_sprite(double dt);

    void add_projectile();

    bool is_walking_right = false;

    bool is_walking_left = false;

    bool is_jumping = false;

    bool is_grounded = true;

    bool is_landing = false;

    bool is_punching = false;

    bool is_kicking = false;

    bool is_blocking = false;

    bool is_crouching = false;

    bool is_hit = false;

    bool is_shooting_projectile = false;

    bool shot_projectile = false;

    bool reached_max_frames = false;

private:
    Config const& config;

    Character character;

    player_num num;

    int health;

    std::unordered_map<std::string, Action> moves;

    std::vector<Projectile> projectiles;

    Action current_action;

    ge211::Image_sprite current_frame;

    double current_duration;

    int current_index;

    int max_index;

    int direction;
};
