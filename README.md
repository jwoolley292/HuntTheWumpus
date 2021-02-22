# HuntTheWumpus
This project involves building an old school dungeon crawler and developing AI to play the game.

The Game
========
The game is set in a dark dungeon containing a monster called the Wumpus, gold and some traps. The objective of the game is to shoot the Wumpus, collect the gold and escape
whilst avoiding the traps. However, the player is unable to see into adjacent rooms. Instead, they have to use their other senses to identify what is nearby. If the player
enters a room adjacent to one containing the Wumpus, they will smell it. If they enter a room adjacent to gold, they will see a slight glisten. If they enter a room adjacent
to a trap, they will feel a breeze. Although this tells them what is nearby, it does not tell them the direction this sense is coming from, so this needs to be worked out by
exploring.

The dungeon map is a 4 x 4 grid of squares. The exit is always in (0, 0). This is where the player starts and where they must go in order to escape. The dungeon contains
exactly one Wumpus and one gold, each in a random room other than the exit. The remaining rooms have a 20% chance of containing a trap. Rooms can only contain one thing or be
empty.

The actions available to the player are:
 - move into an adjacent square (not including diagonals)
 - shoot into an adjacent square. the shot will only reach the adjacent square; it will not continue into further rooms and the player only has one ammo
 - escape, ending the game if the player is on the exit square

If the player walks into the room containing the Wumpus and they haven't killed it, they die. If they walk into the room containing the Wumpus after killing it, they
simply enter the room. If they shoot the Wumpus, it dies. If they walk into the room containing gold, they pick it up. If they walk into a trap, they die.

The player loses 10 points each time they move. 1000 points are deducted if the player dies. Collecting the gold and shooting the Wumpus awards the player with 1000 points
upon escaping the dungeon.

The AI
======
The AI has a knowledge base built in conjunctive normal form which initially contains the rules of the game. When it takes an action, the application tells it the location it is
now in as well as the state of that room. The agent uses DPLL to infer additional information whenever it discovers new information, after which it determines an appropriate
action to take.

https://en.wikipedia.org/wiki/Conjunctive_normal_form

https://en.wikipedia.org/wiki/DPLL_algorithm
