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
    protected:
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

    void initializeBoard() {
        // Initialize white pieces
        placePiece(Position(7, 0), PieceFactory::createPiece(ROOK, WHITE));
        placePiece(Position(7, 1), PieceFactory::createPiece(KNIGHT, WHITE));
        placePiece(Position(7, 2), PieceFactory::createPiece(BISHOP, WHITE));
        placePiece(Position(7, 3), PieceFactory::createPiece(QUEEN, WHITE));
        placePiece(Position(7, 4), PieceFactory::createPiece(KING, WHITE));
        placePiece(Position(7, 5), PieceFactory::createPiece(BISHOP, WHITE));
        placePiece(Position(7, 6), PieceFactory::createPiece(KNIGHT, WHITE));
        placePiece(Position(7, 7), PieceFactory::createPiece(ROOK, WHITE));
        
        for (int i = 0; i < 8; i++) {
            placePiece(Position(6, i), PieceFactory::createPiece(PAWN, WHITE));
        }
        
        // Initialize black pieces
        placePiece(Position(0, 0), PieceFactory::createPiece(ROOK, BLACK));
        placePiece(Position(0, 1), PieceFactory::createPiece(KNIGHT, BLACK));
        placePiece(Position(0, 2), PieceFactory::createPiece(BISHOP, BLACK));
        placePiece(Position(0, 3), PieceFactory::createPiece(QUEEN, BLACK));
        placePiece(Position(0, 4), PieceFactory::createPiece(KING, BLACK));
        placePiece(Position(0, 5), PieceFactory::createPiece(BISHOP, BLACK));
        placePiece(Position(0, 6), PieceFactory::createPiece(KNIGHT, BLACK));
        placePiece(Position(0, 7), PieceFactory::createPiece(ROOK, BLACK));
        
        for (int i = 0; i < 8; i++) {
            placePiece(Position(1, i), PieceFactory::createPiece(PAWN, BLACK));
        }
    }

    void placePiece(Position pos,Piece* piece) {
        board[pos.getRow()][pos.getCol()] = piece;
        piecePositions[pos] = piece;
    }
    void removePiece(Position pos) {
        board[pos.getRow()][pos.getCol()] = nullptr;
        piecePositions.erase(pos);
    }
    Piece* getPiece(Position pos) {
        return board[pos.getRow()][pos.getCol()];
    }
    bool isOccupied(Position pos) {
        return getPiece(pos) != nullptr;
    }
    bool isOccupiedBySameColor(Position pos, Color color) {
        Piece* piece = getPiece(pos);
        return piece!=nullptr && piece->getColor() == color;
    }
    void movePiece(Position from,Position to){
        Piece* piece = getPiece(from);
        if(piece!=nullptr) {
            Piece* capturedPiece = getPiece(to);
            if(capturedPiece!=nullptr) {
                delete capturedPiece;
                piecePositions.erase(to);
            }

            board[from.getRow()][from.getCol()] = nullptr;
            board[to.getRow()][to.getCol()] = piece;
            piecePositions.erase(from);
            piecePositions[to] = piece;

            piece->setMoved(true);
        }
    }
    Position findKing(Color color) {
        for(auto& pair:piecePositions) {
            if(pair.second->getType() == KING && pair.second->getColor() == color) {
                return pair.first;
            }
        }
        return Position(-1,-1);
    }
    vector<Position> getAllPiecesOfColor(Color color) {
        vector<Position> pieces;
        for(auto& pair:piecePositions) {
            if(pair.second->getColor() == color) {
                pieces.push_back(pair.first);
            }
        }
        return pieces;
    }
    void display() {
        constexpr int cellW = 3;  // cell width

        // — horizontal border —
        auto printBorder = [&]() {
            std::cout << "  +";
            for (int i = 0; i < 8; ++i)
                std::cout << std::string(cellW, '-') << "+";
            std::cout << "\n";
        };

        // — top border —
        printBorder();

        // — column labels inside the grid —
        std::cout << "  |";
        for (char f = 'a'; f <= 'h'; ++f) {
            int pad = (cellW - 1) / 2;
            std::cout
                << std::string(pad, ' ')
                << f
                << std::string(cellW - 1 - pad, ' ')
                << "|";
        }
        std::cout << "\n";

        // — border under labels —
        printBorder();

        // — each rank of pieces —
        for (int rank = 8; rank >= 1; --rank) {
            int row = 8 - rank;
            std::cout << rank << " |";

            for (int file = 0; file < 8; ++file) {
                Piece* p = board[row][file];
                std::string s = p ? p->to_string() : "  ";  // two spaces if empty

                // center a 2-char string in cellW
                int pad = (cellW - 2) / 2;
                std::cout
                    << std::string(pad, ' ')
                    << s
                    << std::string(cellW - 2 - pad, ' ')
                    << "|";
            }

            std::cout << " " << rank << "\n";
            printBorder();
        }

        // — bottom labels inside the grid —
        std::cout << "  |";
        for (char f = 'a'; f <= 'h'; ++f) {
            int pad = (cellW - 1) / 2;
            std::cout
                << std::string(pad, ' ')
                << f
                << std::string(cellW - 1 - pad, ' ')
                << "|";
        }
        std::cout << "\n";

        // — final border —
        printBorder();
    }
};

vector<Position> King::getPossibleMoves(Position currentPos,Board* board) {
    vector<Position> moves;
    int directions[8][2] = {{-1,-1},{-1,0},{-1,1},{0,-1},{0,1},{1,-1},{1,0},{1,1}};
    for(int i=0;i<8;i++) {
        Position newPos(currentPos.getRow()+directions[i][0] , currentPos.getCol()+directions[i][1]);
        if(newPos.isValid() && !board->isOccupiedBySameColor(newPos,this->color)){
            moves.push_back(newPos);
        }
    }
    return moves;
}

vector<Position> Queen::getPossibleMoves(Position currentPos,Board* board) {
    vector<Position> moves;
    int directions[8][2] = {{-1,-1},{-1,0},{-1,1},{0,-1},{0,1},{1,-1},{1,0},{1,1}};
    for(int d=0;d<8;d++) {
        for (int i = 1; i < 8; i++) {
            Position newPos(currentPos.getRow() + directions[d][0]*i, currentPos.getCol() + directions[d][1]*i);
            if (!newPos.isValid()) break;
            if (board->isOccupiedBySameColor(newPos, this->color)) break;
            moves.push_back(newPos);
            if (board->isOccupied(newPos)) break; // Stop after capturing
        }
    }
    return moves;
}

vector<Position> Rook::getPossibleMoves(Position currentPos,Board* board) {
    vector<Position> moves;
    int directions[4][2] = {{-1,0},{0,-1},{0,1},{1,0}};
    for(int d=0;d<4;d++) {
        for(int i=1;i<8;i++) {
            Position newPos(currentPos.getRow()+ i*directions[d][0] , currentPos.getCol() + i*directions[d][1]);
            if (!newPos.isValid()) break;
            if (board->isOccupiedBySameColor(newPos, this->color)) break;
            moves.push_back(newPos);
            if (board->isOccupied(newPos)) break; // Stop after capturing
        }
    }
    return moves;
}

vector<Position> Bishop::getPossibleMoves(Position currentPos,Board* board) {
    vector<Position> moves;
    int directions[4][2]={{-1,-1},{-1,1},{1,-1},{1,1}};
    for(int d=0;d<4;d++) {
        for(int i=1;i<8;i++) {
            Position newPos(currentPos.getRow() + directions[d][0]*i , currentPos.getCol() + directions[d][1]*i);
            if(!newPos.isValid()) break;
            if(board->isOccupiedBySameColor(newPos,this->color)) break;
            moves.push_back(newPos);
            if(board->isOccupied(newPos)) break; // Stop after capturing
        }
    }
    return moves;
}

vector<Position> Knight::getPossibleMoves(Position currentPos,Board* board) {
    vector<Position> moves;
    int directions[8][2] = {{-2,-1},{-1,-2},{-2,1},{-1,2},{2,1},{2,-1},{1,2},{1,-2}};
    for(int d=0;d<8;d++) {
        Position newPos(currentPos.getRow()+directions[d][0] , currentPos.getCol()+directions[d][1]);
        if(newPos.isValid() && !board->isOccupiedBySameColor(newPos,this->color)){
            moves.push_back(newPos);
        }
    }
    return moves;
}

vector<Position> Pawn::getPossibleMoves(Position currentPos,Board* board) {
    vector<Position> moves;
    int direction = (color==WHITE) ? -1 : 1;
    Position oneStep(currentPos.getRow()+direction,currentPos.getCol());
    Position leftCapture(currentPos.getRow()+direction,currentPos.getCol()-1);
    Position rightCapture(currentPos.getRow()+direction,currentPos.getCol()+1);

    if(oneStep.isValid() && !board->isOccupied(oneStep)) {
        moves.push_back(oneStep);

        if(!hasMoved) {
            Position twoStep(currentPos.getRow()+direction,currentPos.getCol());
            if(twoStep.isValid() && !board->isOccupied(twoStep)){
                moves.push_back(twoStep);
            }
        }
    }
    if (leftCapture.isValid() && board->isOccupied(leftCapture) && 
        !board->isOccupiedBySameColor(leftCapture, this->color)) {
        moves.push_back(leftCapture);
    }
    
    if (rightCapture.isValid() && board->isOccupied(rightCapture) && 
        !board->isOccupiedBySameColor(rightCapture, this->color)) {
        moves.push_back(rightCapture);
    }

    return moves;
}

class ChessRules {
    public:
    virtual bool isValidMove(Move m,Board* b) = 0;
    virtual bool isInCheck(Color c,Board* b) = 0;
    virtual bool isCheckMate(Color c,Board* b) = 0;
    virtual bool isStaleMate(Color c,Board* b) = 0;
    virtual bool wouldCauseCheck(Move m,Board* b,Color c) = 0;
};
class StandardChessRules : public ChessRules {
    public:
    bool isValidMove(Move m,Board* b) override {
        vector<Position> possibleMoves = m.getPiece()->getPossibleMoves(m.getFrom(),b);

        bool validDestination = false;
        for(auto it:possibleMoves) {
            if(it == m.getTo()){
                validDestination = true;
                break;
            }
        }

        if(!validDestination)
            return false;

        return !wouldCauseCheck(m, b, m.getPiece()->getColor());

    }
    bool isInCheck(Color c,Board* b) override {

    }
    bool isCheckMate(Color c,Board* b) override {

    }
    bool isStaleMate(Color c,Board* b) override {

    }
    bool wouldCauseCheck(Move m,Board* b,Color c) override {

    }
};





int main() {

    return 0;
}