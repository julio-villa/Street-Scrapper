#include "config.hxx"


Config::Config()
    : icon_dims{63, 100},
      window_dimensions{1024, 768},
      num_backgrounds(4),
      spacing(50),
      map_icon_scale(0.15),
      all_characters{"ryu", "ken", "guile", "chunli"},
      z_values{{"grid", 0}, {"cursor", 1}, {"background", 2}, {"image", 3}},
      all_moves{"walking", "punch", "kick", "jump", "projectile",
                "idle", "crouch", "block"},
      floor_y(400),
      character_jump_height(300),
      jump_speed(500),
      walking_speed(200),
      punch_damage(5),
      kick_damage(10),
      projectile_damage(10),
      projectile_speed(400)

{
    all_moves_frames["walking"] = 5;
    all_moves_frames["punch"] = 3;
    all_moves_frames["kick"] = 3;
    all_moves_frames["jump"] = 7;
    all_moves_frames["projectile"] = 4;
    all_moves_frames["idle"] = 4;
    all_moves_frames["crouch"] = 1;
    all_moves_frames["block"] = 1;
}