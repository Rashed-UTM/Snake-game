#include <iostream>
#include <conio.h>
#include <windows.h>
#include <vector>
#include <ctime>
#include <cstdlib>

using namespace std;

// Constants
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

// Direction enum
enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };

// Position structure
struct Position {
    int x, y;
    Position(int _x = 0, int _y = 0) : x(_x), y(_y) {}
    bool operator==(const Position& pos) const {
        return x == pos.x && y == pos.y;
    }
};

// Game class
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

    // Set console window size to fit the game field
    void setConsoleWindowSize() {
        COORD bufferSize = {WIDTH + 5, HEIGHT + 10};
        SMALL_RECT windowSize = {0, 0, WIDTH + 4, HEIGHT + 9};
        
        // Set console buffer size
        SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), bufferSize);
        
        // Set console window size
        SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), TRUE, &windowSize);
    }

    // Set cursor position for console rendering
    void gotoxy(int x, int y) {
        COORD coord;
        coord.X = x;
        coord.Y = y;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    }

    // Hide console cursor
    void hideCursor() {
        CONSOLE_CURSOR_INFO cursor;
        cursor.dwSize = 100;
        cursor.bVisible = false;
        SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor);
    }

    // Initialize the game
    void init() {
        gameOver = false;
        win = false;
        score = 0;
        dir = RIGHT; // Start with moving right
        speed = 100;

        // Initialize snake with 3 segments at the center
        snake.clear();
        Position center(WIDTH / 2, HEIGHT / 2);
        snake.push_back(center);
        snake.push_back(Position(center.x - 1, center.y));
        snake.push_back(Position(center.x - 2, center.y));

        // Place initial food
        placeFood();

        // Initialize map with empty spaces
        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                if (i == 0 || i == HEIGHT - 1 || j == 0 || j == WIDTH - 1)
                    map[i][j] = WALL;
                else
                    map[i][j] = EMPTY;
            }
        }
    }

    // Place food at random position
    void placeFood() {
        int x, y;
        bool valid;
        
        do {
            valid = true;
            x = rand() % (WIDTH - 2) + 1;
            y = rand() % (HEIGHT - 2) + 1;
            
            // Check if food position is not on snake
            for (const Position& pos : snake) {
                if (pos.x == x && pos.y == y) {
                    valid = false;
                    break;
                }
            }
        } while (!valid);
        
        food = Position(x, y);
    }

    // Update the game map
    void updateMap() {
        // Clear map (keep walls)
        for (int i = 1; i < HEIGHT - 1; i++) {
            for (int j = 1; j < WIDTH - 1; j++) {
                map[i][j] = EMPTY;
            }
        }

        // Place food
        map[food.y][food.x] = FOOD;

        // Place snake body
        for (size_t i = 1; i < snake.size(); i++) {
            map[snake[i].y][snake[i].x] = SNAKE_BODY;
        }

        // Place snake head with direction
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

    // Draw the game map
    void draw() {
        // Clear screen once
        system("cls");
        
        // Draw map
        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                cout << map[i][j];
            }
            cout << endl;
        }

        // Draw score
        cout << "Score: " << score << endl;
        cout << "Controls: WASD to move, Q to quit" << endl;
    }

    // Process input
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

    // Move the snake and update game state
    void update() {
        if (dir == STOP) return;

        // Calculate new head position
        Position newHead = snake[0];
        
        switch (dir) {
            case LEFT: newHead.x--; break;
            case RIGHT: newHead.x++; break;
            case UP: newHead.y--; break;
            case DOWN: newHead.y++; break;
            default: break;
        }

        // Check for collision with wall
        if (newHead.x == 0 || newHead.x == WIDTH - 1 || 
            newHead.y == 0 || newHead.y == HEIGHT - 1) {
            gameOver = true;
            return;
        }

        // Check for collision with self
        for (size_t i = 1; i < snake.size(); i++) {
            if (newHead == snake[i]) {
                gameOver = true;
                return;
            }
        }

        // Add new head
        snake.insert(snake.begin(), newHead);

        // Check for food
        if (newHead == food) {
            score += 10;
            
            // Increase speed every 50 points
            if (score % 50 == 0 && speed > 30) {
                speed -= 10;
            }
            
            // Check win condition (arbitrary max length)
            if (snake.size() >= 50) {
                win = true;
                gameOver = true;
                return;
            }
            
            placeFood();
        }
        else {
            // Remove tail if no food eaten
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

    // Main game loop
    void run() {
        while (!gameOver) {
            input();
            update();
            updateMap();
            draw();
            Sleep(speed); // Control game speed
        }

        // Game over message
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
    // Set console title
    SetConsoleTitle("Snake Game");

    // Welcome message
    cout << "Welcome to Snake Game!" << endl;
    cout << "Controls: WASD to move, Q to quit" << endl;
    cout << "Press any key to start..." << endl;
    _getch();

    // Run the game
    SnakeGame game;
    game.run();

    return 0;
} 