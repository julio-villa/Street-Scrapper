#include "action.hxx"

Action::Action(Character character, std::string action, Config const& config)
    : name(action)
{
    std::string filename = "sprites/characters/";
    filename += character.get_name();
    filename += "/";
    filename += action;
    filename += "/";
    filename += action;
    for (int i = 0; i < config.all_moves_frames.at(action); i++) {
        frames.push_back(ge211::Image_sprite(filename + std::to_string(i)
        + ".png"));
    }
}

