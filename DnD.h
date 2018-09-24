/*  DnD.h
    Header File.
    By: James Arthur Anderson
    Student ID #: 1013076
    Date: Jul. 12, 2018
*/
/*including of standard libraries*/
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>

#ifndef DND_H
#define DND_H
/*structs*/

typedef struct mapStruct {
    int xCord;
    int yCord;
    int visual;/*0 for ' ', 1 for 'O' , 2 for 'X', 3 for '~', 4 for 'T', 5 for 'p'*/
    int pass; /*0 for no 1 for yes*/
} map;

typedef struct pathFndNode pathNode;
struct pathFndNode{
    pathNode *from; /**/
    pathNode *next; /**/
    
    int relativeDist;
    int traveledDist;
    int totalDist;
    
    int xCord;
    int yCord;
};

/*Map related Functions*/

void makeMapPoint (FILE *fp, int xMax, int yMax, map input);
void loadMap (FILE *fp, int xMax, int yMax, map mapSelect[xMax][yMax]);
void newMap (int xMax, int yMax);
void showMap (FILE *fp);

/*Pathfinding related functions*/

void pathFind(int startX, int startY, int targetX,int targetY, FILE *fpMap);
int relLength(int xPos, int yPos, int targetX,int targetY);
pathNode *newNode(int xPos, int yPos, int targetX, int targetY, pathNode *origin, int travDist);
pathNode *popNode(pathNode **list);/*removes node from front of the list*/
pathNode *checkNode (int xPos, int yPos, pathNode **givenList);/*checks to see if a node with given coridnartes exists*/
void compareNodes(pathNode *foundNode, int travDist, pathNode *origin, pathNode **givenList);/*checks to see if a new origin will shorten the length of total distance travleed and will replace the origin if it does*/
void sortNodes(pathNode *changedNode, pathNode **givenList);/*compares a new or changed nodes total distance and places it in the corect positon in the linked list*/
void addNode(pathNode **giveList, pathNode *toAdd);
void deleteList (pathNode **list);
void deleteNode (pathNode **toDelete);

void showPath(pathNode **givenList, FILE *fp, int xMax, int yMax);

void showNodes(pathNode **givenList);

#endif

  
