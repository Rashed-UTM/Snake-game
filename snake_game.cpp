#include <iostream>
#include <conio.h>
#include <windows.h>
#include <vector>
#include <ctime>
#include <cstdlib>

using namespace std;
const int WIDTH = 90;
const int HEIGHT = 80;
const char SNAKE_BODY = 'O';
const char HEAD_LEFT = '<';
const char HEAD_RIGHT = '>';
const char HEAD_UP = '^';
const char HEAD_DOWN = 'v';
const char FOOD = '*';
const char EMPTY = ' ';
const char WALL = '#';


enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };


struct Position {
    int x, y;
    Position(int _x = 0, int _y = 0) : x(_x), y(_y) {}
    bool operator==(const Position& pos) const {
        return x == pos.x && y == pos.y;
    }
};


class SnakeGame {
private:
    bool gameOver;
    bool win;
    int score;
    char map[HEIGHT][WIDTH];
    Direction dir;
    vector<Position> snake;
    Position food;
    int speed;

    
    void setConsoleWindowSize() {
        COORD bufferSize = {WIDTH + 5, HEIGHT + 10};
        SMALL_RECT windowSize = {0, 0, WIDTH + 4, HEIGHT + 9};
        

        SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), bufferSize);
      
        SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), TRUE, &windowSize);
    }

    void gotoxy(int x, int y) {
        COORD coord;
        coord.X = x;
        coord.Y = y;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    }

    void hideCursor() {
        CONSOLE_CURSOR_INFO cursor;
        cursor.dwSize = 100;
        cursor.bVisible = false;
        SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor);
    }

    void init() {
        gameOver = false;
        win = false;
        score = 0;
        dir = RIGHT; 
        speed = 100;

        snake.clear();
        Position center(WIDTH / 2, HEIGHT / 2);
        snake.push_back(center);
        snake.push_back(Position(center.x - 1, center.y));
        snake.push_back(Position(center.x - 2, center.y));

        placeFood();

        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                if (i == 0 || i == HEIGHT - 1 || j == 0 || j == WIDTH - 1)
                    map[i][j] = WALL;
                else
                    map[i][j] = EMPTY;
            }
        }
    }

    void placeFood() {
        int x, y;
        bool valid;
        
        do {
            valid = true;
            x = rand() % (WIDTH - 2) + 1;
            y = rand() % (HEIGHT - 2) + 1;
            
            for (const Position& pos : snake) {
                if (pos.x == x && pos.y == y) {
                    valid = false;
                    break;
                }
            }
        } while (!valid);
        
        food = Position(x, y);
    }

    void updateMap() {
        for (int i = 1; i < HEIGHT - 1; i++) {
            for (int j = 1; j < WIDTH - 1; j++) {
                map[i][j] = EMPTY;
            }
        }

        map[food.y][food.x] = FOOD;

        for (size_t i = 1; i < snake.size(); i++) {
            map[snake[i].y][snake[i].x] = SNAKE_BODY;
        }

        char headChar;
        switch (dir) {
            case LEFT: headChar = HEAD_LEFT; break;
            case RIGHT: headChar = HEAD_RIGHT; break;
            case UP: headChar = HEAD_UP; break;
            case DOWN: headChar = HEAD_DOWN; break;
            default: headChar = HEAD_RIGHT; break;
        }
        map[snake[0].y][snake[0].x] = headChar;
    }


    void draw() {
        system("cls");
        
        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                cout << map[i][j];
            }
            cout << endl;
        }

        cout << "Score: " << score << endl;
        cout << "Controls: WASD to move, Q to quit" << endl;
    }

    void input() {
        if (_kbhit()) {
            char key = _getch();
            switch (key) {
                case 'a': if (dir != RIGHT) dir = LEFT; break;
                case 'd': if (dir != LEFT) dir = RIGHT; break;
                case 'w': if (dir != DOWN) dir = UP; break;
                case 's': if (dir != UP) dir = DOWN; break;
                case 'q': gameOver = true; break;
            }
        }
    }

    void update() {
        if (dir == STOP) return;

        Position newHead = snake[0];
        
        switch (dir) {
            case LEFT: newHead.x--; break;
            case RIGHT: newHead.x++; break;
            case UP: newHead.y--; break;
            case DOWN: newHead.y++; break;
            default: break;
        }

        if (newHead.x == 0 || newHead.x == WIDTH - 1 || 
            newHead.y == 0 || newHead.y == HEIGHT - 1) {
            gameOver = true;
            return;
        }

        for (size_t i = 1; i < snake.size(); i++) {
            if (newHead == snake[i]) {
                gameOver = true;
                return;
            }
        }

        snake.insert(snake.begin(), newHead);

        if (newHead == food) {
            score += 10;
            
            if (score % 50 == 0 && speed > 30) {
                speed -= 10;
            }
            
            if (snake.size() >= 50) {
                win = true;
                gameOver = true;
                return;
            }
            
            placeFood();
        }
        else {
            snake.pop_back();
        }
    }

public:
    SnakeGame() {
        srand(static_cast<unsigned>(time(nullptr)));
        setConsoleWindowSize();
        init();
        hideCursor();
    }

    void run() {
        while (!gameOver) {
            input();
            update();
            updateMap();
            draw();
            Sleep(speed); 
        }

        gotoxy(0, HEIGHT + 3);
        if (win) {
            cout << "Congratulations! You won with score: " << score << endl;
        } else {
            cout << "Game Over! Your score: " << score << endl;
        }
        cout << "Press any key to exit..." << endl;
        _getch();
    }
};

int main() {

    SetConsoleTitle("Snake Game");

    cout << "Welcome to Snake Game!" << endl;
    cout << "Controls: WASD to move, Q to quit" << endl;
    cout << "Press any key to start..." << endl;
    _getch();

    SnakeGame game;
    game.run();

    return 0;
} 
