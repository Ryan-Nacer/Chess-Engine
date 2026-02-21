#pragma once
#include <QApplication>
#include <vector>
#include "../include/cppitertools/range.hpp"

namespace config {
    constexpr int BOARD_DIMENSION_X = 8;
    constexpr int BOARD_DIMENSION_Y = 8;
    constexpr int NUMBER_OF_TILES = 64;
    constexpr int MAXIMUM_KINGS_CONFRONTATION = 2;
    constexpr int INITIAL_PIECE_POSITION = 0;
    constexpr char BLACK_KING = 'g';
    constexpr char WHITE_KING = 'k';
    constexpr char BLACK_KNIGHT = 't';
    constexpr char WHITE_KNIGHT = 'n';
    constexpr char BLACK_ROOK = 'o';
    constexpr char WHITE_ROOK = 'r';
    constexpr char EMPTY_PIECE = 'x';
    constexpr char START_WITH_BLACK = 'b';
    const std::string KING_OVER_LIMIT = "Le nombre de rois dépasse le seuil";
    const std::string KING_BELOW_LIMIT = "Le nombre de rois est inférieur au seuil";

    class Board;
    enum class Color { Black, White };

//Abstract class Piece 
    class Piece {
    public:
        Piece(const Color&);
        Color getColor() const;
        bool isInsideBounds(const std::pair<int, int>&) const;
        void setColor(const Color&);

        virtual char getName() const = 0;
        virtual void addMovement(const std::pair<int, int>&) =0 ;
        //virtual void calculatePossibleMovements(const std::pair<int, int>&, Board*) = 0;
        virtual void calculatePossibleSimpleMovements(const std::pair<int, int>&, Board*) = 0;
        virtual void calculatePossibleBasicMovements(const std::pair<int, int>& initialPosition, Board* board) = 0;
        virtual std::vector<std::pair<int, int>> getPossibleMovements() const = 0;
        virtual void clearMovements() = 0;

        virtual	~Piece() = default;

    private:
        Color color_;
        
    };

//les peices : knight et rook
    class King : public Piece {
    public:
        King(const Color&);

        bool isConfrontingEnemyKing(const std::pair<int, int>&, const std::pair<int, int>&) const;
        void addMovement(const std::pair<int, int>&) override;
        //void calculatePossibleMovements(const std::pair<int, int>&, Board*) override;
        void calculatePossibleSimpleMovements(const std::pair<int, int>&, Board*) override;
        void calculatePossibleBasicMovements(const std::pair<int, int>& initialPosition, Board* board) override;
        bool legalKingMove(const std::pair<int, int>& mouvement, Board* board);
        char getName() const override;
        void clearMovements();
        std::vector<std::pair<int, int>> getPossibleMovements() const;

    private:
        char pieceName_;
        std::vector<std::pair<int, int>> possibleNextMoves_;
        const std::vector<std::pair<int, int>> possibleMovements_={
        {1, 0}, {-1, 0}, {1, 1}, {1, -1},
        {-1, 1}, {-1, -1}, {0, 1}, {0, -1}};
    };

    class Rook : public Piece {
    public:
        Rook(const Color&);

        void addMovement(const std::pair<int, int>&) override;
        //void calculatePossibleMovements(const std::pair<int, int>&, Board*) override;
        void calculatePossibleSimpleMovements(const std::pair<int, int>&, Board*) override;
        void calculatePossibleBasicMovements(const std::pair<int, int>& initialPosition, Board* board) override;
        char getName() const override;
        void clearMovements();
        std::vector<std::pair<int, int>> getPossibleMovements() const;

    private:
        char pieceName_;
        std::vector<std::pair<int, int>> possibleNextMoves_;
        const std::vector<std::pair<int, int>> possibleMovements_= { 
            {1, 0}, {-1, 0}, {0, 1}, {0, -1} };
        bool verifyCheck_ = false;
    };

    class Knight : public Piece {
    public:
        Knight(const Color&);

        void addMovement(const std::pair<int, int>&) override;
        //void calculatePossibleMovements(const std::pair<int, int>&, Board*) override;
        void calculatePossibleSimpleMovements(const std::pair<int, int>&, Board*) override;
        /*void calculatePossibleMovements(const std::pair<int, int>& initialPosition, Board* board);*/
        void calculatePossibleBasicMovements(const std::pair<int, int>& initialPosition, Board* board) override;
        char getName() const override;
        void clearMovements();
        std::vector<std::pair<int, int>> getPossibleMovements() const;

    private:
        char pieceName_;
        std::vector<std::pair<int, int>> possibleNextMoves_;
        const std::vector<std::pair<int, int>> possibleMovements_ = {
            {+2, +1}, {+2, -1}, {-2, +1}, {-2, -1},
            {+1, +2}, {+1, -2}, {-1, +2}, {-1, -2}};
    };

//Tile de Board
    class Tile
    {
    public:
        Tile(const std::string&, bool);

        void setOccupyingPiece(std::unique_ptr<Piece>);
        std::string	getTileName()		const;
        bool getIsOccupied()		const;
        Piece* getOccupyingPiece() const;
        void destroyOccupyingPiece();
        std::unique_ptr<Piece>	changePossessingPiece();
        void reverseOccupation();


    private:
        std::string name_;
        bool isOccupied_;
        std::unique_ptr<Piece>	occupyingPiece_;
    };
//Board
    class Board
    {
    public:
        Board(const Color&);

        std::unique_ptr<Tile> createTile(const std::string&, bool) const;
        std::unique_ptr<Piece> createPiece(char) const;
        void resetNumberOfKings();
        void create(const std::vector<std::pair<int, int>>& positions, const std::vector<char>& pieces);
        void reset();
        std::pair<Tile*, std::pair<int, int>> findTile(const char) const;
        void calculatePossibleMoves(Piece*, const std::pair<int, int>&);
        void resetValidPiecePositions();
        void invertTurn();
        void setTurn(const Color&);
        Color getTurn() const;
        void movePiece(Tile*, Tile*);
        Tile* getTile(const std::pair<int, int>&) const;
        void invertCheck();
        bool testCheckAfterMove(Tile*);
        bool testCheckProtection(const std::pair<int, int>&, const std::pair<int, int>&);
        bool getCheckState() const;
        std::pair<int, int> getTilePosition(Tile*) const;
        bool testCheckAfterOpponentMove(Tile* finalTile);
        bool testUnprotectedCheck(const std::pair<int, int>&, const std::pair<int, int>& movement);
        //bool legalKingMove(const std::pair<int, int>&);
        void setIsCheck();
        bool isDraw() const;
        bool isCheckmate();
        //void updateKingsTiles(const char color,QPoint);

    protected:
        std::unique_ptr<Tile> board_[BOARD_DIMENSION_X][BOARD_DIMENSION_Y];

    private:
        Color turn_;
        bool check_ = false;
        inline static int nKings_ = 0;
        //std::map<char, QPoint> kingsTiles;
        const char* tileNames_[NUMBER_OF_TILES] = {
            "A8", "B8", "C8", "D8", "E8", "F8", "G8", "H8",
            "A7", "B7", "C7", "D7", "E7", "F7", "G7", "H7",
            "A6", "B6", "C6", "D6", "E6", "F6", "G6", "H6",
            "A5", "B5", "C5", "D5", "E5", "F5", "G5", "H5",
            "A4", "B4", "C4", "D4", "E4", "F4", "G4", "H4",
            "A3", "B3", "C3", "D3", "E3", "F3", "G3", "H3",
            "A2", "B2", "C2", "D2", "E2", "F2", "G2", "H2",
            "A1", "B1", "C1", "D1", "E1", "F1", "G1", "H1"
        };
    };

    class CorrectNumberofKings : public std::invalid_argument
    {
    public:
        using invalid_argument::invalid_argument;
    };

//RAII
     class RAII{
     public:
         RAII(const std::pair<int, int>& from, const std::pair<int, int>& to,Board* board);
         ~RAII();
     private:
         Board* board_;
         std::unique_ptr <config::Piece> savedPiece_;
         std::unique_ptr <config::Piece> ourPiece_;
         config::Tile* startTile_;
         config::Tile* newTile_;
     };
};
