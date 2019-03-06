/*
	CSC D84 - Unit 3 - Reinforcement Learning

	This file contains stubs for implementing the Q-Learning method
	for reinforcement learning as discussed in lecture. You have to
	complete two versions of Q-Learning.

	* Standard Q-Learning, based on a full-state representation and
	  a large Q-Table
	* Feature based Q-Learning to handle problems too big to allow
	  for a full-state representation

	Read the assignment handout carefully, then implement the
	required functions below. Sections where you have to add code
	are marked

	**************
	*** TO DO:
	**************

	If you add any helper functions, make sure you document them
	properly and indicate in the report.txt file what you added.

	Have fun!

	DO NOT FORGET TO 'valgrind' YOUR CODE - We will check for pointer
	management being done properly, and for memory leaks.

	Starter code: F.J.E. Jan. 16
*/

#include "QLearn.h"

int queue[max_graph_size];
int front = 0;
int queue_size = 0;

void QLearn_update(int s, int a, double r, int s_new, double * QTable) {
  /*
    This function implementes the Q-Learning update as stated in Lecture. It
    receives as input a <s,a,r,s'> tuple, and updates the Q-table accordingly.

    Your work here is to calculate the required update for the Q-table entry
    for state s, and apply it to the Q-table

    The update involves two constants, alpha and lambda, which are defined in QLearn.h - you should not
    have to change their values. Use them as they are.

    Details on how states are used for indexing into the QTable are shown
    below, in the comments for QLearn_action. Be sure to read those as well!
  */

  /***********************************************************************************************
   * TO DO: Complete this function
   ***********************************************************************************************/
  double max_next = -INFINITY;
  for (int x = 0; x < 4; x++) {
    double curr = *(QTable + (4 * s_new) + x);
    if (curr > max_next) {
      max_next = curr;
    }
  }
  double curr_val = *(QTable + (4 * s) + a);
  *(QTable + (4 * s) + a) = curr_val + alpha * (r + lambda * max_next - curr_val);
}

int QLearn_action(double gr[max_graph_size][4], int mouse_pos[1][2], int cats[5][2], int cheeses[5][2], double pct, double * QTable, int size_X, int graph_size) {
  /*
     This function decides the action the mouse will take. It receives as inputs
     - The graph - so you can check for walls! The mouse must never move through walls
     - The mouse position
     - The cat position
     - The chees position
     - A 'pct' value in [0,1] indicating the amount of time the mouse uses the QTable to decide its action,
       for example, if pct=.25, then 25% of the time the mouse uses the QTable to choose its action,
       the remaining 75% of the time it chooses randomly among the available actions.

     Remember that the training process involves random exploration initially, but as training
     proceeds we use the QTable more and more, in order to improve our QTable values around promising
     actions.

     The value of pct is controlled by QLearn_core_GL, and increases with each round of training.

     This function *must return* an action index in [0,3] where
        0 - move up
        1 - move right
        2 - move down
        3 - move left

     QLearn_core_GL will print a warning if your action makes the mouse cross a wall, or if it makes
     the mouse leave the map - this should not happen. If you see a warning, fix the code in this
     function!

   The Q-table has been pre-allocated and initialized to 0. The Q-table has
   a size of

        graph_size^3 x 4

   This is because the table requires one entry for each possible state, and
   the state is comprised of the position of the mouse, cat, and cheese.
   Since each of these agents can be in one of graph_size positions, all
   possible combinations yield graph_size^3 states.

   Now, for each state, the mouse has up to 4 possible moves (up, right,
   down, and left). We ignore here the fact that some moves are not possible
   from some states (due to walls) - it is up to the QLearn_action() function
   to make sure the mouse never crosses a wall.

   So all in all, you have a big table.

   For example, on an 8x8 maze, the Q-table will have a size of

       64^3 x 4  entries

       with

       size_X = 8		<--- size of one side of the maze
       graph_size = 64		<--- Total number of nodes in the graph

   Indexing within the Q-table

     say the mouse is at   i,j
         the cat is at     k,l
         the cheese is at  m,n

     state = (i+(j*size_X)) + ((k+(l*size_X))*graph_size) + ((m+(n*size_X))*graph_size*graph_size)
     ** Make sure you undestand the state encoding above!

     Entries in the Q-table for this state are

     *(QTable+(4*state)+a)      <-- here a is the action in [0,3]

     (yes, it's a linear array, no shorcuts with brackets!)

     NOTE: There is only one cat and once cheese, so you only need to use cats[0][:] and cheeses[0][:]
   */

  /***********************************************************************************************
   * TO DO: Complete this function
   ***********************************************************************************************/

  // maybe do the randomization better
  int randChance = (int)100*pct;
  int dieRoll = rand()%100+1;
  // maybe do the randomization better
  int move = 0;
  int currentMouseIndex = mouse_pos[0][0] + mouse_pos[0][1]*size_X;
  int state = (mouse_pos[0][0]+(mouse_pos[0][1]*size_X)) + ((cats[0][0]+(cats[0][1]*size_X))*graph_size) + ((cheeses[0][0]+(cheeses[0][1]*size_X))*graph_size*graph_size);

  // printf("pct: %d\n", randChance);
  // printf("rand: %d\n", dieRoll);

  // if below exploit
  if (randChance >= dieRoll){
    double maxVal = -INFINITY;
    int maxIndex;
    for(int a = 0; a < 4; a++){
      double currentVal = *(QTable+(4*state)+a);
      if ((gr[currentMouseIndex][a])&&(maxVal < currentVal)){
        maxVal = currentVal;
        maxIndex = a;
      }
    }
    move = maxIndex;
  // o/w explore
  } else {
    int initMoves[4];
    // should always be positive by assumption
    int counter = 0;
    for (int i = 0; i < 4; i++){
      initMoves[i] = -1;
    }
    for (int i = 0; i < 4; i++){
      if (gr[currentMouseIndex][i]){
        initMoves[i] = i;
        counter++;
      }
    }
    int availableMoves[counter];
    int nCounter = 0;
    for (int j = 0; j < 4; j++){
        if (initMoves[j] != -1){
          availableMoves[nCounter] = initMoves[j];
          nCounter++;
        }
    }
    int randMove = rand()%counter;
    move = availableMoves[randMove];
  }
  return move; // <--- of course, you will change this!
}

double QLearn_reward(double gr[max_graph_size][4], int mouse_pos[1][2], int cats[5][2], int cheeses[5][2], int size_X, int graph_size) {
  /*
    This function computes and returns a reward for the state represented by the input mouse, cat, and
    cheese position.

    You can make this function as simple or as complex as you like. But it should return positive values
    for states that are favorable to the mouse, and negative values for states that are bad for the
    mouse.

    I am providing you with the graph, in case you want to do some processing on the maze in order to
    decide the reward.

    This function should return a maximim/minimum reward when the mouse eats/gets eaten respectively.
   */

  /***********************************************************************************************
   * TO DO: Complete this function
   ***********************************************************************************************/
  double distances_from_mouse[32][32];
  for (int x = 0; x < size_X; x++) {
    for (int y = 0; y < size_X; y++) {
      distances_from_mouse[x][y] = INFINITY;
    }
  }
  distances_from_mouse[mouse_pos[0][0]][mouse_pos[0][1]] = 0;
  int mouse_index = mouse_pos[0][0] + (mouse_pos[0][1] * size_X);
  BFS(gr, mouse_index, cats, 1, cheeses, 1, distances_from_mouse, size_X);
  double reward = 0;
  double cat_dist = distances_from_mouse[cats[0][0]][cats[0][1]];
  double cheese_dist = distances_from_mouse[cheeses[0][0]][cheeses[0][1]];

  //If mouse gets eaten by cat, give a negative reward. However, if it survives and if mouse eats
  //cheese, then give a positive reward. If none of these cases, then other computations are done.
  if (cat_dist == 0) {
    reward -= 50;
  } else if (cheese_dist == 0) {
    reward += 50;
  } else {
    //The farther the cheese, thefeature based learningo reward, and the closer the cat, the more is
    //subtracted from the reward.feature based learning
    reward = (1 / cheese_dist) * 25 - cat_dist * 0.03;
    double cat_cheese_dist_diff = cheese_dist - cat_dist;
    //If cat is farther from mouse than cheese, then add to the reward depending on how far the cat is
    //from the mouse. If cat is closer to mouse than cheese, then subtract from the reward depending
    //on how close the cat is to the mouse.
    if (cat_cheese_dist_diff <= -4) {
      reward += 9;
    } else if (cat_cheese_dist_diff <= -2) {
      reward += 4;
    } else if (cat_cheese_dist_diff <= 0) {
      reward += 1;
    } else if (cat_cheese_dist_diff >= 5) {
      reward -= 12;
    } else if (cat_cheese_dist_diff >= 3) {
      reward -= 6;
    } else if (cat_cheese_dist_diff >= 1) {
      reward -= 2;
    }
    //If mouse is within 2 steps of getting cheese, then add more to the reward.
    if (cheese_dist <= 2) {
      reward += 10;
    }
    int wall_counter = 0;
    //Count number of walls around mouse.
    for (int x = 0; x < 4; x++) {
      if (gr[mouse_index][x] == 0) {
        wall_counter++;
      }
    }
    //If mouse is at a square with 3 walls around it then reduce the reward some more.
    if (wall_counter == 3) {
      reward -= 50;
    }
  }
  return reward*(size_X/2);
}

void feat_QLearn_update(double gr[max_graph_size][4], double weights[25], double reward, int mouse_pos[1][2], int cats[5][2], int cheeses[5][2], int size_X, int graph_size) {
  /*
    This function performs the Q-learning adjustment to all the weights associated with your
    features. Unlike standard Q-learning, you don't receive a <s,a,r,s'> tuple, instead,
    you receive the current state (mouse, cats, and cheese potisions), and the reward
    associated with this action (this is called immediately after the mouse makes a move,
    so implicit in this is the mouse having selected some action)

    Your code must then evaluate the update and apply it to the weights in the weight array.
   */

  /***********************************************************************************************
   * TO DO: Complete this function
   ***********************************************************************************************/

  double* maxU; //max q value given current state
  int* maxA; //max action
  maxQsa(gr, weights, mouse_pos, cats, cheeses, size_X, graph_size, maxU, maxA);
  double defU = *maxU;
  //need current Qsa value as well
  double features[25];
  evaluateFeatures(gr, features, mouse_pos, cats, cheeses, size_X, graph_size);

  //evaluate current QVal
  double currentQVal = Qsa(weights, features);
  double tdp = (reward + lambda*(defU)) - currentQVal;

  for (int i = 0; i < 25; i++){
    weights[i] += alpha*tdp*features[i];
  }
}

int feat_QLearn_action(double gr[max_graph_size][4], double weights[25], int mouse_pos[1][2], int cats[5][2], int cheeses[5][2], double pct, int size_X, int graph_size) {
  /*
    Similar to its counterpart for standard Q-learning, this function returns the index of the next
    action to be taken by the mouse.

    Once more, the 'pct' value controls the percent of time that the function chooses an optimal
    action given the current policy.

    E.g. if 'pct' is .15, then 15% of the time the function uses the current weights and chooses
    the optimal action. The remaining 85% of the time, a random action is chosen.

    As before, the mouse must never select an action that causes it to walk through walls or leave
    the maze.
   */

  /***********************************************************************************************
   * TO DO: Complete this function
   ***********************************************************************************************/

  // maybe do the randomization better
  int randChance = (int)100*pct;
  int dieRoll = rand()%100+1;0;
  // maybe do the randomization better
  int move = 0;
  int currentMouseIndex = mouse_pos[0][0] + mouse_pos[0][1]*size_X;


  // if below exploit
  if (randChance >= dieRoll){
    int* bestMove;
    double* bestStateValue;
    maxQsa(gr, weights, mouse_pos, cats, cheeses, size_X, graph_size, bestStateValue, bestMove);
    // consider current state and do the best move
    move = *bestMove;
  // o/w explore
  } else {
    int initMoves[4];
    // should always be positive by assumption
    int counter = 0;
    for (int i = 0; i < 4; i++){
      initMoves[i] = -1;
    }
    for (int i = 0; i < 4; i++){
      if (gr[currentMouseIndex][i]){
        initMoves[i] = i;
        counter++;
      }
    }
    int availableMoves[counter];
    int nCounter = 0;
    for (int j = 0; j < 4; j++){
        if (initMoves[j] != -1){
          availableMoves[nCounter] = initMoves[j];
          nCounter++;
        }
    }
    int randMove = rand()%counter;
    move = availableMoves[randMove];
  }
  return move;
}

void evaluateFeatures(double gr[max_graph_size][4], double features[25], int mouse_pos[1][2], int cats[5][2], int cheeses[5][2], int size_X, int graph_size) {
  /*
   This function evaluates all the features you defined for the game configuration given by the input
   mouse, cats, and cheese positions. You are free to define up to 25 features. This function will
   evaluate each, and return all the feature values in the features[] array.

   Take some time to think about what features would be useful to have, the better your features, the
   smarter your mouse!

   Note that instead of passing down the number of cats and the number of cheese chunks (too many parms!)
   the arrays themselves will tell you what are valid cat/cheese locations.

   You can have up to 5 cats and up to 5 cheese chunks, and array entries for the remaining cats/cheese
   will have a value of -1 - check this when evaluating your features!
  */

  /***********************************************************************************************
   * TO DO: Complete this function
   ***********************************************************************************************/
  double distances_from_mouse[32][32];
  for (int x = 0; x < size_X; x++) {
    for (int y = 0; y < size_X; y++) {
      distances_from_mouse[x][y] = INFINITY;
    }
  }
  distances_from_mouse[mouse_pos[0][0]][mouse_pos[0][1]] = 0;
  int mouse_index = mouse_pos[0][0] + (mouse_pos[0][1] * size_X);
  BFS(gr, mouse_index, cats, 1, cheeses, 1, distances_from_mouse, size_X);
  double cat_dist = distances_from_mouse[cats[0][0]][cats[0][1]];
  double cheese_dist = distances_from_mouse[cheeses[0][0]][cheeses[0][1]];

  int numberOfCheese = 0;
  int numberOfCats = 0;

  for (int j = 0; j<5; j++){
    //only need to check one coordinate
    if (cats[j][0] != -1){
      numberOfCats++;
    }
    if (cheeses[j][0] != -1){
      numberOfCheese++;
    }
  }

  double maxCat = -INFINITY;
  double maxCheese = -INFINITY;

  for (int i = 0; i < numberOfCats; i++){
    if (distances_from_mouse[cats[i][0]][cats[i][1]] > maxCat){
      maxCat = distances_from_mouse[cats[i][0]][cats[i][1]];
    }
  }

  for (int i = 0; i < numberOfCheese; i++){
    if (distances_from_mouse[cheeses[i][0]][cheeses[i][1]] > maxCheese){
      maxCheese = distances_from_mouse[cheeses[i][0]][cheeses[i][1]];
    }
  }

  for (int i = 0; i < numberOfCheese; i++){}

  //assume values int cats, cheeses are non -1 for the first catsInGame, cheeseInGame indices

  // int catsInGame[numberOfCats];
  // int cheeseInGame[numberOfCheese];

  //feature 1 - closest cheese via gaussian func
  //feature 2 - closest cat via gaussian func
  //new features - deadends and corners possibly
  //also new features - maybe mean distance between cats/cheeses

  for (int i = 0; i<25; i++){
    switch(i){
      case 1: features[i] = (double)1.0/(maxCheese+1.0);
      case 2: features[i] = (double)1.0/(maxCat+1.0);
      default: features[i] = (double)0;
    }
  }
  return;
}

double Qsa(double weights[25], double features[25]) {
  /*
    Compute and return the Qsa value given the input features and current weights
   */

  /***********************************************************************************************
   * TO DO: Complete this function
   ***********************************************************************************************/

  double ret = 0;
  //maybe make # features less than 25 (depending on how many features we have)
  for (int i = 0; i<25; i++){
    ret += weights[i]*features[i];
  }

  return ret;
}

void maxQsa(double gr[max_graph_size][4], double weights[25], int mouse_pos[1][2], int cats[5][2], int cheeses[5][2], int size_X, int graph_size, double * maxU, int * maxA) {
  /*
    Given the state represented by the input positions for mouse, cats, and cheese, this function evaluates
    the Q-value at all possible neighbour states and returns the max. The maximum value is returned in maxU
    and the index of the action corresponding to this value is returned in maxA.

    You should make sure the function does not evaluate moves that would make the mouse walk through a
    wall.
   */

  /***********************************************************************************************
   * TO DO: Complete this function
   ***********************************************************************************************/
   * maxU = -INFINITY;
   int numberOfCats = 0;

   for (int x = 0; x < 5; x++) {
     if (cats[x][0] != -1) {
       numberOfCats++;
     }
   }

   int mouse_index = mouse_pos[0][0] + (mouse_pos[0][1] * size_X);
   for (int x = 0; x < 4; x++) {
     if (gr[mouse_index][x]) {
       int temp_mouse_pos[1][2];
       temp_mouse_pos[0][0] = mouse_pos[0][0] - ((x - 2) % 2);
       temp_mouse_pos[0][1] = mouse_pos[0][1] + ((x - 1) % 2);
       double curr = maxQsaHelper(gr, weights, temp_mouse_pos, cats, numberOfCats, 0, cheeses, size_X, graph_size);
       if (curr > * maxU) {
         * maxU = curr;
         * maxA = x;
       }
     }
   }
}

/***************************************************************************************************
 *  Add any functions needed to compute your features below
 *                 ---->  THIS BOX <-----
 * *************************************************************************************************/

bool allDistancesFound(int cat_loc[5][2], int cats, int cheese_loc[5][2], int cheeses, double distances[32][32]) {
  //check if distance found to all cats
  for (int x = 0; x < cats; x++) {
    if (distances[cat_loc[x][0]][cat_loc[x][1]] == INFINITY) {
      return false;
    }
  }
  //check if distance found to all cheeses
  for (int x = 0; x < cheeses; x++) {
    if (distances[cheese_loc[x][0]][cheese_loc[x][1]] == INFINITY) {
      return false;
    }
  }
  //"empty" the queue
  queue_size = 0;
  front = 0;
  return true;
}

void enqueue(int cell) {
  //Add cell to the queue.
  queue[queue_size + front] = cell;
  queue_size++;
}

int dequeue() {
  //Remove first value from queue.
  int cell = queue[front];
  front++;
  queue_size--;
  return cell;
}

void BFS(double gr[max_graph_size][4], int source_index, int cat_loc[5][2], int cats, int cheese_loc[5][2], int cheeses, double distances[32][32], int size_X) {
  /* This function is used to calculate the BFS distance from the mouse to all cats, and cheeses.*/
  enqueue(source_index);
  while (!allDistancesFound(cat_loc, cats, cheese_loc, cheeses, distances)) {
    int curr = dequeue();
    int curr_x = curr % size_X;
    int curr_y = curr / size_X;
    for (int x = 0; x < 4; x++) {
      //child is the index of the current child
      int child_x = curr_x - ((x - 2) % 2);
      int child_y = curr_y + ((x - 1) % 2);
      int child = child_x + (child_y * size_X);
      if (gr[curr][x] == 1 && distances[child_x][child_y] == INFINITY) {
        distances[child_x][child_y] = distances[curr_x][curr_y] + 1;
        enqueue(child);
      }
    }
  }
}

double maxQsaHelper(double gr[max_graph_size][4], double weights[25], int mouse_pos[1][2], int cats[5][2], int numCats, int counter, int cheeses[5][2], int size_X, int graph_size) {
  if (counter == numCats) {
    return -INFINITY;
  }
  int cat_index = cats[counter][0] + (cats[counter][1] * size_X);
  double curr_max = -INFINITY;
  for (int y = 0; y < 4; y++) {
    if (gr[cat_index][y]) {
      cats[counter][0] = cats[counter][0] - ((y - 2) % 2);
      cats[counter][1] = cats[counter][1] + ((y - 1) % 2);
      double next_max = maxQsaHelper(gr, weights, mouse_pos, cats, numCats, counter + 1, cheeses, size_X, graph_size);
      double features[25];
      evaluateFeatures(gr, features, mouse_pos, cats, cheeses, size_X, graph_size);
      double curr_q = Qsa(weights, features);
      if (curr_q > next_max && curr_q > curr_max) {
        curr_max = curr_q;
      } else if (curr_q <= next_max && next_max > curr_max) {
        curr_max = next_max;
      }
    }
  }
  return curr_max;
}
