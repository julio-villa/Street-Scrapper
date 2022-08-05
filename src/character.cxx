#include "character.hxx"


Character::Character(std::string name, ge211::Posn<int> pos, Config config)
    : name(name),
      icon("sprites/icons/" + name + ".png"),
      position_(pos),
      destination_(pos),
      velocity_(config.walking_speed, config.jump_speed)
{

}

void
Character::move_to_destination(double dt)
{
    ge211::Dims<double> diff = (destination_ - position_).into<double>();

    double magnitude = std::sqrt(diff.width*diff.width + diff.height*diff
            .height);

    ge211::Dims<double> unit = diff / magnitude;

    ge211::Dims<double> offset = velocity_*unit*dt;

    position_.x = position_.x + offset.width;
    position_.y = position_.y + offset.height;
}

void
Character::set_destination(ge211::Posn<double> dest)
{
    destination_ = dest;
}
