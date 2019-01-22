/*
	CSC D84 - Unit 1 - Search

	This file contains stubs for implementing the different search
	algorithms covered in the course. Please read the assignment
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

	Starter code: F.J.E., Jul. 15
	Updated: F.J.E., Jan. 18
*/

/**********************************************************************
% COMPLETE THIS TEXT BOX:
%
% 1) Student Name: Mrigank Mehta
% 2) Student Name:
%
% 1) Student number: 1001309014
% 2) Student number:
%
% 1) UtorID mehtamri
% 2) UtorID
%
% We hereby certify that the work contained here is our own
%
% Mrigank Mehta                    _____________________
% (sign with your name)            (sign with your name)
***********************************************************************/

#include "AI_search.h"

int queue[graph_size];
int pQueue[graph_size][2];
int front = 0;
int pfront = 0;
int back = -1;
int queueItemCount = 0;
int pQueueItemCount = 0;
int stack[graph_size];
int top = -1;

void search(double gr[graph_size][4], int path[graph_size][2], int visit_order[size_X][size_Y], int cat_loc[10][2], int cats, int cheese_loc[10][2], int cheeses, int mouse_loc[1][2], int mode, int (*heuristic)(int x, int y, int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, double gr[graph_size][4]))
{
 /*
   This function is the interface between your solution for the assignment and the driver code. The driver code
   in AI_search_core_GL will call this function once per frame, and provide the following data

   Board and game layout:

	The game takes place on a grid of size 32x32, the file board_layout.h specifies the size and defines two
	constants 'size_X' and 'size_Y' for the horizontal and vertical size of the board, respectively. For our
	purposes, the grid of locations is represented by a graph with one node per grid location, so given
	the 32x32 cells, the graph has 1024 nodes.

	To create a maze, we connect cell locations in the grid in such a way that a) there is a path from any
	grid location to any other grid location (i.e. there are no disconnected subsets of nodes in the graph),
	and b) there are loops.

	Since each node represents a grid location, each node can be connected to up to 4 neighbours in the
	top, right, bottom, and left directions respectively:

			node at (i,j-1)
				^
				|
	(node at i-1, j) <- node at (i,j) -> node at (i+1, j)
				|
				v
			node at (i,j+1)

	The graph is theredore stored as an adjacency list with size 1024 x 4, with one row per node in the
	graph, and 4 columns corresponding to the weight of an edge linking the node with each of its 4
	possible neighbours in the order towp, right, bottom, left (clockwise from top).

	Since all we care is whether nodes are connected. Weights will be either 0 or 1, if the weight is
	1, then the neighbouring nodes are connected, if the weight is 0, they are not. For example, if

	graph[i][0] = 0
	graph[i][1] = 1
	graph[i][2] = 0
	graph[i][3] = 1

	then node i is connected to the right and left neighbours, but not to top or bottom.

	The index in the graph for the node corresponding to grid location (x,y) is

	index = x + (y*size_X) 		or in this case		index = x + (y*32)

	Conversely, if you have the index and want to figure out the grid location,

	x = index % size_X		or in this case		x = index % 32
	y = index / size_Y		or in this case		y = index / 32

	(all of the above are *integer* operations!)

	A path is a sequence of (x,y) grid locations. We store it using an array of size
	1024 x 2 (since there are 1024 locations, this is the maximum length of any
	path that visits locations only once).

	Agent locations are coordinate pairs (x,y)

   Arguments:front++
		gr[graph_size][4]   - This is an adjacency list for the maze
		path[graph_size][2] - An initially empty path for your code to fill.
				      In this case, empty means all entries are initially -1
		visit_order[size_X][size_Y]	- An array in which your code will store the
						  *order* in which grid locations were
						  visited during search. For example, while
						  doing BFS, the initial location is the
						  start location, it's visit order is 1.
						  Then the search would expand the immediateprev[cell_x + ((cell_y-1) * size_X)] = cell_index;
						  neighbours of the i  printf("TotalSTEPS!! %d\n", totalSteps);
  totalSteps++;nitial node in some order,
						  these would get a visit order of 2, 3, 4, and
						  5 respectively, and so on.
visit_order[cell_x][cell_y]
						  This array will bgr[cell_index][0]e used to display the search
						  pattern generated by each search method.

		cat_loc[10][2], cats   - Location of cats and number of cats (we can have at most 10,
					 but there can be fewer). Only valid cat locations are 0 to (cats-1)
		cheese_loc[10][2], cheeses - Location and number of cheese chunks (again at most 10,
					     but possibly fewer). Valid locations are 0 to (cheeses-1)
		mouse_loc[1][2] - Mouse location - there can be only one!
		mode - Search mode selection:
					mode = 0 	- BFS
					mode = 1	- DFS
					mode = 2	- A*

		(*heuristic)(int x, int y, int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[10][2], int cats, int cheeses)
				- This is a pointer to one of the heuristic functions you will implement, either H_cost()
				  or H_cost_nokitty(). The driver in AI_search_core_GL will pass the appropriate pointer
				  depending on what search the user wants to run.

				  If the mode is 0 or 1, this pofront++inter is NULL

				  * How to call the heuristic function from within this function : *
					- Like any other function:
						h = heuristic( x, y, cat_loc, cheese_loc, mouse_loc, cats, cheeses);

   Return values:
		Your search code will directly update data passed-in as arguments:

		- path[graph_size][2]	: Your search code will update this array to contain the path from
					  the mouse to one of the cheese chunks. The order matters, so
					  path[0][:] must be the mouse's current location, path[1][:]
					  is the next move for the mouse. Each successive row will contain
					  the next move toward the cheese, and the path ends at a location
					  whose coordinates correspond to one of the cheese chunks.
					  Any entries beyond that must remain set to -1
		- visit_order[size_X][size_Y] 	:  Your search code will update this array to contain the
						   order in which each location in the grid was expanded
						   during search. This means, when that particular location
						   was checked for being a goal, and if not a goal, had its
						   neighbours added as candidates for future expansion (in
						   whatever order is dictated by the search mode).

						   Note that since there are 1024 locations, the values in
						   this array must always be in [0, 1023]. The driver code
						   will then display search order as a yellow-colored
						   brightness map where nodes expanded earlier will look850
						   brighter.

		* Your code MUST NOT modify the locations or numbers of cats and/or cheeses, the graph,
	     	  or the location of the mouse - if you try, the driver code will know it *

		That's that, now, implement your solution!
 */

 /********************************************************************************************************
 *
 * TO DO:	Implement code to carry out the different types of search depending on the
 *		mode.
 *
 *		You can do this by writing code within this single function (and being clever about it!)
 *		Or, you can use this function as a wrapper that calls a different search function
 *		 (BFS, DFS, A*) depending on the mode. Note that in the latter case, you will have
 *	    	  to inform your A* function somehow of what heuristic it's supposed to use.
 *
 *		Visiting Order: When adding the neighbours of a node to your list of candidates for
 *				expansion, do so in the order top, right, bottom, left.
 *
 *		NOTE: Your search functions should be smart enough to not choose a path that goes
 *		      through a cat! this is easily done without any heuristics.
 *
 *		How you design your solution is up to you. But:
 *
 *		- Document your implementation by adding concise and clear comments in this file
 *		- Document your design (how you implemented the solution, and why) in the report
 *
 ********************************************************************************************************/

  int mouse_x = mouse_loc[0][0];
  int mouse_y = mouse_loc[0][1];
  int visit_counter = 0;
  int path_counter = 0;
  int cell_index, cell_x, cell_y;
  bool catFound = false;
  int visited[size_X][size_Y];
  int visited2[size_X][size_Y];
  /*for(int x = 0; x < graph_size; x++) {
    path[x][0] = -1;
    path[x][1] = -1;
  }*/
  //Reset visited and visit_order values for every single iteration of the searches.
  for(int x = 0; x < size_X; x++) {
    for (int y = 0; y < size_Y; y ++) {
      visit_order[x][y] = 0;
      visited[x][y] = -99;
      visited2[x][y] = -2;
    }
  }
  visited[mouse_x][mouse_y] = -1;

//BFS
if(mode == 0) {
  //Store cell_index within queue, first one being the intial mouse location.


  
  // Test stuff
  int t = steps_to_cat(mouse_x, mouse_y, cat_loc[0][0], cat_loc[0][1], gr, visited2, -1);
  printf("S: %d\n", t);
  while(true);
  //End Test Stuff

  enqueue(mouse_x + (mouse_y * size_X));
  while(!isQueueEmpty()){
    cell_index = dequeue();
    cell_x = cell_index % size_X;
    cell_y = cell_index / size_Y;
    visit_counter++;
    visit_order[cell_x][cell_y] = visit_counter;
    //Check whether any cheese is at the location of the current expanded cell.
    for(int x = 0; x < cheeses; x++) {
      if(cell_x == cheese_loc[x][0] && cell_y == cheese_loc[x][1]) {
        //Count the number of steps in the path.
        for(int at=cell_index; at != -1; at=visited[at % size_X][at / size_Y]) {
          path_counter++;
        }
        printf("%d\n", path_counter);
        for(int at=cell_index; at != -1; at=visited[at % size_X][at / size_Y]) {
          path_counter--;
          path[path_counter][0] = at % size_X;
          path[path_counter][1] = at / size_Y;
        }
        emptyQueue();
        printf("V: %d\n", visit_counter);
        return;
      }
    }
    //Check if the cell above is not visited and is connected to the current cell.
    if(gr[cell_index][0] == 1 && visited[cell_x][cell_y-1] == -99) {
      //Check if the cell above has a cat in it.
      for(int x = 0; x < cats; x++) {
        if(cell_x == cat_loc[x][0] && cell_y - 1 == cat_loc[x][1]) {
          catFound = true;
        }
      }
      //If the cell above doesn't have a cat, only then add it to the BFS queue.
      if(!catFound) {
        enqueue(cell_x + ((cell_y-1) * size_X));
        visited[cell_x][cell_y-1] = cell_index;
      }
      catFound = false;
    }
    //Check if the cell to the right is not visited and is connected to the current cell.
    if(gr[cell_index][1] == 1 && visited[cell_x+1][cell_y] == -99) {
      //Check if the cell to the right has a cat in it.
      for(int x = 0; x < cats; x++) {
        if(cell_x + 1 == cat_loc[x][0] && cell_y == cat_loc[x][1]) {
          catFound = true;
        }
      }
      //If the cell to the right doesn't have a cat, only then add it to the BFS queue.
      if(!catFound) {
        enqueue(cell_x + 1 + (cell_y * size_X));
        visited[cell_x+1][cell_y] = cell_index;
      }
      catFound = false;
    }
    //Check if the cell below is not visited and is connected to the current cell.
    if(gr[cell_index][2] == 1 && visited[cell_x][cell_y+1] == -99) {
      //Check if the cell below has a cat next_stepin it.
      for(int x = 0; x < cats; x++) {
        if(cell_x == cat_loc[x][0] && cell_y + 1 == cat_loc[x][1]) {
          catFound = true;
        }
      }
      //If the cell below doesn't have a cat, only then add it to the BFS queue.
      if(!catFound) {
        enqueue(cell_x + ((cell_y+1) * size_X));
        visited[cell_x][cell_y+1] = cell_index;
      }
      catFound = false;
    }
    //Check if the cell to the left is not visited and is connected to the current cell.
    if(gr[cell_index][3] == 1 && visited[cell_x-1][cell_y] == -99) {
      //Check if the cell to the left has a cat in it.
      for(int x = 0; x < cats; x++) {
        if(cell_x - 1 == cat_loc[x][0] && cell_y == cat_loc[x][1]) {
          catFound = true;
        }
      }
      //If the cell to the left doesn't have a cat, only then add it to the BFS queue.
      if(!catFound) {
        enqueue(cell_x - 1 + (cell_y * size_X));
        visited[cell_x-1][cell_y] = cell_index;
      }
      catFound = false;
    }
  }
} else if (mode == 1) {
  //-------- DFS --------//
  push(mouse_x + (mouse_y * size_X));
  while(!isStackEmpty()){
    cell_index = pop();
    cell_x = cell_index % size_X;
    cell_y = cell_index / size_Y;
    visit_counter++;
    visit_order[cell_x][cell_y] = visit_counter;
    for(int x = 0; x < cheeses; x++) {
      if(cell_x == cheese_loc[x][0] && cell_y == cheese_loc[x][1]) {
        for(int at=cell_index; at != -1; at=visited[at % size_X][at / size_Y]) {
          path_counter++;
        }
        for(int at=cell_index; at != -1; at=visited[at % size_X][at / size_Y]) {
          path_counter --;
          path[path_counter][0] = at % size_X;
          path[path_counter][1] = at / size_Y;
          printf("(%d, %d) <- ", at % size_X, at / size_Y);
        }
        emptyStack();
        return;
      }
    }
    if(gr[cell_index][0] == 1 && visited[cell_x][cell_y-1] == -99) {
      for(int x = 0; x < cats; x++) {
        if(cell_x == cat_loc[x][0] && cell_y - 1 == cat_loc[x][1]) {
          catFound = true;
        }
      }
      if(!catFound) {
        push(cell_x + ((cell_y-1) * size_X));
        visited[cell_x][cell_y-1] = cell_index;
      }
      catFound = false;
    }
    if(gr[cell_index][1] == 1 && visited[cell_x+1][cell_y] == -99) {
      for(int x = 0; x < cats; x++) {
        if(cell_x + 1 == cat_loc[x][0] && cell_y == cat_loc[x][1]) {
          catFound = true;
        }
      }
      if(!catFound) {
        push(cell_x + 1 + (cell_y * size_X));
        visited[cell_x+1][cell_y] = cell_index;
      }
      catFound = false;
    }
    if(gr[cell_index][2] == 1 && visited[cell_x][cell_y+1] == -99) {
      for(int x = 0; x < cats; x++) {
        if(cell_x == cat_loc[x][0] && cell_y + 1 == cat_loc[x][1]) {
          catFound = true;
        }
      }
      if(!catFound) {
        push(cell_x + ((cell_y+1) * size_X));
        visited[cell_x][cell_y+1] = cell_index;
      }
      catFound = false;
    }
    if(gr[cell_index][3] == 1 && visited[cell_x-1][cell_y] == -99) {
      for(int x = 0; x < cats; x++) {
        if(cell_x - 1 == cat_loc[x][0] && cell_y == cat_loc[x][1]) {
          catFound = true;
        }
      }
      if(!catFound) {
        push(cell_x - 1 + (cell_y * size_X));
        visited[cell_x-1][cell_y] = cell_index;
      }
      catFound = false;
    }
  }
} else if (mode == 2) {
  //-----A*-----//
  pEnqueue(mouse_x + (mouse_y * size_X), heuristic(mouse_x, mouse_y, cat_loc, cheese_loc, mouse_loc, cats, cheeses, gr));
  while(!isPQueueEmpty()){
    cell_index = pDequeue();
    cell_x = cell_index % size_X;
    cell_y = cell_index / size_Y;
    visit_counter++;
    visit_order[cell_x][cell_y] = visit_counter;
    for(int x = 0; x < cheeses; x++) {
      if(cell_x == cheese_loc[x][0] && cell_y == cheese_loc[x][1]) {
        for(int at=cell_index; at != -1; at=visited[at % size_X][at / size_Y]) {
          path_counter++;
        }
        for(int at=cell_index; at != -1; at=visited[at % size_X][at / size_Y]) {
          path_counter --;
          path[path_counter][0] = at % size_X;
          path[path_counter][1] = at / size_Y;
        }
        printf("(%d, %d) ", path[path_counter][0], path[path_counter][1]);
        printf("%d\n", visit_counter);
        emptyPQueue();
        return;
      }
    }
    if(gr[cell_index][0] == 1 && visited[cell_x][cell_y-1] == -99) {
      for(int x = 0; x < cats; x++) {
        if(cell_x == cat_loc[x][0] && cell_y - 1 == cat_loc[x][1]) {
          catFound = true;
        }
      }
      if(!catFound) {
        pEnqueue(cell_x + ((cell_y-1) * size_X), heuristic(cell_x, cell_y-1, cat_loc, cheese_loc, mouse_loc, cats, cheeses, gr));
        visited[cell_x][cell_y-1] = cell_index;
      }
      catFound = false;
    }
    if(gr[cell_index][1] == 1 && visited[cell_x+1][cell_y] == -99) {
      for(int x = 0; x < cats; x++) {
        if(cell_x + 1 == cat_loc[x][0] && cell_y == cat_loc[x][1]) {
          catFound = true;
        }
      }
      if(!catFound) {
        pEnqueue(cell_x + 1 + (cell_y * size_X), heuristic(cell_x+1, cell_y, cat_loc, cheese_loc, mouse_loc, cats, cheeses, gr));
        visited[cell_x+1][cell_y] = cell_index;
      }
      catFound = false;
    }
    if(gr[cell_index][2] == 1 && visited[cell_x][cell_y+1] == -99) {
      for(int x = 0; x < cats; x++) {
        if(cell_x == cat_loc[x][0] && cell_y + 1 == cat_loc[x][1]) {
          catFound = true;
        }
      }
      if(!catFound) {
        pEnqueue(cell_x + ((cell_y+1) * size_X), heuristic(cell_x, cell_y+1, cat_loc, cheese_loc, mouse_loc, cats, cheeses, gr));
        visited[cell_x][cell_y+1] = cell_index;
      }
      catFound = false;
    }
    if(gr[cell_index][3] == 1 && visited[cell_x-1][cell_y] == -99) {
      for(int x = 0; x < cats; x++) {
        if(cell_x - 1 == cat_loc[x][0] && cell_y == cat_loc[x][1]) {
          catFound = true;
        }
      }
      if(!catFound) {
        pEnqueue(cell_x - 1 + (cell_y * size_X), heuristic(cell_x-1, cell_y, cat_loc, cheese_loc, mouse_loc, cats, cheeses, gr));
        visited[cell_x-1][cell_y] = cell_index;
      }
      catFound = false;
    }
  }
}
printf("No path found, standing still.\n============\n");
path[0][0] = mouse_x;
path[0][1] = mouse_y;
path[1][0] = mouse_x;
path[1][1] = mouse_y;
}

//Queue
bool isQueueEmpty() {
  if(queueItemCount == 0) {
    front = 0;
    back = -1;
  }
  return queueItemCount == 0;
}

void enqueue(int cell_index) {
    if(back == graph_size-1) {
      back = -1;
    }
    back++;
    queue[back] = cell_index;
    queueItemCount++;
}

int dequeue() {
  int cell_index = queue[front];
  front++;
  queueItemCount--;
   if(front == graph_size) {
      front = 0;
   }
   return cell_index;
}

void emptyQueue() {
  while(!isQueueEmpty()){
    dequeue();
  }
}

/*
void pEnqueue(int cell_index, int heu) {
    if(back == graph_size-1) {
      back = -1;
    }
    back++;
    int x = back;
    for (; x > front && heu < pQueue[x-1][1]; x--) {
      pQueue[x][0] = pQueue[x-1][0];
      pQueue[x][1] = pQueue[x-1][1];
    }
    pQueue[x][0] = cell_index;
    pQueue[x][1] = heu;
    queueItemCount++;
}*/

//Priority Queue
bool isPQueueEmpty() {
  return pQueueItemCount == 0;
}

void pEnqueue(int cell_index, int heu) {
  int i = pQueueItemCount + pfront - 1;
  for (; (i > pfront && pQueue[i][1] > heu); i--) {
    pQueue[i+1][0] = pQueue[i][0];
    pQueue[i+1][1] = pQueue[i][1];
  }
  pQueue[i+1][0] = cell_index;
  pQueue[i+1][1] = heu;
  pQueueItemCount++;
}

int pDequeue() {
  int cell_index = pQueue[pfront][0];
  pfront++;
  if(pfront == graph_size) {
    pfront = 0;
  }
  pQueueItemCount--;
  return cell_index;
}

void emptyPQueue() {
  while (!isPQueueEmpty()){
    pDequeue();
  }
}

//Stack
bool isStackEmpty() {
   return top == -1;
}

int pop() {
   int cell_index = stack[top];
   top --;
   return cell_index;
}

void push(int cell_index) {
    top++;
    stack[top] = cell_index;
}

void emptyStack() {
  while(!isStackEmpty()){
    pop();
  }
}

int H_cost(int x, int y, int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, double gr[graph_size][4])
{
 /*
	This function computes and returns the heuristic cost for location x,y.
	As discussed in lecture, this means estimating the cost of getting from x,y to the goal.
	The goal is cheese. Which cheese is up to you.
	Whatever you code here, your heuristic must be admissible.

	Input arguments:

		x,y - Location for which this function will compute a heuristic search cost
		cat_loc - Cat locations
		cheese_loc - Cheese locations
		mouse_loc - Mouse location
		cats - # of cats
		cheeses - # of cheesesisQueueEmpty
		gr - The graph's adjacency list for the maze

		These arguments are as described in the search() function above
 */
  int small_h = graph_size;
  for (int i = 0;i < cheeses;i++) {
    int curr_h = (pow(cheese_loc[i][0] - x, 2) + pow(cheese_loc[i][1] - y, 2));
    if (small_h > curr_h) {
      small_h = curr_h;
    }
  }
  return (pow(x-mouse_loc[0][0], 2) + pow(y-mouse_loc[0][1], 2)) + small_h;
}

int H_cost_nokitty(int x, int y, int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, double gr[graph_size][4])
{
 /*
	This function computes and returns the heuristic cost for location x,y.
	As discussed in lecture, this means estimating the cost of getting from x,y to the goal.
	The goal is cheese.

	However - this time you want your heuristic function to help the mouse avoid being eaten.
	Therefore: You have to somehow incorporate knowledge of the cats' locations into your
	heuristic cost estimate. How well you do this will determine how well your mouse behaves
	and how good it is at escaping kitties.

	This heuristic *does not have to* be admissible.

	Input arguments have the same meaning as in the H_cost() function above.
 */
  int small_h = graph_size;
  for (int i = 0;i < cheeses;i++) {
    int curr_h = (pow(cheese_loc[i][0] - x, 2) + pow(cheese_loc[i][1] - y, 2));
    if (small_h > curr_h) {
      small_h = curr_h;
    }
  }
  return (pow(x-mouse_loc[0][0], 2) + pow(y-mouse_loc[0][1], 2)) + small_h ;


}

int steps_to_cat(int curr_x, int curr_y, int cat_x, int cat_y, double gr[graph_size][4],int visited2[size_X][size_Y], int prev) {
  int cell_index = curr_x + (curr_y * size_X);
  visited2[curr_x][curr_y] = prev;
  if (curr_x == cat_x && curr_y == cat_y) {
    printf("exit\n");
    int steps = 0;
    for (int p=cell_index; p != -1; p=visited2[p % size_X][p / size_Y]) {
      printf("%d: (%d, %d)\n", steps, p % size_X, p / size_Y);
      steps++;
    }
    return steps;
  } else {
    if (gr[cell_index][0] == 1 && visited2[curr_x][curr_y-1] == -2) {
      enqueue(curr_x + ((curr_y - 1) * size_X));
      visited2[curr_x][curr_y-1] = cell_index;
    }
    if (gr[cell_index][1] == 1 && visited2[curr_x+1][curr_y] == -2) {
      enqueue((curr_x + 1) + (curr_y * size_X));
      visited2[curr_x+1][curr_y] = cell_index;
    }
    if (gr[cell_index][2] == 1 && visited2[curr_x][curr_y+1] == -2) {
      enqueue(curr_x + ((curr_y + 1) * size_X));
      visited2[curr_x][curr_y+1] = cell_index;
    }
    if (gr[cell_index][3] == 1 && visited2[curr_x-1][curr_y] == -2) {
      enqueue((curr_x - 1) + (curr_y * size_X));
      visited2[curr_x-1][curr_y] = cell_index;
    }
    int next_cell = dequeue();
    steps_to_cat(next_cell % size_X, next_cell / size_Y, cat_x, cat_y, gr, visited2, cell_index);
    
  }
}


