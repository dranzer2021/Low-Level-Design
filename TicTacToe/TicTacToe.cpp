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

class TicTacToeRules {
    public:

    virtual bool isValidMove(Board* board,int row,int col) = 0;
    virtual bool checkWin(Board* board,Symbol* s) = 0;
    virtual bool checkDraw(Board* board) = 0;
};

class StandardRules: public TicTacToeRules {
    public:

    virtual bool isValidMove(Board* board,int row,int col) override {
        return board->isCellEmpty(row,col);
    }
    virtual bool checkWin(Board* board,Symbol* s) override {
        int size = board->getSize();
        //Row wise win check
        for(int i=0;i<size;i++) {
            bool win = true;
            for(int j=0;j<size;j++) {
                if(board->getCell(i,j) != s){
                    win = false;
                    break;
                }
            }
            if(win == true)
                return true;
        }
        //Column wise win check
        for(int i=0;i<size;i++) {
            bool win = true;
            for(int j=0;j<size;j++) {
                if(board->getCell(j,i) != s){
                    win = false;
                    break;
                }
            }
            if(win == true)
                return true;
        }
        //Diagonal wise win check
        bool win = true;
        for(int i=0;i<size;i++) {
            if(board->getCell(i,i) != s){
                win = false;
                break;
            }
        }
        if(win == true)
            return true;
        win = true;
        for(int i=0;i<size;i++) {
            if(board->getCell(i,size-1-i) != s){
                win = false;
                break;
            }
        }
        if(win == true)
            return true;

        return false;
    }
    virtual bool checkDraw(Board* board) override {
        int size = board->getSize();
        for(int i=0;i<size;i++) {
            for(int j=0;j<size;j++) {
                if(board->getCell(i,j) == board->getEmptyCell())
                    return false;
            }
        }
        return true;
    }
};

class TicTacToeGame {
    Board* board;
    deque<TicTacToePlayer*> players;
    TicTacToeRules* rules;
    vector<IObserver*> observers;
    bool gameOver;

    public:
    TicTacToeGame(int boardSize) {
        board = new Board(boardSize);
        rules = new StandardRules();
        gameOver = false;
    }
    void addPlayer(TicTacToePlayer* player) {
        players.push_back(player);
    }
    void addObserver(IObserver* observer) {
        observers.push_back(observer);
    }
    void notify(string message) {
        for(auto it:observers) {
            it->update(message);
        }
    }
    void play() {
        if(players.size() < 2) {
            cout<<"Need atleast 2 players!\n";
            return;
        }
        notify("Tic Tac Toe game started !\n");
        while(!gameOver) {
            board->display();

            TicTacToePlayer* currentPlayer = players.front();
            cout<<currentPlayer->getName()<<" ("<<currentPlayer->getSymbol()->getMark()<<") = Enter row and column: ";
            int row , col;
            cin>>row>>col;

            if(rules->isValidMove(board,row,col)) {
                board->placeMark(row,col,currentPlayer->getSymbol());
                notify(currentPlayer->getName() + "played (" + to_string(row) + "," + to_string(col) + ")");

                if(rules->checkWin(board,currentPlayer->getSymbol())) {
                    board->display();
                    cout<<currentPlayer->getName()<<" wins! \n\n";
                    currentPlayer->incrementScore();

                    notify(currentPlayer->getName() + " wins!");
                    gameOver = true;
                }
                else if(rules->checkDraw(board)) {
                    board->display();

                    cout<<"Match Drawn!"<<endl;
                    notify("Game is draw!");

                    gameOver = true;
                }
                else {
                    players.pop_front();
                    players.push_back(currentPlayer);
                }
            }
            else {
                cout<<"Invalid Move!"<<endl;
            }
        }
    }
    ~TicTacToeGame() {
        delete board;
        delete rules;
    }
};

enum GameType {
    STANDARD
};

class TicTacToeGameFactory {
    public:
    static TicTacToeGame* createGame(GameType gt,int boardSize) {
        if(GameType::STANDARD == gt) {
            return new TicTacToeGame(boardSize);
        }
        return nullptr;
    }
};

int main() {
    cout << "=== TIC TAC TOE GAME ===" << endl;
    
    // Create game with custom board size
    int boardSize;
    cout << "Enter board size (e.g., 3 for 3x3): ";
    cin >> boardSize;
    
    TicTacToeGame* game = TicTacToeGameFactory::createGame(GameType::STANDARD, boardSize);
    
    // Add observer
    IObserver* notifier = new ConsoleNotifier();
    game->addObserver(notifier);
    
    // Create players with custom symbols
    TicTacToePlayer* player1 = new TicTacToePlayer(1, "Aditya", new Symbol('X'));
    TicTacToePlayer* player2 = new TicTacToePlayer(2, "Harshita", new Symbol('O'));
    
    game->addPlayer(player1);
    game->addPlayer(player2);
    
    // Play the game
    game->play();
    
    // Cleanup
    delete game;
    delete player1;
    delete player2;
    delete notifier;
    return 0;
}
 


