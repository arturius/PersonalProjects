/*  mapMaker.c
    Program to make, edit, and view maps
    By: James Arthur Anderson
    Student ID #: 1013076
    Date: Jul. 12, 2018
*/

#include"DnD.h"

int main(){
    int mapXMax = 2;
    int mapYMax = 2;
    int mapXSelect = 0;
    int continueLoop = 0;
    int mapYSelect = 0;
    int userSelection = 0;
    int i,j = 0;
    char mapName[256];
    FILE *fp = NULL;
    map temp;
    time_t t;
    int holding;
    srand((unsigned)time(&t));
    while (userSelection<1 || userSelection >3 ){
        printf("Please select a Function by pressing the apropriate number.\nPress 1 to view a map.\nPress 2 to edit a map.\nPress 3 to create a new map.\n");
        scanf("%d",&userSelection);
    }
    if (userSelection == 1){
       /* showMap();*/
        printf("Please enter the map name:");
        scanf("%s",mapName);
        fp = fopen(mapName,"rb");
        if (fp == NULL){
            fprintf(stderr,"The map '%s' failed to open\n",mapName);
            exit(1);
        }
        showMap(fp);
        fclose(fp);
        printf ("Derp\n");
    }else if (userSelection == 2){
        /*Editing Map section*/
        printf("Please enter the map name:");
        scanf("%s",mapName);
        fp = fopen(mapName,"r+b");
        if (fp == NULL){
            fprintf(stderr,"The map '%s' failed to open\n",mapName);
            exit(1);
        }
        printf("showing Map\n");
        showMap (fp);
        fseek (fp, 0, SEEK_SET);
        fread (&mapXMax,sizeof(int),1,fp);
        fread (&mapYMax,sizeof(int),1,fp);
        printf ("xMax = %d, yMax = %d\n",mapXMax,mapYMax);
        printf("Please select an option:\nPress 1 to edit a specific coordinate on the map\n");
        printf("Press 2 to make and inpassible x border around the map\nPress 3 to randomise all elements in the map\n");
        scanf("%d",&userSelection);
        if (userSelection == 1){
            while (continueLoop == 0){
                printf("Please enter the X corordinate of the point you want to change:");
                scanf("%d",&(temp.xCord));
                printf("Please enter the Y corordinate of the point you want to change:");
                scanf("%d",&(temp.yCord));
                printf("Please enter the new values for the visual and weither or not it can be pased through\nVisual:");
                scanf("%d",&(temp.visual));
                printf("Passed Through (1 for yes; 0 for no):");
                scanf("%d",&(temp.pass));
                printf("X = %d, Y = %d, Visual = %d, Passed Through = %d\n",temp.xCord,temp.yCord,temp.visual,temp.pass);
                makeMapPoint(fp,mapXMax,mapYMax,temp);
                printf("showing Map\n");
                showMap (fp);
                printf("\nPress 0 to continue editing this map and any other number to exit: ");
                scanf("%d",&continueLoop);
            }
        } else if (userSelection == 2){
            temp.visual = 2;
            temp.pass = 1;
            for (i =0;i <mapYMax;i++){
                for (j = 0; j< mapXMax; j++){
                    if (i ==0 ||j == 0||i == (mapYMax-1)||j ==(mapXMax-1)){
                        temp.xCord = j;
                        temp.yCord = i;
                        makeMapPoint(fp,mapXMax,mapYMax,temp);
                    }
                }
            }
            showMap (fp);

        } else if (userSelection == 3){
            for (i =0;i <mapYMax;i++){
                for (j = 0; j< mapXMax; j++){
                    temp.xCord = j;
                    temp.yCord = i;
                    holding = rand()%8;
                    if (holding < 4){
                        temp.visual = holding;
                    } else { 
                        temp.visual = 0;
                    }
                    if (temp.visual == 1|| temp.visual == 2){
                        temp.pass = 0;
                    } else if (temp.visual == 0){
                        temp.pass = 1;
                    }else if (temp.visual == 3) {
                        temp.pass = 2;
                    }
                    makeMapPoint(fp,mapXMax,mapYMax,temp);
                    /*printf("X = %d, Y = %d, Visual = %d, Passed Through = %d\n",temp.xCord,temp.yCord,temp.visual,temp.pass);
                */
                }
            }
            showMap (fp);
        }
        fclose (fp);
    }else if (userSelection == 3){
        printf("Hello please input how large the map will be\n");
        printf("for the x demension :");
        scanf("%d", &mapXSelect);
        printf("for the y demension :");
        scanf("%d", &mapYSelect);
        newMap(mapXSelect,mapYSelect);
        printf("new map Created\n");
    }
    return 0;
}

