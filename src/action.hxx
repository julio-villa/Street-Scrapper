#pragma once

#include "character.hxx"
#include "config.hxx"


class Action{

public:
    Action(Character charac, std::string action, Config const& config);

    std::string get_name() const {
        return name;
    }

    const std::vector<ge211::sprites::Image_sprite>& get_frames() const {
        return frames;
    }

private:
    std::string name;

    std::vector<ge211::sprites::Image_sprite> frames;
};

