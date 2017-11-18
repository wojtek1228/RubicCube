//  solver.cpp
//  RubicQube
//
//  Created by Wojciech on 18.11.2017.
//  Copyright © 2017 Wojciech. All rights reserved.
//

#include "solver.h"

char colorToLetterTab[6];
vector<int> programSequence;
string solverInput;
string movesForRobot;

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

void generateSolverInput()
{
    colorToLetterTab[cube_result[0].at<int>(1,1)] = 'L';
    colorToLetterTab[cube_result[1].at<int>(1,1)] = 'D';
    colorToLetterTab[cube_result[2].at<int>(1,1)] = 'R';
    colorToLetterTab[cube_result[3].at<int>(1,1)] = 'B';
    colorToLetterTab[cube_result[4].at<int>(1,1)] = 'U';
    colorToLetterTab[cube_result[5].at<int>(1,1)] = 'F';
    
    
    solverInput = "";
    
    //U
    for(int j = 0; j<3; j++)
        for(int k = 0; k<3; k++)
        {
            solverInput += colorToLetterTab[cube_result[4].at<int>(j,k)];
        }
    
    //R
    for(int j = 0; j<3; j++)
        for(int k = 0; k<3; k++)
        {
            solverInput += colorToLetterTab[cube_result[2].at<int>(j,k)];
        }
    
    //F
    for(int j = 0; j<3; j++)
        for(int k = 0; k<3; k++)
        {
            solverInput += colorToLetterTab[cube_result[5].at<int>(j,k)];
        }
    
    //D
    for(int j = 0; j<3; j++)
        for(int k = 0; k<3; k++)
        {
            solverInput += colorToLetterTab[cube_result[1].at<int>(j,k)];
        }
    
    //L
    for(int j = 0; j<3; j++)
        for(int k = 0; k<3; k++)
        {
            solverInput += colorToLetterTab[cube_result[0].at<int>(j,k)];
        }
    
    //B
    for(int j = 0; j<3; j++)
        for(int k = 0; k<3; k++)
        {
            solverInput += colorToLetterTab[cube_result[3].at<int>(j,k)];
        }
    
    
}

void rotateCubeResults()
{
    Mat pom1;
    rotate(cube_result[1], pom1, ROTATE_90_COUNTERCLOCKWISE);
    cube_result[1] = pom1;  //D
    
    Mat pom2;
    rotate(cube_result[3], pom2, ROTATE_180);
    cube_result[3] = pom2;   //B
    
    Mat pom3;
    rotate(cube_result[5], pom3, ROTATE_180);
    cube_result[5] = pom3;   //F
}

void translateMovesToPins()
{cout<<"start"<<endl;
    for(int i = 0; i<movesForRobot.size(); i++)
    {
        string temp ="";
        while(movesForRobot[i] != ' ')
        {
            temp +=movesForRobot[i];
            i++;
        }
        
        cout<<"...."<<temp<<endl;
        if(temp == "X") programSequence.push_back(1);
        else if(temp == "x") programSequence.push_back(2);
        else if(temp == "Y") programSequence.push_back(3);
        else if(temp == "y") programSequence.push_back(4);
        else if(temp == "U") programSequence.push_back(5);
        else if(temp == "U2") programSequence.push_back(6);
        else if(temp == "U'") programSequence.push_back(7);
        else if(temp == "D") programSequence.push_back(8);
        else if(temp == "D2") programSequence.push_back(9);
        else if(temp == "D'") programSequence.push_back(10);
    }
}

void solve()
{
    char pom[64];
    char *facelets = pom;
    strcpy(facelets, solverInput.c_str());
    char *sol = solution(
                         facelets,
                         24,
                         1000,
                         0,
                         "cache"
                         );
    if (sol == NULL) {
        puts("Unsolvable cube!");
    }
    else
    {
        cout<<"pocz: "<<sol<<endl;
        movesForRobot = translateForRobot(sol);
        cout<<movesForRobot;
        free(sol);
    }
}

void prepareSolverInputAndSolve()
{
    rotateCubeResults();
    generateSolverInput();
    solve();
    translateMovesToPins();
    int a;

}








