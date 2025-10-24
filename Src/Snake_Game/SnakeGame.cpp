#include <iostream>
#include <vector>
#include <conio.h>
#include <windows.h>
#include <cstdlib>
#include <ctime>

using namespace std;

/*
Snake Game in C++ (Console Version)

How to Play:
- W → Up, A → Left, S → Down, D → Right
- Eat the food (*) to grow and increase your score
Snake head = @, body = o, food = *
- Avoid walls and your own body
- After Game Over, press Y to play again
*/

class Snake {
private:
    vector<pair<int,int>> body;
    char direction;
public:
    Snake(int startX, int startY) {
        body.push_back({startX, startY});
        direction = 'D';
    }
    pair<int,int> getHead() { return body.front(); }
    vector<pair<int,int>> getBody() { return body; }
    char getDirection() { return direction; }
    void setDirection(char dir) { direction = dir; }
    void move(bool grow=false) {
        pair<int,int> head = body.front();
        switch(direction) {
            case 'W': head.first--; break;
            case 'S': head.first++; break;
            case 'A': head.second--; break;
            case 'D': head.second++; break;
        }
        body.insert(body.begin(), head);
        if(!grow) body.pop_back();
    }
    bool checkCollision(int width, int height) {
        pair<int,int> head = getHead();
        if(head.first < 0 || head.first >= height || head.second < 0 || head.second >= width)
            return true;
        for(size_t i=1; i<body.size(); i++)
            if(body[i] == head) return true;
        return false;
    }
    void reset(int startX, int startY) {
        body.clear();
        body.push_back({startX, startY});
        direction = 'D';
    }
};

class Food {
private:
    pair<int,int> pos;
public:
    Food(int width, int height) { respawn(width, height, {}); }
    pair<int,int> getPosition() { return pos; }
    void respawn(int width, int height, vector<pair<int,int>> snakeBody) {
        while(true) {
            int x = rand()%height;
            int y = rand()%width;
            pos = {x, y};
            bool onSnake = false;
            for(auto s: snakeBody)
                if(s==pos) onSnake = true;
            if(!onSnake) break;
        }
    }
};

class Game {
private:
    int width, height, score;
    Snake snake;
    Food food;
    bool gameOver;
public:
    Game(int w,int h) : width(w), height(h), score(0), snake(w/2,h/2), food(w,h), gameOver(false) {}
    void draw() {
        system("cls");
        for(int i=0;i<width+2;i++) cout<<"# ";
        cout<<endl;
        for(int i=0;i<height;i++){
            cout<<"# ";
            for(int j=0;j<width;j++){
                pair<int,int> head = snake.getHead();
                if(head.first==i && head.second==j) cout<<"@ ";
                else if(food.getPosition()==make_pair(i,j)) cout<<"* ";
                else {
                    bool printed=false;
                    for(auto s:snake.getBody())
                        if(s.first==i && s.second==j){cout<<"o "; printed=true; break;}
                    if(!printed) cout<<"  ";
                }
            }
            cout<<"#"<<endl;
        }
        for(int i=0;i<width+2;i++) cout<<"# ";
        cout<<"\nScore: "<<score<<endl;
    }
    void input() {
        if(_kbhit()){
            char key = toupper(_getch());
            char dir = snake.getDirection();
            if((key=='W' && dir!='S') || (key=='S' && dir!='W') ||
               (key=='A' && dir!='D') || (key=='D' && dir!='A'))
                snake.setDirection(key);
        }
    }
    void logic() {
        pair<int,int> nextHead = snake.getHead();
        switch(snake.getDirection()){
            case 'W': nextHead.first--; break;
            case 'S': nextHead.first++; break;
            case 'A': nextHead.second--; break;
            case 'D': nextHead.second++; break;
        }
        bool grow = (nextHead == food.getPosition());
        if(grow){score+=10; food.respawn(width,height,snake.getBody());}
        snake.move(grow);
        if(snake.checkCollision(width,height)) gameOver=true;
    }
    void reset() {
        score = 0;
        snake.reset(width/2, height/2);
        food.respawn(width, height, snake.getBody());
        gameOver = false;
    }
    void run() {
        do {
            reset();
            while(!gameOver){
                draw();
                input();
                logic();
                Sleep(250);
            }
            draw();
            cout<<"\nGame Over! Final Score: "<<score<<endl;
            cout<<"Play again? (Y/N): ";
        } while(toupper(_getch()) == 'Y');
    }
};

int main(){
    srand(time(0));
    Game g(20,20);
    g.run();
    return 0;
}
