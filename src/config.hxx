#pragma once

#include <ge211.hxx>
#include <unordered_map>

struct Config {
    // Constructs an instance with the default parameters.
    Config();

    ge211::Dims<int> icon_dims;

    ge211::Dims<int> window_dimensions;

    // Number of backgrounds:
    int num_backgrounds;

    int spacing;

    float map_icon_scale;

    std::vector<const char*> all_characters;

    std::unordered_map<std::string, int> z_values;

    std::vector<std::string> all_moves;

    std::unordered_map<std::string, int> all_moves_frames;

    int floor_y;

    int character_jump_height;

    double jump_speed;

    double walking_speed;

    int punch_damage;

    int kick_damage;

    int projectile_damage;

    double projectile_speed;
};