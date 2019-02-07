/*
	CSC D84 - Unit 2 - MiniMax search and adversarial games

	This file contains stubs for implementing a MiniMax search
        procedure with alpha-beta pruning. Please read the assignment
	handout carefully - it describes the game, the data you will
	have to handle, and the search functions you must provide.

	Once you have read the handout carefully, implement your search
	code in the sections below marked with

	**************
	*** TO DO:
	**************

	Make sure to add it to your report.txt file - it will be marked!

	Have fun!

	DO NOT FORGET TO 'valgrind' YOUR CODE - We will check for pointer
	management being done properly, and for memory leaks.

	Starter code: F.J.E. Sep. 15
*/

#include "MiniMax_search.h"

int prev_util = 0;

double MiniMax(double gr[graph_size][4], int path[1][2], double minmax_cost[size_X][size_Y], int cat_loc[10][2], int cats, int cheese_loc[10][2], int cheeses, int mouse_loc[1][2], int mode, double (*utility)(int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, int depth, double gr[graph_size][4]), int agentId, int depth, int maxDepth, double alpha, double beta)
{
 /*
   This function is the interface between your solution for the assignment and the driver code. The driver code
   in MiniMax_search_core_GL will call this function once per frame, and provide the following data

   Board and game layout:

	Exactly the same as for Assignment 1 - have a look at your code if you need a reminder of how the adjacency
	list and agent positions are stored.

	Note that in this case, the path will contain a single move - at the top level, this function will provide
	an agent with the 'optimal' mini-max move given the game state.

5
   IMPORTANT NOTE: Mini-max is a recursive procedure. This function will need to fill-in the mini-max values for
	 	   all game states down to the maximum search depth specified by the user. In order to do that,
		   the function needs to be called with the correct state at each specific node in the mini-max
		   search tree.

		   The game state is composed of:

			* Mouse, cat, and cheese positions (and number of cats and cheeses)

		   At the top level (when this function is called by the mini-max driver code), the game state
		   correspond to the current situation of the game. But once you start recursively calling
		   this function for lower levels of the search tree the positions of agents will have changed.

		   Therefore, you will need to define local variables to keep the game state at each node of the
		   mini-max search tree, and you will need to update this state when calling recursively so that
		   the search does the right thing.

		   This function *must check* whether:
			* A candidate move results in a terminal configuration (cat eats mouse, mouse eats cheese)
			  at which point it calls the utility function to get a value
	 		* Maximum search depth has been reached (depth==maxDepth), at which point it will also call
			  the utility function to get a value
			* Otherwise, call recursively using the candidate configuration to find out what happens
			  deeper into the mini-max tree.

   Arguments:
		gr[graph_size][4]   		- This is an adjacency list for the maze
		path[1][2] 			- Your function will return the optimal mini-max move in this array.
		minmax_cost[size_X][size_Y]	- An array in which your code will store the
						  minimax value for maze locations expanded by
						  the search *when called for the mouse, not
						  for the cats!*

						  This array will be used to provide a visual
						  display of minimax values during the game.

		cat_loc[10][2], cats   - Location of cats and number of cats (we can have at most 10,
					 but there can be fewer). Only valid cat locations are 0 to (cats-1)
		cheese_loc[10][2], cheeses - Location and number of cheese chunks (again at most 10,
					     but possibly fewer). Valid locations are 0 to (cheeses-1)
		mouse_loc[1][2] - Mouse location - there can be only one!
		mode - Search mode selection:
					mode = 0 	- No alpha-beta pruning
					mode = 1	- Alpha-beta pruning

		(*utility)(int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, int depth, double gr[graph_size][4]);
				- This is a pointer to the utility function which returns a value for a specific game configuration

				   NOTE: Unlike the search assignment, this utility function also gets access to the graph so you can do any processing 					 that requires knowledge of the maze for computing the utility values.

				  * How to call the utility function from within this function : *
					- Like any other function:
						u = utility(cat_loc, cheese_loc, mouse_loc, cats, cheeses, depth, gr);

		agentId: Identifies which agent we are doing MiniMax for. agentId=0 for the mouse, agentId in [1, cats] for cats. Notice that recursive calls
                         to this function should increase the agentId to reflect the fact that the next level down corresponds to the next agent! For a game
                         with two cats and a mouse, the agentIds for the recursion should look like 0, 1, 2, 0, 1, 2, ...

		depth: Current search depth - whether this is a MIN or a MAX node depends both on depth and agentId.

		maxDepth: maximum desired search depth - once reached, your code should somehow return
			  a minimax utility value for this location.

		alpha. beta: alpha and beta values passed from the parent node to constrain search at this
			     level.

   Return values:
		Your search code will directly update data passed-in as arguments:

		- Mini-Max value	: Notice this function returns a double precision number5. This is
					  the minimax value at this level of the tree. It will be used
					  as the recursion backtracks filling-in the mini-max values back
					  from the leaves to the root of the search tree.

		- path[1][2]		: Your MiniMax function will return the location for the agent's
					  next location (i.e. the optimal move for the agent).
		- minmax_cost[size_X][size_Y] 	:  Your search code will update this array to contain the
						   minimax value for locations that were expanded during
						   the search. This must be done *only* for the mouse.

						   Values in this array will be in the range returned by
						   your utility function.
#include "MiniMax_search.h"
		* Your code MUST NOT modify the locations or numbers of cats and/or cheeses, the graph,
	     	  or the location of the mouse - if you try, the driver code will know it *

		That's that, now, implement your solution!
 */

 /********************************************************************************************************
 *
 * TO DO:	Implement code to perform a MiniMax search. This will involve a limited-depth BFS-like
 *              expansion. Once nodes below return values, your function will propagate minimax utilities
 *		as per the minimax algorithm.
 *
 *		Note that if alpha-beta pruning is specified, you must keep track of alphas and betas
 *		along the path.
 *
 *		You can use helper functions if it seems reasonable. Add them to the MiniMax_search.h
 *		file and explain in your code why they are needed and how they are used.
 *
 *		Recursion should appear somewhere.
 *
 *		MiniMax cost: If the agentId=0 (Mouse), then once you have a MiniMax value for a location
 *		in the maze, you must update minmax_cost[][] for that location.
 *
 *		How you design your solution is up to you. But:
 *
 *		- Document your implementation by adding concise and clear comments in this file
 *		- Document your design (how you implemented the solution, and why) in the report
 *
 ********************************************************************************************************/

 // Stub so that the code compiles/runs - This will be removed and replaced by your code!
   if(depth == maxDepth || checkForTerminal(mouse_loc, cat_loc, cheese_loc, cats, cheeses)) {
     return utility(cat_loc, cheese_loc, mouse_loc, cats, cheeses, depth, gr);
   }
   if(agentId == 0) {
     int curr_loc[1][2];
     int best_loc[1][2];
     curr_loc[0][0] = mouse_loc[0][0];
     curr_loc[0][1] = mouse_loc[0][1];
     double maxEval = -INFINITY;
     int x = 0;
     //Reset all minimax_cost values to 0
     if(depth == 0 && x == 0) {
       for(int i = 0; i < size_X; i++) {
         for(int j = 0; j < size_Y; j++) {
           minmax_cost[i][j] = 0;
         }
       }
     }
     for (x = 0; x < 4; x++) {
       if(gr[curr_loc[0][0] + ((curr_loc[0][1]) * size_X)][x] == 1) {
         if(x == 0 || x == 2) {
           curr_loc[0][1] += (x - 1);
         } else if (x == 1) {
           curr_loc[0][0] += 1;
         } else {
           curr_loc[0][0] -= 1;
         }
         double eval = MiniMax(gr, path, minmax_cost, cat_loc, cats, cheese_loc, cheeses, curr_loc, mode, utility, agentId + 1, depth + 1, maxDepth, alpha, beta);
         if(eval > maxEval) {
           maxEval = eval;
           best_loc[0][0] = curr_loc[0][0];
           best_loc[0][1] = curr_loc[0][1];
         }
         if(mode == 1) {
           if(alpha < eval) {
             alpha = eval;
           }
           if(beta <= alpha) {
             break;
           }
         }
         minmax_cost[curr_loc[0][0]][curr_loc[0][1]] = eval;
         curr_loc[0][0] = mouse_loc[0][0];
         curr_loc[0][1] = mouse_loc[0][1];
       }
     }
     if(depth == 0 && x == 4) {
       path[0][0] = best_loc[0][0];
       path[0][1] = best_loc[0][1];
     }
     return maxEval;
   } else {
     int curr_cat = agentId - 1;
     int curr_loc[cats][2];
     for(int x = 0; x < cats; x++) {
       curr_loc[x][0] = cat_loc[x][0];
       curr_loc[x][1] = cat_loc[x][1];
     }
     double minEval = INFINITY;
     for (int x = 0; x < 4; x++) {
       if(gr[curr_loc[curr_cat][0] + ((curr_loc[curr_cat][1]) * size_X)][x] == 1) {
         if(x == 0 || x == 2) {
           curr_loc[curr_cat][1] += (x - 1);
         } else if (x == 1) {
           curr_loc[curr_cat][0] += 1;
         } else {
           curr_loc[curr_cat][0] -= 1;
         }
         if(agentId == cats) {
           agentId = -1;
         }
         double eval = MiniMax(gr, path, minmax_cost, curr_loc, cats, cheese_loc, cheeses, mouse_loc, mode, utility, agentId + 1, depth + 1, maxDepth, alpha, beta);
         if(eval < minEval) {
           minEval = eval;
         }
         if(mode == 1) {
           if(beta > eval) {
             beta = eval;
           }
           if(beta <= alpha) {
             break;
           }
         }
         for(int x = 0; x < cats; x++) {
           curr_loc[x][0] = cat_loc[x][0];
           curr_loc[x][1] = cat_loc[x][1];
         }
       }
     }
     return minEval;
   }

 //utility(cat_loc, cheese_loc, mouse_loc, cats, cheeses, depth, gr);

}

double utility(int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, int depth, double gr[graph_size][4])
{
 /*
	This function computes and returns the utility value for a given game configuration.
	As discussed in lecture, this should return a positive value for configurations that are 'good'
	for the mouse, and a negative value for locations that are 'bad' for the mouse.

	How to define 'good' and 'bad' is up to you. Note that you can write a utility function
	that favours your mouse or favours the cats, but that would be a bad idea... (why?)

	Input arguments:#include "MiniMax_search.h"

		cat_loc - Cat locations
		cheese_loc - Cheese locations
		mouse_loc - Mouse location
		cats - # of heesecats
		cheeses - # of cheeses
		depth - current search depth
		gr - The graph's adjacency list for the maze

		These arguments are as described in A1. Do have a look at your solution!
 */

 double cheese_factor = 25 - calculateMinDistance(mouse_loc, cheese_loc, cheeses);
 if(cheese_factor >= 23) {
   cheese_factor *= 3;
 } else if(cheese_factor >= 20) {
   cheese_factor *= 2.2;
 } else if(cheese_factor >= 17) {
   cheese_factor *= 2;
 } else if(cheese_factor >= 15) {
   cheese_factor *= 1.5;
 } else {
   cheese_factor *= 1.3;
 }
 double cat_factor = 25 - calculateMinDistance(mouse_loc, cat_loc, cats);
 if(cat_factor >= 23) {
   cat_factor *= 5;
 } else if(cat_factor >= 20) {
   cat_factor *= 3;
 } else if(cat_factor >= 17) {
   cat_factor = 1.5;
 } else {
   cat_factor = 0;
 }

 printf("Cheese: %f", cheese_factor);
 printf("Cat: %f", cat_factor);

  //farther distance to cheese means lower utility, closer means higher utility
  //closer distance to cat means lower utility, farther distance from cat means higher utility
  double util = cheese_factor - cat_factor;

  return util;
}

double calculateMinDistance(int mouse_loc[1][2], int object_loc[10][2], int objects) {
  double min_dist = INFINITY;
  for(int x = 0; x < objects; x++) {
    double curr_dist = sqrt(pow(object_loc[x][0] - mouse_loc[x][0], 2) + pow(object_loc[x][1] - mouse_loc[x][1], 2));
    if(min_dist > curr_dist) {
      min_dist = curr_dist;
    }
  }
  return min_dist;
}

int checkForTerminal(int mouse_loc[1][2],int cat_loc[10][2],int cheese_loc[10][2],int cats,int cheeses)
{
 /*
   This function determines whether a given configuration constitutes a terminal node.
   Terminal nodes are those for which:
     - A cat eats the mouse
     or
     - The mouse eats a cheese

   If the node is a terminal, the function returns 1, else it returns 0
 */

 // Check for cats having lunch
 for (int i=0; i<cats; i++)
  if (mouse_loc[0][0]==cat_loc[i][0]&&mouse_loc[0][1]==cat_loc[i][1]) return(1);

 // Check for mouse having lunch
 for (int i=0; i<cheeses; i++)
  if (mouse_loc[0][0]==cheese_loc[i][0]&&mouse_loc[0][1]==cheese_loc[i][1]) return(1);

 return(0);

}
