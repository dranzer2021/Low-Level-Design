#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <algorithm>
#include <cmath>
#include <ctime>
#include <iomanip>

using namespace std;

enum Color {
    WHITE,
    BLACK
};

enum PieceType {
    KING,
    QUEEN,
    BISHOP,
    ROOK,
    KNIGHT,
    PAWN
};

enum GameStatus {
    WAITING,
    IN_PROGRESS,
    COMPLETED,
    ABORTED
};

class Position {
    int row;
    int col;

    public:
    Position() {
        row=0;
        col=0;
    }
    Position(int r,int c) {
        row=r;
        col=c;
    }
    int getRow() const {
        return row;
    }
    int getCol() const {
        return col;
    }
    bool isValid() const {
        return row>=0 && row<8 && col>=0 && col<8 ;
    }
    bool operator==(const Position& other) const {
        return row==other.row && col==other.col;
    }
    bool operator<(const Position& other) const {
        if(row!=other.row) return row<other.row;
        return col<other.col;
    }
    string toString() const {
        return "(" + to_string(row) + "," + to_string(col) + ")";
    }
    // Convert to chess notation (e.g., e4, f7)
    string toChessNotation() const {
        char file='a'+row;
        char rank='8'-row;
        return string(1,file)+string(1,rank);
    }

};

class Piece;
class Board;

class Move {
    Position from;
    Position to;
    Piece* piece;
    Piece* capturedPiece;

    public:
    Move() {
        piece=nullptr;
        capturedPiece=nullptr;
    }
    Move(Position from,Position to,Piece* piece,Piece* capturedPiece) {
        this->from = from;
        this->to = to;
        this->capturedPiece = capturedPiece;
        this->piece = piece;
    }
    Position getFrom() const {
        return from;
    }
    Position getTo() const {
        return to;
    }
    Piece* getPiece() const {
        return piece;
    }
    Piece* getCapturedPiece() const {
        return capturedPiece;
    }
};

class Piece {
    Color color;
    PieceType pieceType;
    bool hasMoved;

    public:
    Piece(Color c,PieceType pt) {
        hasMoved=false;
        color=c;
        pieceType=pt;
    }
    virtual ~Piece() {}
    
    Color getColor() const { 
        return color; 
    }
    PieceType getType() const { 
        return pieceType; 
    }
    bool getHasMoved() const { 
        return hasMoved; 
    }
    void setMoved(bool moved) { 
        hasMoved = moved; 
    }

    virtual vector<Position> getPossibleMoves(Position currentPos, Board* board) = 0;
    virtual string getSymbol() = 0;

    string to_string() {
        string colorString = (color==WHITE ? "W" : "B");
        return colorString + getSymbol();
    }
};

class King : public Piece {
public:
    King(Color color) : Piece(color, KING) {}
    
    vector<Position> getPossibleMoves(Position currentPos, Board* board) override;
    string getSymbol() override { 
        return "K"; 
    }
};

class Queen : public Piece {
public:
    Queen(Color color) : Piece(color, QUEEN) {}
    
    vector<Position> getPossibleMoves(Position currentPos, Board* board) override;
    string getSymbol() override { 
        return "Q"; 
    }
};

class Rook : public Piece {
public:
    Rook(Color color) : Piece(color, ROOK) {}
    
    vector<Position> getPossibleMoves(Position currentPos, Board* board) override;
    string getSymbol() override { 
        return "R"; 
    }
};

class Bishop : public Piece {
public:
    Bishop(Color color) : Piece(color, BISHOP) {}
    
    vector<Position> getPossibleMoves(Position currentPos, Board* board) override;
    string getSymbol() override { 
        return "B"; 
    }
};

class Knight : public Piece {
public:
    Knight(Color color) : Piece(color, KNIGHT) {}
    
    vector<Position> getPossibleMoves(Position currentPos, Board* board) override;
    string getSymbol() override { 
        return "N"; 
    }
};

class Pawn : public Piece {
public:
    Pawn(Color color) : Piece(color, PAWN) {}
    
    vector<Position> getPossibleMoves(Position currentPos, Board* board) override;
    string getSymbol() override { 
        return "P"; 
    }
};

class PieceFactory {
public:
    static Piece* createPiece(PieceType type, Color color) {
        switch (type) {
            case KING: return new King(color);
            case QUEEN: return new Queen(color);
            case ROOK: return new Rook(color);
            case BISHOP: return new Bishop(color);
            case KNIGHT: return new Knight(color);
            case PAWN: return new Pawn(color);
            default: return nullptr;
        }
    }
};

class Board {
    Piece* board[8][8];
    map<Position, Piece*> piecePositions;

public:
    Board() {
        // Initialize board to null
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                board[i][j] = nullptr;
            }
        }
        initializeBoard();
    }
    
    ~Board() {
        // Clean up pieces safely
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (board[i][j] != nullptr) {
                    delete board[i][j];
                    board[i][j] = nullptr;
                }
            }
        }
        piecePositions.clear();
    }

    void initializeBoard(){
        
    }

};


int main() {

    return 0;
}