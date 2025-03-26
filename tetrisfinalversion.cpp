#include <iostream>
#include <windows.h>
#include <conio.h>
#include <fstream>
#include <string>
#include <ctime>
using namespace std;

// ANSI Color Codes for Tetrominoes
#define RED "\033[31m"
#define GREEN "\033[32m"
#define BLUE "\033[34m"
#define YELLOW "\033[33m"
#define CYAN "\033[36m"
#define MAGENTA "\033[35m"
#define WHITE "\033[37m"
#define GREY "\033[90m"
#define RESET "\033[0m"

#define HEIGHT 20
#define WIDTH 20

void moveCursorToTop() {
    cout << "\033[H";  // ANSI Escape Code to Move Cursor to Top
}

void showGuideMenu() {
    system("cls");
    cout << "\n===== TETRIS CONTROLS =====\n";
    cout << "Left Arrow  : Move Left\n";
    cout << "Right Arrow : Move Right\n";
    cout << "Up Arrow    : Rotate\n";
    cout << "Down Arrow  : Soft Drop\n";
    cout << "Space bar   : Hard Drop\n";
    cout << "ESC         : Pause\n";
    cout << "\nPress any key to start...";
    _getch();
    system("cls");
}

bool showGameOverMenu() {
    cout << "\n===== GAME OVER =====\n";
    cout << "1. Restart\n";
    cout << "2. Exit\n";
    cout << "Enter your choice: ";
    char choice = _getch();
    return (choice == '1');
}

class Tetromino {
public:
    struct shape {
        char name;
        int states[4][4][4];
        int currState = 0;
    };

    shape shapes[7] = {
        { 'I', { 
            { {0,0,0,0}, {1,1,1,1}, {0,0,0,0}, {0,0,0,0} },  
            { {0,0,1,0}, {0,0,1,0}, {0,0,1,0}, {0,0,1,0} },  
            { {0,0,0,0}, {1,1,1,1}, {0,0,0,0}, {0,0,0,0} },  
            { {0,0,1,0}, {0,0,1,0}, {0,0,1,0}, {0,0,1,0} }   
        }, 0 },
    
        { 'O', { 
            { {1,1,0,0}, {1,1,0,0}, {0,0,0,0}, {0,0,0,0} },  
            { {1,1,0,0}, {1,1,0,0}, {0,0,0,0}, {0,0,0,0} },  
            { {1,1,0,0}, {1,1,0,0}, {0,0,0,0}, {0,0,0,0} },  
            { {1,1,0,0}, {1,1,0,0}, {0,0,0,0}, {0,0,0,0} }   
        }, 0 },
    
        { 'T', { 
            { {0,1,0,0}, {1,1,1,0}, {0,0,0,0}, {0,0,0,0} },  
            { {0,1,0,0}, {0,1,1,0}, {0,1,0,0}, {0,0,0,0} },  
            { {0,0,0,0}, {1,1,1,0}, {0,1,0,0}, {0,0,0,0} },  
            { {0,1,0,0}, {1,1,0,0}, {0,1,0,0}, {0,0,0,0} }   
        }, 0 },
    
        { 'S', { 
            { {0,1,1,0}, {1,1,0,0}, {0,0,0,0}, {0,0,0,0} },  
            { {0,1,0,0}, {0,1,1,0}, {0,0,1,0}, {0,0,0,0} },  
            { {0,1,1,0}, {1,1,0,0}, {0,0,0,0}, {0,0,0,0} },  
            { {0,1,0,0}, {0,1,1,0}, {0,0,1,0}, {0,0,0,0} }   
        }, 0 },
    
        { 'Z', { 
            { {1,1,0,0}, {0,1,1,0}, {0,0,0,0}, {0,0,0,0} },  
            { {0,0,1,0}, {0,1,1,0}, {0,1,0,0}, {0,0,0,0} },  
            { {1,1,0,0}, {0,1,1,0}, {0,0,0,0}, {0,0,0,0} },  
            { {0,0,1,0}, {0,1,1,0}, {0,1,0,0}, {0,0,0,0} }   
        }, 0 },
    
        { 'J', { 
            { {1,0,0,0}, {1,1,1,0}, {0,0,0,0}, {0,0,0,0} },  
            { {0,1,1,0}, {0,1,0,0}, {0,1,0,0}, {0,0,0,0} },  
            { {0,0,0,0}, {1,1,1,0}, {0,0,1,0}, {0,0,0,0} },  
            { {0,1,0,0}, {0,1,0,0}, {1,1,0,0}, {0,0,0,0} }   
        }, 0 },
    
        { 'L', { 
            { {0,0,1,0}, {1,1,1,0}, {0,0,0,0}, {0,0,0,0} },  
            { {0,1,0,0}, {0,1,0,0}, {0,1,1,0}, {0,0,0,0} },  
            { {0,0,0,0}, {1,1,1,0}, {1,0,0,0}, {0,0,0,0} },  
            { {1,1,0,0}, {0,1,0,0}, {0,1,0,0}, {0,0,0,0} }   
        }, 0 }
    };

    shape generateTetrominoes() {
        return shapes[rand() % 7];
    }

    static string getTetrominoColor(char type) {
        switch (type) {
            case 'I': return CYAN;      
            case 'O': return YELLOW;    
            case 'T': return MAGENTA;   
            case 'S': return GREEN;     
            case 'Z': return RED;       
            case 'J': return BLUE;      
            case 'L': return WHITE;     
            default: return RESET;
        }
    }

    static void rotate(shape &t) {
        t.currState = (t.currState + 1) % 4;  // Cycle through 0,1,2,3
    }
};

class gameboard {
public:
    static char grid[HEIGHT][WIDTH];
    static string colorGrid[HEIGHT][WIDTH];

    gameboard() {
        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                grid[i][j] = ' ';
                colorGrid[i][j] = RESET;
            }
        }
    }

    void displayBoard(int x, int y, Tetromino::shape &t, int ghostY) {
        moveCursorToTop();

        string border = "";
        bool isghostPiece;
        for (int i = 0; i < HEIGHT; i++) {
            border += char(219); // Left border
            for (int j = 1; j < WIDTH; j++) {
                bool tetrominoPart = false;
                bool isghostPiece=false;
                string color = RESET;

                if (i == 0 || i == HEIGHT - 1) {
                    border += char(219);
                    continue;
                }

                for (int ti = 0; ti < 4; ti++) {
                    for (int tj = 0; tj < 4; tj++) {
                        if (t.states[t.currState][ti][tj] == 1 && i == y + ti && j == x + tj) {
                            tetrominoPart = true;
                            color = Tetromino::getTetrominoColor(t.name);
                        }
                        if(i==ghostY+ ti && t.states[t.currState][ti][tj] == 1 && j==x+tj ){
                            color=GREY;
                            isghostPiece=true;
                        }
                    }
                }



                if (tetrominoPart) {
                    border += color +char(219) + RESET;
                }
                 else if (grid[i][j] != ' ') {
                    border += colorGrid[i][j] + char(219) + RESET;
                } 
                else if (isghostPiece) {
                    border += color + char(219) + RESET; // Grey color for ghost
                }
                else {
                    border += " ";
                }
            }
            border += char(219); // Right border
            border += '\n';
        }
        cout << border;
    }
};

char gameboard::grid[HEIGHT][WIDTH] = {};
string gameboard::colorGrid[HEIGHT][WIDTH] = {};

class ScoreManager {
public:
    int currentScore;
    int highScore;

    ScoreManager() : currentScore(0), highScore(0) {
        loadHighScore();
    }

    void loadHighScore() {
        ifstream file("highscore.txt");
        if (file.is_open()) {
            file >> highScore;
            file.close();
        } else {
            highScore = 0;
        }
    }

    void saveHighScore() {
        ofstream file("highscore.txt");
        if (file.is_open()) {
            file << highScore;
            file.close();
        }
    }

    void updateScore(int linesCleared) {
        switch (linesCleared) {
            case 1: currentScore += 100; break;
            case 2: currentScore += 300; break;
            case 3: currentScore += 500; break;
            case 4: currentScore += 800; break;
        }
        if (currentScore > highScore) {
            highScore = currentScore;
            saveHighScore();
        }
    }

    void displayScores() const {
        cout << "Current Score: " << currentScore << " | High Score: " << highScore << endl;
    }
};

class gameLogic {
public:
    bool gameOver;

    gameLogic() : gameOver(false) {}

    int clearFullRows(ScoreManager &scoreManager) {
        int newRow = HEIGHT - 2;
        int clearedRows = 0;

        for (int y = HEIGHT - 2; y > 0; y--) {
            bool full = true;

            for (int x = 1; x < WIDTH-1; x++) {
                if (gameboard::grid[y][x] == ' ') {
                    full = false;
                    break;
                }
            }

            if (!full) {
                for (int x = 0; x < WIDTH; x++) {
                    gameboard::grid[newRow][x] = gameboard::grid[y][x];
                }
                newRow--;
            } else {
                clearedRows++;
            }
        }

        for (int y = newRow; y >= 0; y--) {
            for (int x = 0; x < WIDTH; x++) {
                gameboard::grid[y][x] = ' ';
            }
        }

        if (clearedRows > 0) {
            scoreManager.updateScore(clearedRows);
        }

        return clearedRows;
    }

    static bool collided(Tetromino::shape &tetromino, int x, int y) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if (tetromino.states[tetromino.currState][i][j] == 0) continue;

                int newX = x + j;
                int newY = y + i;

                if (newX <= 0 || newX >= WIDTH || newY >= HEIGHT-1) return true;
                if (newY >= 0 && gameboard::grid[newY][newX] != ' ') return true;
            }
        }
        return false;
    }

    static void placeTetrominoOnBoard(int x, int y, Tetromino::shape &tetromino) {
        for (int i = 0; i < 4; i++) {
            for (int j =0; j < 4; j++) {
                if (tetromino.states[tetromino.currState][i][j] == 1) {
                    int newX = x + j;
                    int newY = y + i;

                    if (newX >= 0 && newX < WIDTH && newY >= 0 && newY < HEIGHT) {
                        gameboard::grid[newY][newX] = '#';
                        gameboard::colorGrid[newY][newX] = Tetromino::getTetrominoColor(tetromino.name);
                    }
                }
            }
        }
    }

    bool isGameOver(int x, int y, Tetromino::shape &tetromino) {
        return collided(tetromino, x, y);
    }

    static bool canRotate(Tetromino::shape &tetromino, int x, int y) {
        Tetromino::shape rotatedTetromino = tetromino;
        Tetromino::rotate(rotatedTetromino);
        return !collided(rotatedTetromino, x, y);
    }

    static int hard_drop(int x, int y, Tetromino::shape &tetromino, ScoreManager &scoreManager) {
        gameLogic logic;
        while (!collided(tetromino, x, y + 1)) {
            y++;
        }
        placeTetrominoOnBoard(x, y, tetromino);
        logic.clearFullRows(scoreManager);
        return y;
    }

    static int findGhostY(int x, int y, Tetromino::shape &tetromino) {
        while (!gameLogic::collided(tetromino, x, y +1)) {
            y++;
        }
        return y;
    }
};

class Input {
public:
    void handleInput(int &x, int &y, Tetromino::shape &tetromino, ScoreManager &scoreManager) {
        if (_kbhit()) {
            char input = _getch();

            if (input == 27) { // ESC key to pause
                cout << "Game Paused!" << endl;
                cout << "Press any key to continue..." << endl;
                _getch(); 
                return;
            } else if (input == 224 || input == -32) { // Handle both standard and extended arrow key codes
                input = _getch();
                
                switch(input) {
                    case 75: // Left arrow
                        if (!gameLogic::collided(tetromino, x - 1, y)) {
                            x--;
                        }
                        break;
                    case 77: // Right arrow
                        if (!gameLogic::collided(tetromino, x + 1, y)) {
                            x++; 
                        }
                        break;
                    case 72: // Up arrow
                        if (gameLogic::canRotate(tetromino, x, y)) {
                            Tetromino::rotate(tetromino); // Rotate
                        }
                        break;
                    case 80: // Down arrow
                        if (!gameLogic::collided(tetromino, x, y + 1)) {
                            y++; // Soft drop
                        }
                        break;
                }
            } else if (input == ' ') { // Spacebar for hard drop
                y = gameLogic::hard_drop(x, y, tetromino, scoreManager);
            }
        }
    }
};

int main() {
    srand(time(0));
    bool playAgain = true;
    
    while (playAgain) {
        showGuideMenu();
        
        gameboard board;
        gameLogic logic;
        Input userInput;
        ScoreManager scoreManager;
        Tetromino tetromino;

        int x = WIDTH / 2 - 2, y = 0;
        Tetromino::shape currTetromino = tetromino.generateTetrominoes();
        
        while (!logic.isGameOver(x, y, currTetromino)) {
            int ghostY =gameLogic:: findGhostY(x, y, currTetromino);
            board.displayBoard(x, y, currTetromino, ghostY);
            scoreManager.displayScores();
            
            userInput.handleInput(x, y, currTetromino, scoreManager);
            
            if (!gameLogic::collided(currTetromino, x, y + 1)) {
                y++;
            } else {
                gameLogic::placeTetrominoOnBoard(x, y, currTetromino);
                logic.clearFullRows(scoreManager);
                board.displayBoard(x, y, currTetromino,ghostY);

                x = WIDTH / 2 - 2;
                y = 0;
                currTetromino = tetromino.generateTetrominoes();
            }

            Sleep(300);
        }

        cout << "\nGame Over!\n";
        cout << "Final Score: " << scoreManager.currentScore << "\n";
        playAgain = showGameOverMenu();
    }
    
    cout << "\nThanks for playing!\n";
    return 0;
}
