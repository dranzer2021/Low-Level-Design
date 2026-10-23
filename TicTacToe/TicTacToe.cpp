#include <iostream>
#include <vector>
#include <queue>
#include <deque>

using namespace std;

class IObserver {
    public:
    virtual void update(string message) = 0;
    virtual ~IObserver() {}
};

class ConsoleNotifier : public IObserver {
    public:
    void update(string message) override {
        cout<<"[Notification] "<<message<<"\n";
    }
};

class Symbol {
    char mark;
    
    public:
    Symbol(char ch) {
        mark = ch;
    }
    char getMark() {
        return mark;
    }
};

class Board {
    vector<vector<Symbol*>> grid;
    int size;
    Symbol* emptyCell;

    public:
    Board(int s) {
        size = s;
        emptyCell = new Symbol('-');
        grid = vector<vector<Symbol*>>(size, vector<Symbol*>(size, emptyCell));
    }
    bool isCellEmpty(int row,int col) {
        if(row < 0 || row >= size || col < 0 || col >= size) {
            return false;
        }
        return grid[row][col] == emptyCell;
    }
    bool placeMark(int row,int col,Symbol* symbol) {
        if(row < 0 || row >= size || col < 0 || col >= size) {
            return false;
        }
        grid[row][col] = symbol;
        return true;
    }
    Symbol* getCell(int row,int col) {
        if(row < 0 || row >= size || col < 0 || col >= size) {
            return emptyCell;
        }
        return grid[row][col];
    }

    int getSize() {
        return size;
    }
    
    Symbol* getEmptyCell() {
        return emptyCell;
    }
    
    void display() {
        cout << "\n  ";
        for(int i = 0; i < size; i++) {
            cout << i << " ";
        }
        cout << endl;
        
        for(int i = 0; i < size; i++) {
            cout << i << " ";
            for(int j = 0; j < size; j++) {
                cout << grid[i][j]->getMark() << " ";
            }
            cout << endl;
        }
        cout << endl;
    }
};

class TicTacToePlayer {
private:
    int playerId;
    string name;
    Symbol* symbol;
    int score;
    
public:
    TicTacToePlayer(int playerId, string n, Symbol* s) {
        this->playerId = playerId;
        name = n;
        symbol = s;
        score = 0;
    }
    
    // Getters and setters
    string getName() { 
        return name; 
    }

    Symbol* getSymbol() { 
        return symbol; 
    }

    int getScore() { 
        return score; 
    }

    void incrementScore() { 
        score++;
    }
    
    ~TicTacToePlayer() {
        delete symbol;
    }
};



