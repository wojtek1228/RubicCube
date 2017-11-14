#include <stdio.h>
#include <stdlib.h>
#include "search.h"
#include <string>
#include<iostream>

using namespace std;


char rotationLookUp(char up, char front, char next){
    if(up=='U' && front=='B' && (next=='F' || next=='B')) return 'X';
    if(up=='U' && front=='F' && (next=='F' || next=='B')) return 'x';
    if(up=='U' && front=='R' && (next=='F' || next=='B')) return 'Y';
    if(up=='U' && front=='L' && (next=='F' || next=='B')) return 'y';
    
    if(up=='U' && front=='L' && (next=='R' || next=='L')) return 'X';
    if(up=='U' && front=='R' && (next=='R' || next=='L')) return 'x';
    if(up=='U' && front=='B' && (next=='R' || next=='L')) return 'Y';
    if(up=='U' && front=='F' && (next=='R' || next=='L')) return 'y';
    
    if(up=='F' && front=='D' && (next=='U' || next=='D')) return 'X';
    if(up=='F' && front=='U' && (next=='U' || next=='D')) return 'x';
    if(up=='F' && front=='L' && (next=='U' || next=='D')) return 'Y';
    if(up=='F' && front=='R' && (next=='U' || next=='D')) return 'y';
    
    if(up=='F' && front=='L' && (next=='R' || next=='L')) return 'X';
    if(up=='F' && front=='R' && (next=='R' || next=='L')) return 'x';
    if(up=='F' && front=='U' && (next=='R' || next=='L')) return 'Y';
    if(up=='F' && front=='D' && (next=='R' || next=='L')) return 'y';
    
    if(up=='R' && front=='D' && (next=='U' || next=='D')) return 'X';
    if(up=='R' && front=='U' && (next=='U' || next=='D')) return 'x';
    if(up=='R' && front=='F' && (next=='U' || next=='D')) return 'Y';
    if(up=='R' && front=='B' && (next=='U' || next=='D')) return 'y';
    
    if(up=='R' && front=='B' && (next=='F' || next=='B')) return 'X';
    if(up=='R' && front=='F' && (next=='F' || next=='B')) return 'x';
    if(up=='R' && front=='D' && (next=='F' || next=='B')) return 'Y';
    if(up=='R' && front=='U' && (next=='F' || next=='B')) return 'y';
    
    return NULL;
}

char oppositeSide(char side){
    if(side=='U') return 'D';
    if(side=='D') return 'U';
    if(side=='F') return 'B';
    if(side=='B') return 'F';
    if(side=='R') return 'L';
    if(side=='L') return 'R';
    
    return NULL;
}

string translateForRobot(string sol){
    char rotation;
    char act_pos[2]={'U','F'};
    string solutionForRobot="";
    
    for (int i=0;i<sol.size();i++)
        if (sol[i] == 'U' || sol[i] == 'F' || sol[i] == 'R' || sol[i] == 'D' || sol[i] == 'B' || sol[i] == 'L'){
            rotation=rotationLookUp(act_pos[0],act_pos[1],sol[i]);
            
            if(rotation=='X') act_pos[1]=act_pos[0];
            if(rotation=='x') act_pos[1]=oppositeSide(act_pos[0]);
            act_pos[0]=(sol[i] == 'U' || sol[i] == 'F'|| sol[i] == 'R') ? sol[i] : oppositeSide(sol[i]);
            
            solutionForRobot+=rotation;
            if(rotation) solutionForRobot+=' ';
            solutionForRobot+=(sol[i] == 'U' || sol[i] == 'F'|| sol[i] == 'R') ? 'U' : 'D';
        } else solutionForRobot+=sol[i];
    
    return solutionForRobot;
}
