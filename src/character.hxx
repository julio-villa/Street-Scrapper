#pragma once

#include "config.hxx"

#include <ge211.hxx>


class Character
{
public:
     Character(std::string name, ge211::Posn<int> pos, Config config);

     ge211::Posn<double> get_position() const
     {
         return position_;
     }

     std::string get_name() const
     {
         return name;
     }

     double get_destination_x() const
     {
         return destination_.x;
     }

     void move_to_destination(double dt);

     void set_destination(ge211::Posn<double> dest);

     void set_position(ge211::Posn<double> posn)
     {
         position_ = posn;
     }

     const ge211::Image_sprite& get_icon() const {
         return icon;
     }

private:
     std::string name;

     ge211::Image_sprite icon;

     ge211::Posn<double> position_;

     ge211::Posn<double> destination_;

     ge211::Dims<double> velocity_;

};