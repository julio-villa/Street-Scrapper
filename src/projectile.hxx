#pragma once

#include "character.hxx"
#include "config.hxx"


class Projectile
{
friend class Model;

public:
    Projectile(Character character, int width, int dir, Config const& config);

    const ge211::Image_sprite& get_projectile_sprite() const
    {
        return projectile_sprite;
    }

    ge211::Posn<double> get_position() const
    {
        return position;
    }

    bool should_flip() const;


protected:
    void update_sprite(double dt);

private:
    Config config;

    ge211::Image_sprite projectile_sprite;

    ge211::Posn<double> position;

    ge211::Posn<double> destination;

    int direction;

    double speed;
};