#include "model.hxx"
#include <catch.hxx>


Config const config;

struct Test_access
{
    Model& model;

    explicit Test_access(Model&);

    void progress_game_state();

    std::vector<Player>& get_players_();

    void set_bool(Player& player, std::string boolean1, bool boolean2);

    bool& get_bool(Player& player, std::string boolean);

    int get_health(Player& player);

    void set_health(Player& player, int health);

    void on_frame(double dt);

    void check_game_end();

    void player_one_selects_character();

    void player_two_selects_character();

    bool& get_selected_character_bool(std::string str);

    void player_one_selects_map();

    void player_two_selects_map();

    bool& get_selected_map_bool(std::string str);

    void reset_game();
};


TEST_CASE("game run-through from start screen to after match screen")
{
    Model model(config);
    Test_access access(model);

    // Advance from start screen
    access.progress_game_state();

    // Both players select a character based on their cursor position, and
    // game advances
    access.player_one_selects_character();
    access.player_two_selects_character();

    // If both players have selected characters, advance game state
    if(access.get_selected_character_bool("player_one_selected_character") &&
    access.get_selected_character_bool("player_two_selected_character")){
        access.progress_game_state();
    }

    //Check that the game state has properly progressed to the map select state
    CHECK(model.get_game_state() == Game_state::map_select_state);

    //Both characters select a map
    access.player_one_selects_map();
    access.player_two_selects_map();

    // If both players have selected a map, advance game state
    if (access.get_selected_map_bool("player_one_selected_map") && access
    .get_selected_character_bool("player_two_selected_map")) {
        access.progress_game_state();
    }
    // Check that the game state has properly progressed to the map select state
    CHECK(model.get_game_state() == Game_state::match_state);

    //Manually create characters and set health to 0
    Character character1 = Character("ryu", {500, 500}, config);
    Character character2 = Character("ken", {750, 500}, config);
    model.add_players(character1);
    model.add_players(character2);
    access.set_health(access.get_players_()[0], 0);

    //Check that game is over once a player's health has reached 0
    access.check_game_end();

    //Check that the game state has properly progressed to after match state
    // automatically
    CHECK(access.model.get_game_state() == Game_state::after_match_state);
    CHECK(access.model.get_winner() == player_num::two);
}

TEST_CASE("projectiles collide and destroy each other, and get destroyed if "
          "they go off screen")
{
    Model model(config);
    Test_access access(model);

    // Advance from start screen to match state
    access.progress_game_state();
    access.progress_game_state();
    access.progress_game_state();


    Character character1 = Character("ryu", {100, 500}, config);
    Character character2 = Character("ken", {800, 500}, config);
    model.add_players(character1);
    model.add_players(character2);

    access.set_bool(access.get_players_()[0], "is_shooting_projectile", true);
    access.set_bool(access.get_players_()[1], "is_shooting_projectile", true);

    access.on_frame(0);
    CHECK(access.get_players_()[0].get_projectiles().size() == 1);
    CHECK(access.get_players_()[1].get_projectiles().size() == 1);

    // 1/60th of a second 0.16666667 == 1/60
    double dt = 0.01666667;

    for(int i = 0; i < 120; i++) {
        access.on_frame(dt);
    }

    // Size of projectile vectors should decrease to 0 since they collide
    CHECK(access.get_players_()[0].get_projectiles().size() == 0);
    CHECK(access.get_players_()[1].get_projectiles().size() == 0);

    // Players' health should stay the same because projectile collided
    // between them
    CHECK(access.get_health(access.get_players_()[0]) == 100);
    CHECK(access.get_health(access.get_players_()[1]) == 100);

    // Remove player 2
    access.get_players_().pop_back();
    access.set_bool(access.get_players_()[0], "is_shooting_projectile", true);

    access.on_frame(0);
    CHECK(access.get_players_()[0].get_projectiles().size() == 1);

    dt = 1;
    // 400 pixels/sec projectile speed -> in 3 seconds, then position of
    // projectile should be 0 + 1200 and therefore pass 1024
    for (int i = 0; i < 3; i++) {
        access.on_frame(dt);
    }

    // Verify that projectile was removed once it reached screen boundary
    CHECK(access.get_players_()[0].get_projectiles().size() == 0);

}

TEST_CASE("testing character movement")
{
    Model model(config);
    Test_access access(model);

    // Advance from start screen to map select screen
    access.progress_game_state();
    access.progress_game_state();
    access.progress_game_state();

    // Add player to game
    Character character1 = Character("ryu", {0, 400}, config);
    model.add_players(character1);

    Character character2 = Character("ryu", {0, 400}, config);
    model.add_players(character2);

    //Set the character to "walking right"
    access.set_bool(access.get_players_()[0], "is_walking_right", true);

    double dt = 1;
    access.on_frame(dt);
    // X coordinate changes 200 pixels/sec so after 1 sec, should be at 200
    CHECK(access.get_players_()[0].get_character().get_position().x == 200);


    // Set the character to "walking right"
    access.set_bool(access.get_players_()[0], "is_walking_left", true);

    // Character position must stay the same after this loop because both
    // "is_walking_left" and "is_walking_right" are true
    access.on_frame(dt);

    CHECK(access.get_players_()[0].get_character().get_position().x == 200);

    // Set "is walking right" to false, and check that character moves back to
    // original position
    access.set_bool(access.get_players_()[0], "is_walking_right", false);
    access.set_bool(access.get_players_()[0], "is_walking_left", true);

    access.on_frame(dt);

    CHECK(access.get_players_()[0].get_character().get_position().x == 0);

    access.set_bool(access.get_players_()[0], "is_walking_left", false);

    // Setting player action to jumping, not grounded
    access.set_bool(access.get_players_()[0], "is_jumping", true);
    access.set_bool(access.get_players_()[0], "is_grounded", false);

    dt = 0.6;

    // Max jump height = config.floor_y - config.character_jump_height
    // 100 = 400 - 300
    // Player jumps 500 pixels/sec so after 0.6 secs, player will jump 300
    // pixels
    // 400 - 300 = 100, player should be at y = 100
    access.on_frame(dt);

    CHECK(access.get_players_()[0].get_character().get_position().y == 100);

    // After reaching 300, which is maximum jump height, player should begin to
    CHECK(access.get_bool(access.get_players_()[0], "is_landing"));

    access.on_frame(dt);

    // After same amount of time passes, they should land back at original pos
    CHECK(access.get_players_()[0].get_character().get_position().y == 400);
}

TEST_CASE("player1 has more health; timer runs out and winner is declared")
{
    Model model(config);
    Test_access access(model);

    // Advance from start screen to match state
    access.progress_game_state();
    access.progress_game_state();
    access.progress_game_state();

    CHECK(access.model.get_game_state() == Game_state::match_state);

    // Manually create characters
    Character character1 = Character("ryu", {500, 500}, config);
    Character character2 = Character("ken", {750, 500}, config);
    model.add_players(character1);
    model.add_players(character2);

    // Set player two's health to 50
    access.set_health(access.get_players_()[1], 50);
    CHECK(access.get_health(access.get_players_()[1]) == 50);

    double dt = 1;
    // Elapse 101 seconds to end timer
    for (int i = 0; i < 101; i++) {
        access.on_frame(dt);
    }

    CHECK(access.model.get_timer() < 0);
    CHECK(access.model.get_winner() == player_num::one);
    CHECK(access.model.get_game_state() == Game_state::after_match_state);

    // Do the same but for player two
    access.reset_game();

    // Advance from character select to match state
    access.progress_game_state();
    access.progress_game_state();

    // Manually create characters
    character1 = Character("ryu", {500, 500}, config);
    character2 = Character("ken", {750, 500}, config);
    model.add_players(character1);
    model.add_players(character2);

    // Set player one's health to 50
    access.set_health(access.get_players_()[0], 50);
    CHECK(access.get_health(access.get_players_()[0]) == 50);

    dt = 1;
    // Elapse 101 seconds to end timer
    for (int i = 0; i < 101; i++) {
        access.on_frame(dt);
    }

    CHECK(access.model.get_timer() < 0);
    CHECK(access.model.get_winner() == player_num::two);
    CHECK(access.model.get_game_state() == Game_state::after_match_state);
}

TEST_CASE("combat moves inflict correct damage")
{
    Model model(config);
    Test_access access(model);

    // Advance from start screen
    access.progress_game_state();

    // Character select screen
    // access.model.

    // Left side of character is actually current position - width of sprite * 3
    Character character1 = Character("ryu", {500, 500}, config);
    Character character2 = Character("ken", {750, 500}, config);
    model.add_players(character1);
    model.add_players(character2);

    //Advance to match state
    access.progress_game_state();
    access.progress_game_state();

    //Set Player 1 to punch player 2
    access.set_bool(access.get_players_()[0], "is_punching", true);

    double dt = 1;
    int current_health = 100;

    // Run loop 30 times. Since each punch takes 3 frames, and each punch
    // takes 5 health, health should be 50 afterwards
    for(int i = 0; i < 30; i++) {
        access.on_frame(dt);
        access.set_bool(access.get_players_()[0], "is_punching", true);
        if (i % 3 == 1) {
            CHECK(access.get_players_()[1].get_health() == current_health - 5);
            current_health -= 5;
        }
    }
    CHECK(access.get_players_()[1].get_health() == 50);


    //Setting punching to false and kicking to true in order to test kicking
    access.set_bool(access.get_players_()[0], "is_punching", false);
    access.set_bool(access.get_players_()[0], "is_kicking", true);

    // Run loop 12 times since each kick takes 3 frames, in order to get health
    // 10
    for (int i = 0; i < 12; i++){
        access.on_frame(dt);
        access.set_bool(access.get_players_()[0], "is_kicking", true);
        if (i % 3 == 1) {
            CHECK(access.get_players_()[1].get_health() == current_health - 10);
            current_health -= 10;
        }
    }

    CHECK(access.get_players_()[1].get_health() == 10);


    access.set_bool(access.get_players_()[0], "is_kicking", false);
    access.set_bool(access.get_players_()[0], "is_shooting_projectile", true);

    //Decreasing dt so projectile doesn't go past the other player
    dt = 1/60;

    for (int i = 0; i < 60; i++) {
        access.on_frame(dt);
    }

    //Check other player's health has dropped to 0
    CHECK(access.get_players_()[1].get_health() == 0);
    CHECK(access.get_players_()[0].get_projectiles().size() == 0);

    CHECK(access.model.get_winner() == player_num::one);
    CHECK(model.get_game_state() == Game_state::after_match_state);
}

Test_access::Test_access(Model& model)
        : model(model)
{}

void
Test_access::progress_game_state()
{
    model.progress_game_state();
}

std::vector<Player>&
Test_access::get_players_()
{
    return model.players;
}

void
Test_access::set_bool(Player& player, std::string boolean1, bool boolean2)
{
    if (boolean1 == "is_walking_right") {
        player.is_walking_right = boolean2;
    }
    else if (boolean1 == "is_walking_left") {
        player.is_walking_left = boolean2;
    }
    else if (boolean1 == "is_jumping") {
        player.is_jumping = boolean2;
    }
    else if (boolean1 == "is_grounded") {
        player.is_grounded = boolean2;
    }
    else if (boolean1 == "is_landing") {
        player.is_landing = boolean2;
    }
    else if (boolean1 == "is_kicking") {
        player.is_kicking = boolean2;
    }
    else if (boolean1 == "is_hit") {
        player.is_hit = boolean2;
    }
    else if (boolean1 == "is_punching")
    {
        player.is_punching = boolean2;
    }
    else if (boolean1 == "is_blocking") {
        player.is_blocking = boolean2;
    }
    else if (boolean1 == "is_crouching") {
        player.is_crouching = boolean2;
    }
    else if (boolean1 == "is_shooting_projectile") {
        player.is_shooting_projectile = boolean2;
    }
    else if (boolean1 == "shot_projectile") {
        player.shot_projectile = boolean2;
    }
    else {
        player.reached_max_frames = boolean2;
    }
}

bool&
Test_access::get_bool(Player& player, std::string boolean)
{
    if (boolean == "is_walking_right") {
        return player.is_walking_right;
    }
    else if (boolean == "is_walking_left") {
        return player.is_walking_left;
    }
    else if (boolean == "is_jumping") {
        return player.is_jumping;
    }
    else if (boolean == "is_grounded") {
        return player.is_grounded;
    }
    else if (boolean == "is_landing") {
        return player.is_landing;
    }
    else if (boolean == "is_kicking") {
        return player.is_kicking;
    }
    else if (boolean == "is_hit") {
        return player.is_hit;
    }
    else if (boolean == "is_punching")
    {
        return player.is_punching;
    }
    else if (boolean == "is_blocking") {
        return player.is_blocking;
    }
    else if (boolean == "is_crouching") {
        return player.is_crouching;
    }
    else if (boolean == "is_shooting_projectile") {
        return player.is_shooting_projectile;
    }
    else if (boolean == "shot_projectile") {
        return player.shot_projectile;
    }
    else {
        return player.reached_max_frames;
    }
}

int
Test_access::get_health(Player& player)
{
    return player.get_health();
}

void
Test_access::set_health(Player& player, int health)

{
    player.health = health;
}

void
Test_access::on_frame(double dt)
{
    model.on_frame(dt);
}

void
Test_access::check_game_end()
{
    model.check_game_end();
}

void
Test_access::player_one_selects_character()
{
    model.player_one_selected_character = true;
}

void
Test_access::player_two_selects_character()
{
    model.player_two_selected_character = true;
}

bool&
Test_access::get_selected_character_bool(std::string str)
{
    if(str == "player_one_selected_character"){
        return model.player_one_selected_character;
    }
    else if(str == "player_two_selected_character"){
        return model.player_two_selected_character;
    }
    else{
        return model.player_two_selected_character;
    }
}

void
Test_access::player_one_selects_map()
{
    model.player_one_selected_map = true;
}

void
Test_access::player_two_selects_map()
{
    model.player_two_selected_map = true;
}

bool&
Test_access::get_selected_map_bool(std::string str)
{
    if(str == "player_one_selected_map"){
        return model.player_one_selected_character;
    }
    else if(str == "player_two_selected_map"){
        return model.player_two_selected_character;
    }
    else{
        return model.player_two_selected_map;
    }
}

void
Test_access::reset_game()
{
    model.reset_game();
}