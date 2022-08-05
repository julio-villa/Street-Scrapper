#include "projectile.hxx"

Projectile::Projectile(Character character, int width, int dir, Config const&
config)
    : config(config),
      projectile_sprite("sprites/characters/" + character.get_name() +
      "/projectile.png"),
      position(character.get_position().down_by(50)),
      destination(character.get_position()),
      direction(dir),
      speed(config.projectile_speed)
{
    if (direction == 1) {
        position.x += width;
    }
}

void
Projectile::update_sprite(double dt)
{
    destination.x = position.x + direction * speed;

    double diff = destination.x - position.x;

    double magnitude = std::abs(diff);

    double unit = diff / magnitude;

    double offset = speed*unit*dt;

    position.x = position.x + offset;
}

bool
Projectile::should_flip() const
{
    if (direction == 1) {
        return false;
    }
    else {
        return true;
    }
}

