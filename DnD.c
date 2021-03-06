/*  DnD.c
    Main File.
    By: James Arthur Anderson
    Student ID #: 1013076
    Date: Jul. 12, 2018
*/

#include"DnD.h"

void makeMapPoint (FILE *fp, int xMax, int yMax, map input){
    if (input.xCord>xMax||input.yCord>yMax){
        fprintf(stderr,"\nmap point (%d,%d) exceeds x max of %d or y max of %d\n",input.xCord,input.yCord,xMax,yMax);
    }
    fseek(fp,(sizeof(int)*2)+ (sizeof(map)*input.xCord)+ ((sizeof(map)*xMax)*input.yCord) , SEEK_SET);
    
    fwrite(&input, sizeof(map),1,fp);


}

void newMap(int xMax,int yMax){
    int i =0;
    int mapSize = xMax*yMax;
    map intial[mapSize];
    FILE* outFile = NULL;
    char mapName [256];
    /*opening file*/
    printf("Please enter new map name (entering existing map name will overwite)\n");
    scanf ("%s",mapName);
    outFile = fopen(mapName, "wb");
    fwrite(&xMax,sizeof(int),1,outFile);
    fwrite(&yMax,sizeof(int),1,outFile);
    /*writing values to file*/
    for(i= 0; i < mapSize; ++i) {
        intial[i].xCord = i%xMax;
        intial[i].yCord = i/xMax;
        intial[i].visual= 0;
        intial[i].pass = 0;
        fwrite(&(intial[i]), sizeof(map), 1, outFile);
    }
    /*closing file*/
    fclose(outFile);
}

void loadMap (FILE *fp, int xMax, int yMax, map mapSelect[xMax][yMax]){
    int mapSize;
    int i;
    map holding;
    
    mapSize = xMax * yMax;
    /*printf("Loading Map\n");*/
    fseek (fp, sizeof(int)*2, SEEK_SET);
    /*printf("xmax =%d, ymax = %d, mapsize =%d\n",xMax,yMax, mapSize) ;   
    */
    for (i = 1 ; i<= mapSize; i++){
        /*printf("%d ",i);*/
        fread(&holding, sizeof(map),1,fp);
        mapSelect[holding.xCord][holding.yCord].xCord = holding.xCord ;
        mapSelect[holding.xCord][holding.yCord].yCord = holding.yCord ;
        mapSelect[holding.xCord][holding.yCord].visual = holding.visual ;
        mapSelect[holding.xCord][holding.yCord].pass = holding.pass ;
    /*    printf("xcord:%d, ycord:%d, visual:%d, pass:%d\n",holding.xCord, holding.yCord, holding.visual, holding.pass);
    */}
    printf("Loading Complete\n");
}

void showMap (FILE *fp){
    int i,j;
    /*header for copy / paste*/
    int xMax = 0;
    int yMax = 0;
    fseek(fp, 0, SEEK_SET);
    fread(&xMax,sizeof(int),1,fp);
    fread(&yMax,sizeof(int),1,fp);
    printf("x max is %d, ymax is %d\n",xMax,yMax);
    map mapSelect [xMax][yMax];

    loadMap(fp,xMax,yMax, mapSelect);
    for (i = 0; i<= xMax + 2; i++){
        printf("_");
    }
    printf ("\n");
    for (i = 0; i< yMax; i++){
        printf ("|");
        for (j = 0; j< xMax; j++){
            
            //make into switch statment
            if (mapSelect[j][i].visual == 0){
                printf(" ");
            } else if (mapSelect[j][i].visual == 1){
                printf("O");
            } else if (mapSelect[j][i].visual == 2){
                printf("X");
            } else if (mapSelect[j][i].visual == 3){
                printf("~");
            } else if (mapSelect[j][i].visual == 4){
                printf("T");
            } else if (mapSelect[j][i].visual == 5){
                printf("p");
            } else {
                printf("?");
            }
        }
        printf("|\n");
    }
    for (i = 0; i< xMax +2; i++){
        printf("_");
    }
    printf ("\n");
}
/*Pathfinding related functions*/

void pathFind(int startX, int startY, int targetX,int targetY, FILE *fpMap){
    char pause;
    int i,j;
    int xMax = 0;
    int yMax = 0;
    int xPos = startX;
    int yPos = startY;
    pathNode *lnkList, *tempNode, *activeNode;
    fseek(fpMap, 0, SEEK_SET);
    fread(&xMax,sizeof(int),1,fpMap);
    fread(&yMax,sizeof(int),1,fpMap);
    map mapSelect [xMax][yMax];
    lnkList = NULL;
    loadMap(fpMap,xMax,yMax, mapSelect);
    tempNode = newNode (startX,startY,targetX,targetY,NULL,0);
    addNode (&lnkList,tempNode);
    showNodes(&lnkList);
    activeNode = tempNode;
    while (activeNode->relativeDist != 0 ){
        printf("New sequence (%p): From:%p, Next:%p\nrelative Dist:%d, traveledDist:%d, totalDist:%d, cord (%d,%d)\n",activeNode,activeNode-> from,activeNode-> next,activeNode-> relativeDist,activeNode-> traveledDist,activeNode-> totalDist,activeNode-> xCord,activeNode-> yCord);
 
        /*go through all posible directions*/
        printf("Doing top left\n");
        xPos = activeNode->xCord -1;
        yPos = activeNode->yCord -1;
        if (xPos>=0 && xPos<xMax && yPos>=0 && yPos<yMax){
            if (mapSelect[xPos][yPos].pass != 0){
                printf("Pass %d\n", mapSelect[xPos][yPos].pass);
                if(checkNode (xPos,yPos, &lnkList)){
                    printf("node found at (%d,%d )\n",xPos,yPos);
                    tempNode = checkNode (xPos,yPos, &lnkList);
                    compareNodes(tempNode, (activeNode->traveledDist + mapSelect[xPos][yPos].pass),activeNode,&lnkList);
                }else{
                    printf("new node at (%d,%d )\n",xPos,yPos);
                    tempNode = newNode(xPos,yPos, targetX, targetY,activeNode,(activeNode->traveledDist + mapSelect[xPos][yPos].pass)) ; 
                }
                printf("sorting nodes\n");
                sortNodes(tempNode,&lnkList);
            }
        }

        printf("Doing top center\n");
        xPos = activeNode->xCord;
        yPos = activeNode->yCord -1;
        if (xPos>=0 && xPos<xMax && yPos>=0 && yPos<yMax){
            if (mapSelect[xPos][yPos].pass != 0){
//                showNodes(&lnkList);
                if(checkNode (xPos,yPos, &lnkList)){
                    printf("node found at (%d,%d )\n",xPos,yPos);
                    tempNode = checkNode (xPos,yPos, &lnkList);
                    compareNodes(tempNode, (activeNode->traveledDist + mapSelect[xPos][yPos].pass),activeNode,&lnkList);
                }else{
                    printf("new node at (%d,%d )\n",xPos,yPos);
                    tempNode = newNode(xPos,yPos, targetX, targetY,activeNode,(activeNode->traveledDist + mapSelect[xPos][yPos].pass)) ; 
                }
                
                sortNodes(tempNode,&lnkList);
            }
        }

        printf("Doing top right\n");
        xPos = activeNode->xCord +1;
        yPos = activeNode->yCord -1;
        if (xPos>=0 && xPos<xMax && yPos>=0 && yPos<yMax){
            if (mapSelect[xPos][yPos].pass != 0){
                if(checkNode (xPos,yPos, &lnkList)){
                    printf("node found at (%d,%d )\n",xPos,yPos);
                    tempNode = checkNode (xPos,yPos, &lnkList);
                    compareNodes(tempNode, (activeNode->traveledDist + mapSelect[xPos][yPos].pass),activeNode,&lnkList);
                }else{
                    printf("new node at (%d,%d )\n",xPos,yPos);
                    tempNode = newNode(xPos,yPos, targetX, targetY,activeNode,(activeNode->traveledDist + mapSelect[xPos][yPos].pass)) ; 
                }
                sortNodes(tempNode,&lnkList);
            }
        }

        printf("Doing center left\n");
        xPos = activeNode->xCord -1;
        yPos = activeNode->yCord;
        if (xPos>=0 && xPos<xMax && yPos>=0 && yPos<yMax){
            if (mapSelect[xPos][yPos].pass != 0){
                if(checkNode (xPos,yPos, &lnkList)){
                    printf("node found at (%d,%d )\n",xPos,yPos);
                    tempNode = checkNode (xPos,yPos, &lnkList);
                    compareNodes(tempNode, (activeNode->traveledDist + mapSelect[xPos][yPos].pass),activeNode,&lnkList);
                }else{
                    printf("new node at (%d,%d )\n",xPos,yPos);
                    tempNode = newNode(xPos,yPos, targetX, targetY,activeNode,(activeNode->traveledDist + mapSelect[xPos][yPos].pass)) ; 
                }
                sortNodes(tempNode,&lnkList);
            }
        }

        printf("Doing center right\n");
        xPos = activeNode->xCord +1;
        yPos = activeNode->yCord ;
        if (xPos>=0 && xPos<xMax && yPos>=0 && yPos<yMax){
            if (mapSelect[xPos][yPos].pass != 0){
                if(checkNode (xPos,yPos, &lnkList)){
                    printf("node found at (%d,%d )\n",xPos,yPos);
                    tempNode = checkNode (xPos,yPos, &lnkList);
                    compareNodes(tempNode, (activeNode->traveledDist + mapSelect[xPos][yPos].pass),activeNode,&lnkList);
                }else{
                    printf("new node at (%d,%d )\n",xPos,yPos);
                    tempNode = newNode(xPos,yPos, targetX, targetY,activeNode,(activeNode->traveledDist + mapSelect[xPos][yPos].pass)) ; 
                }
                sortNodes(tempNode,&lnkList);
            }
        }
        printf("Doing bottom left\n");
        xPos = activeNode->xCord -1;
        yPos = activeNode->yCord +1;
        if (xPos>=0 && xPos<xMax && yPos>=0 && yPos<yMax){
            if (mapSelect[xPos][yPos].pass != 0){
                if(checkNode (xPos,yPos, &lnkList)){
                    printf("node found at (%d,%d )\n",xPos,yPos);
                    tempNode = checkNode (xPos,yPos, &lnkList);
                    compareNodes(tempNode, (activeNode->traveledDist + mapSelect[xPos][yPos].pass),activeNode,&lnkList);
                }else{
                    printf("new node at (%d,%d )\n",xPos,yPos);
                    tempNode = newNode(xPos,yPos, targetX, targetY,activeNode,(activeNode->traveledDist + mapSelect[xPos][yPos].pass)) ; 
                }
                sortNodes(tempNode,&lnkList);
            }
        }
        printf("Doing bottom center\n");
        xPos = activeNode->xCord ;
        yPos = activeNode->yCord +1;
        if (xPos>=0 && xPos<xMax && yPos>=0 && yPos<yMax){
            if (mapSelect[xPos][yPos].pass != 0){
                if(checkNode (xPos,yPos, &lnkList)){
                    printf("node found at (%d,%d )\n",xPos,yPos);
                    tempNode = checkNode (xPos,yPos, &lnkList);
                    compareNodes(tempNode, (activeNode->traveledDist + mapSelect[xPos][yPos].pass),activeNode,&lnkList);
                }else{
                    printf("new node at (%d,%d )\n",xPos,yPos);
                    tempNode = newNode(xPos,yPos, targetX, targetY,activeNode,(activeNode->traveledDist + mapSelect[xPos][yPos].pass)) ; 
                }
                sortNodes(tempNode,&lnkList);
            }
        }

        printf("Doing bottom right\n");
        xPos = activeNode->xCord + 1;
        yPos = activeNode->yCord + 1;
        if (xPos>=0 && xPos<xMax && yPos>=0 && yPos<yMax){
            if (mapSelect[xPos][yPos].pass != 0){
                if(checkNode (xPos,yPos, &lnkList)){
                    printf("node found at (%d,%d )\n",xPos,yPos);
                    tempNode = checkNode (xPos,yPos, &lnkList);
                    compareNodes(tempNode, (activeNode->traveledDist + mapSelect[xPos][yPos].pass),activeNode,&lnkList);
                }else{
                    printf("new node at (%d,%d )\n",xPos,yPos);
                    tempNode = newNode(xPos,yPos, targetX, targetY,activeNode,(activeNode->traveledDist + mapSelect[xPos][yPos].pass)) ; 
                }
                
                sortNodes(tempNode,&lnkList);
            }
        }
        printf("\n");
        scanf("%c",pause);
        showNodes(&lnkList);
        activeNode = lnkList;
     /*end of cycle*/
    }
    showMap(fpMap);
    showPath(&lnkList, fpMap, xMax,yMax);
    deleteList(&lnkList);
}

int relLength(int xPos, int yPos, int targetX,int targetY){
    int length = - 1;
    length = (int)sqrt(pow((targetX-xPos),2)+pow((targetY-yPos),2));
    return length;
}
pathNode *newNode(int xPos, int yPos, int targetX, int targetY, pathNode *origin, int travDist){
    pathNode *new;
    
    new = malloc(sizeof(pathNode));
    new-> xCord = xPos;
    new-> yCord = yPos;
    new-> from = origin;
    new-> traveledDist = travDist;
    new-> relativeDist = relLength(xPos,yPos,targetX,targetY);
    new-> totalDist = new->traveledDist + new->relativeDist;
    new-> next = NULL;
    printf("new node created\n");    
    return new;
}

pathNode *popNode(pathNode **list){/*removes node from front of the list*/
    pathNode *item;
    
    item = *list;
    *list = item->next;
    item->next = NULL;
    
    return item;
}

pathNode *checkNode (int xPos, int yPos, pathNode **givenList){/*checks to see if a node with given coridnartes exists*/
    pathNode *list = *givenList; 
    printf("checknode\n");
    while (list){
        if (list-> xCord == xPos && list-> yCord == yPos){
            return list;
        }
        list= list->next;
        
    }
    return NULL;/*returns NULL if a node dosent exist and the node pointer if it does*/
}

void compareNodes(pathNode *foundNode, int travDist, pathNode *origin, pathNode **givenList){/*checks to see if a new origin will shorten the length of total distance travleed and will replace the origin if it does; Also pops the node from the array list*/
    pathNode *list = *givenList;    
    printf("TravDist:%d\n",travDist);
    if (foundNode-> traveledDist > travDist && foundNode-> totalDist > (travDist + foundNode->relativeDist)){
        foundNode-> from = origin;
        foundNode-> traveledDist = travDist;
    }
    /*removing node from list*/
    while (list){
        if (foundNode == list->next){
            list->next = foundNode->next;
            foundNode-> next = NULL;
        }
        list = list-> next;
    }
    showNodes(givenList);
}

void sortNodes(pathNode *changedNode, pathNode **givenList){/*compares a new or changed nodes total distance and places it in the corect positon in the linked list*/
     pathNode *list = *givenList;    
    printf("Starting to sort %p in '%p'\n",changedNode,list);
    pathNode *temp = NULL;
   /* if (*list == NULL){
        *list = changedNode;
        return;
    }*/
    while (list->next){
        printf("changed = %d, list =%d, next =%d\n",changedNode-> totalDist, list->totalDist, (list-> next)-> totalDist);
        
        if (changedNode-> totalDist > list-> totalDist && changedNode-> totalDist <= (list-> next)-> totalDist){
            printf("Condition 1 met\n");
            temp = list-> next;
            list->next = changedNode;
            changedNode->next = temp;
            return;
        }else if (changedNode-> totalDist <= list-> totalDist){
            printf("conditon 2 met\n");
            if (changedNode-> totalDist == list-> totalDist && changedNode->relativeDist > list->relativeDist){
                temp = list->next;
                list->next = changedNode;
                changedNode->next = temp;
                return;
            }
            addNode(givenList,changedNode);
            return;
        }else{
            //printf("increment conditioon met\n");
            list = list-> next;
        }
    }
    list->next = changedNode;
    changedNode-> next = NULL;
}

void addNode(pathNode **list, pathNode *toAdd){
    toAdd-> next = *list;
    *list = toAdd;
}

void deleteList (pathNode **list){
    pathNode *temp;
    while(*list){
        temp = popNode(list);
        deleteNode(&temp);
    }
}

void deleteNode (pathNode **toDelete){
    free(*toDelete);
}

void showPath(pathNode **givenList, FILE *fp, int xMax, int yMax){
    pathNode *list = *givenList; 
    while (list){
        printf("Path track cord: (%d,%d)\n",list-> xCord,list-> yCord);
        list = list->from;
    }
}

void showNodes(pathNode **givenList){
    pathNode *list = *givenList;    
    int i =0;
    printf("Showing Nodes\n");
    while (list){
        printf("List Node %d(%p): From:%p, Next:%p\nrelative Dist:%d, traveledDist:%d, totalDist:%d, cord (%d,%d)\n",i,list,list-> from,list-> next,list-> relativeDist,list-> traveledDist,list-> totalDist,list-> xCord,list-> yCord);
        i++;
        list = list-> next;
    }
}
