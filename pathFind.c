/*  pathFind.c
    Pathfindering program using A*
    By: James Arthur Anderson
    Student ID #: 1013076
    Date: Sept. 9, 2018
*/
#include"DnD.h"
int main (){
    int startX;
    int startY;
    int targetX;
    int targetY;
    char mapName[256];
    FILE *fp = NULL;
   
    printf("Please enter the map name:");
    scanf("%s",mapName);
    fp = fopen(mapName,"rb");
    if (fp == NULL){
        fprintf(stderr,"The map '%s' failed to open\n",mapName);
        exit(1);
    }
    showMap(fp);
    printf("Please your starting x cordinates: ");
    scanf("%d",&startX);
    printf("Please your starting y cordinates: ");
    scanf("%d",&startY);
    printf("Please your target x cordinates: ");
    scanf("%d",&targetX);
    printf("Please your target y cordinates: ");
    scanf("%d",&targetY);

    pathFind(startX, startY, targetX, targetY, fp);

    fclose(fp);


    return 0;
}
