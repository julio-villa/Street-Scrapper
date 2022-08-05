Street-Scrapper is a replica of the original street fighter game created in collaboration with [Jackie Lin](https://github.com/j-ackie). 
## Game States
There are four game states in total: The start screen, character select, background select, the match, and after match state. To advance from the start screen, press any key or mouse button.
# Character & Background Selection
To select a player and background, the A and D keys will move player 1's cursor left and right. The X key locks in their selection. Player 2 uses the left and right arrows and the J key, respectively. 
# Controls
Here is a table with the set of controls:

| Player 1 |  Player 2   |   Action   |
|:--------:|:-----------:|:----------:|
 |    W     |  Up arrow   |    Jump    |
|    A     | Left Arrow  | Move left  |
|    S     | Down Arrow  |   Crouch   |
|    D     | Right Arrow | Move right |
|    X     |      J      |   Punch    |
|    C     |      K      |    Kick    |
|    V     |      L      | Projectile |
|  Shift   |    Space    |   Block|
#Damage
The damage dealt by each action:

| Action     | Damage dealt |
|------------|--------------|
| Punch      | 5            |
| Kick       | 10           |
| Projectile | 10           |

#Declaring a winner
The match has a time limit of 100 seconds. Once the timer reaches 0, the player with the highest health is declared the winner.


