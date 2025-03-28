#include <iostream>
#include <array>
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include <utility>
#include <unordered_map>
#include <vector>
#include <algorithm>

const std::string Paper = std::string(2,char(176));
const std::string Scissors = std::string(2,char(206));
const std::string Rock = std::string(2,char(219));//'R';
const std::string Space = std::string(2,' ');

const int rows = 38;
const int columns = 76;

const int playerCount = 40;

struct Player{
    int x;
    int y;
    std::string state;

    Player(int x_val, int y_val, std::string state_val){
        x = x_val;
        y = y_val;
        state = state_val;
    }
    Player(){
        x = 0;
        y = 0;
        state = 'O';    
    }
};

void createValues(std::array<std::array<std::string, columns>, rows> &matrix, Player playerList[]);

void placePlayers(std::array<std::array<std::string, columns>, rows> &matrix, Player playerList[]);

void printMatrix(const std::array<std::array<std::string, columns>, rows> matrix);

bool Winners(Player playerList[]);

void battleBetween(std::array<std::array<std::string, columns>, rows> &matrix, Player playerList[]);

void movePlayers(std::array<std::array<std::string, columns>, rows> &matrix, Player playerList[]);

int main()
{
    std::srand(std::time(0));
    std::cout<<"\033[2J";
    std::cout << "\033[?25l";

    std::array<std::array<std::string, columns>, rows> matrix;
    Player playerList[playerCount];

    createValues(matrix, playerList);
    placePlayers(matrix, playerList);

    while(!Winners(playerList)){
        battleBetween(matrix, playerList);
        printMatrix(matrix);
        Sleep(500);

        movePlayers(matrix, playerList);
        printMatrix(matrix);
        Sleep(500/2);
    }

    system("pause");
}

void createValues(std::array<std::array<std::string, columns>, rows> &matrix, Player playerList[]){
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < columns; j++){
            matrix[i][j] = Space;
        }
    }

    for(int i = 0; i < playerCount; i++){
        int x = rand() % columns;
        int y = rand() % rows;

        while(matrix[y][x] != Space){
            x = rand() % columns;
            y = rand() % rows;    
        }
        std::string state;

        switch (rand()%3){
            case 0:state = Paper;break;
            case 1:state = Scissors;break;
            case 2:state = Rock;break;
        }
        playerList[i] = Player(x ,y , state); 
    }
}

void placePlayers(std::array<std::array<std::string, columns>, rows> &matrix, Player playerList[]){
    for(int i = 0; i < playerCount; i++){
        int x = playerList[i].x;
        int y = playerList[i].y;

        matrix[y][x] = playerList[i].state;
    }
}

void printMatrix(const std::array<std::array<std::string, columns>, rows> matrix){
    std::cout << "\033[H";
    for(int i = 0; i < (columns+1)*2; i++){
        std::cout<<'-';
    }std::cout<<'\n';

    for(int i = 0; i < rows; i++){
        std::cout<<'|';
        for(int j = 0; j < columns; j++){
            std::cout<<matrix[i][j];
        }std::cout<<"|\n";
    }
    for(int i = 0; i < (columns+1)*2; i++){
        std::cout<<'-';
    }
}

void battleBetween(std::array<std::array<std::string, columns>, rows> &matrix, Player playerList[]){
    for(int i = 0; i < playerCount; i++){
        std::unordered_map<std::string, std::string> winners = {
            {Scissors, Rock}, {Paper, Scissors},{Rock, Paper}
        };

        int x = playerList[i].x;
        int y = playerList[i].y;

        std::array<std::pair<int, int>, 8> coords = {
            std::make_pair(y-1, x-1), std::make_pair(y-1, x), std::make_pair(y-1, x+1),
            std::make_pair(y, x-1), std::make_pair(y, x+1),
            std::make_pair(y+1, x-1), std::make_pair(y+1, x), std::make_pair(y+1, x+1)
        };

        for(auto pair: coords){
            if(matrix[y][x] != matrix[pair.first][pair.second] and (pair.first > 0 and pair.first < rows - 1) and (pair.second > 0 and pair.second < columns - 1)){
                if(winners.count(matrix[pair.first][pair.second]) == 1){
                    matrix[pair.first][pair.second] = matrix[y][x];
                    break;
                }
            }    
        }
    }
}

bool Winners(Player playerList[]){
    int paper = 0;
    int scissors = 0;
    int rock = 0;

    for(int i = 0; i < playerCount; i++){
        if(playerList[i].state == Paper){paper++;}
        else if (playerList[i].state == Scissors){scissors++;}
        else if (playerList[i].state == Rock){rock++;}
    }
    return paper == playerCount || scissors == playerCount || rock == playerCount;
}

void movePlayers(std::array<std::array<std::string, columns>, rows> &matrix, Player playerList[]){
    for(int i = 0; i < playerCount; i++){
        int x = playerList[i].x;
        int y = playerList[i].y;

        std::array<std::pair<int, int>, 8> coords = {
            std::make_pair(y-1, x-1), std::make_pair(y-1, x), std::make_pair(y-1, x+1),
            std::make_pair(y, x-1), std::make_pair(y, x+1),
            std::make_pair(y+1, x-1), std::make_pair(y+1, x), std::make_pair(y+1, x+1)
        };

        std::vector<int> pattern = {0,1,2,3,4,5,6,7};
        std::random_shuffle(pattern.begin(), pattern.end());

        int index = 0;

        while(index < 8){
            int tempx = coords[pattern[index]].second;
            int tempy = coords[pattern[index]].first;

            if(tempx >= 0 && tempx < columns - 1&& tempy >= 0 && tempy < rows - 1 && matrix[tempy][tempx] == Space){
                std::swap(matrix[y][x], matrix[tempy][tempx]);

                // Update player's coordinates
                playerList[i].x = tempx;
                playerList[i].y = tempy;
                break;
            }
            index++;
        }
    }
}

