#include "structure.h"
#include <QMessageBox>
#include <QString>
#include <QPoint>
#include "utils.h"
#include <memory>

config::Board::Board(const Color& color): turn_(color){}

std::unique_ptr<config::Tile> config::Board::createTile(const std::string& name, bool occupied) const
{
    return std::make_unique<Tile>(name, occupied);
}


std::unique_ptr<config::Piece> config::Board::createPiece(char charc) const
{
    using namespace std;

    if (charc == BLACK_KNIGHT)
        return make_unique<Knight>(Color::Black);

    else if (charc == WHITE_KNIGHT)
        return make_unique<Knight>(Color::White);

    else if (charc == BLACK_ROOK)
        return make_unique<Rook>(Color::Black);

    else if (charc == WHITE_ROOK)
        return make_unique<Rook>(Color::White);

    else if (charc == WHITE_KING) {
        ++nKings_;
        return make_unique<King>(Color::White);
    }
    else if (charc == BLACK_KING) {
        ++nKings_;
        return make_unique<King>(Color::Black);
    }
    return nullptr;
}

void config::Board::resetNumberOfKings()
{
    nKings_ = 0;
}


void config::Board::reset() {
    for (auto& row : board_) {
        for (auto& tile : row) {
            if (tile) {
                tile->destroyOccupyingPiece();
            }
        }
    }
    turn_=Color::White;
    check_ = false;
    resetNumberOfKings();

}

void config::Board::create(const std::vector<std::pair<int, int>>& positions, const std::vector<char>& pieces)
{
    using namespace iter;

    for (int x = 0; x < BOARD_DIMENSION_Y; ++x) {
        for (int y = 0; y < BOARD_DIMENSION_X; ++y) {
            board_[x][y] = createTile(tileNames_[BOARD_DIMENSION_X * y + x], false);
        }
    }

    for (int i = 0; i < positions.size(); ++i) {
        int x = positions[i].first;
        int y = positions[i].second;

        board_[x][y]->setOccupyingPiece(createPiece(pieces[i]));

        if (nKings_ > MAXIMUM_KINGS_CONFRONTATION)
            throw CorrectNumberofKings(KING_OVER_LIMIT);

    }
    if (nKings_ < MAXIMUM_KINGS_CONFRONTATION)
        throw CorrectNumberofKings(KING_BELOW_LIMIT);
}

std::pair<config::Tile*, std::pair<int, int>> config::Board::findTile(const char pieceName) const
{
    using namespace iter;

    for (int x : range(BOARD_DIMENSION_Y))
        for (int y : range(BOARD_DIMENSION_X))
            if (board_[x][y]->getIsOccupied() && board_[x][y]->getOccupyingPiece()->getName() ==pieceName)
                    return { board_[x][y].get(), { x, y } };

    return { nullptr, { 0,0 } };
}



void config::Board::calculatePossibleMoves(Piece* piece, const std::pair<int, int>& initialPosition)
{
    piece->calculatePossibleSimpleMovements(initialPosition, this);
}

void config::Board::resetValidPiecePositions()
{
    using namespace iter;

    for (int y : range(BOARD_DIMENSION_Y))
        for (int x : range(BOARD_DIMENSION_X))
            if (board_[y][x]->getIsOccupied())
                if (board_[y][x]->getOccupyingPiece()->getColor() == turn_)
                    calculatePossibleMoves(board_[y][x]->getOccupyingPiece(), { y, x });
}

void config::Board::invertTurn()
{
    if (turn_ == Color::White)
        turn_ = Color::Black;
    else
        turn_ = Color::White;
}


void config::Board::setTurn(const config::Color& Color)
{
    turn_ = Color;
}


config::Color config::Board::getTurn() const
{
    return turn_;
}



std::pair<int, int> config::Board::getTilePosition(Tile* tile) const
{
    using namespace iter;

    for (int x : range(BOARD_DIMENSION_Y))
        for (int y : range(BOARD_DIMENSION_X))
            if (board_[x][y].get() == tile)
                return { x,y };

    return { 0, 0 };
}



bool config::Board::testCheckAfterMove(Tile* finalTile)
{
    using namespace std;

    finalTile->getOccupyingPiece()->calculatePossibleSimpleMovements(getTilePosition(finalTile), this);

    const vector<pair<int, int>> MOUVEMENTS_POSSIBLES = finalTile->getOccupyingPiece()->getPossibleMovements();
    char KingChar = (getTurn() == Color::White) ? BLACK_KING : WHITE_KING;
    const std::pair<Tile*, std::pair<int, int>> king = findTile(KingChar);
    auto enemyKing = king.second;

    for (auto mouvement : MOUVEMENTS_POSSIBLES) {
        if (enemyKing == mouvement) {
            return true;
        }
    }

    return false;
}

bool config::Board::testCheckAfterOpponentMove(Tile* finalTile)
{
    using namespace std;

    invertTurn();
    finalTile->getOccupyingPiece()->calculatePossibleBasicMovements(getTilePosition(finalTile), this);
     
    const vector<pair<int, int>> MOUVEMENTS_POSSIBLES = finalTile->getOccupyingPiece()->getPossibleMovements();

    for (auto mouvement : MOUVEMENTS_POSSIBLES)
        if (getTile(mouvement)->getIsOccupied() && 
            getTile(mouvement)->getOccupyingPiece()->getName() == (Color::Black == turn_ ? WHITE_KING : BLACK_KING)) 
        {
            invertTurn();
            return true;
        }
    invertTurn();
    return false;
}


void config::Board::invertCheck()
{
        check_ = !check_;
}

void config::Board::setIsCheck() {
    check_ = true;
}

bool config::Board::getCheckState() const
{
    return check_;
}



void config::Board::movePiece(Tile* initialTile, Tile* finalTile)
{
    if (finalTile->getIsOccupied())
        finalTile->destroyOccupyingPiece();

    finalTile->setOccupyingPiece(initialTile->changePossessingPiece());
    initialTile->destroyOccupyingPiece();
    finalTile->getOccupyingPiece()->setColor(turn_);
    if (testCheckAfterMove(finalTile)) {
        check_ = true;
    }
    else {
        check_ = false;
    }
    invertTurn();
}

config::Tile* config::Board::getTile(const std::pair<int, int>& position) const
{
    return board_[position.first][position.second].get();
}


bool config::Board::testCheckProtection(const std::pair<int, int>& initialPosition, const std::pair<int, int>& mouvement)
{
    using namespace iter;
    //if (!check_) {
    //    Debug::show("testCheckProtection was called while not in check.");
    //    return false;
    //}

    Tile* posTile = getTile(mouvement);
    Tile* initialTile = getTile(initialPosition);


    if (!initialTile || !initialTile->getIsOccupied()) {
        return false;
    }

    initialTile->getOccupyingPiece()->calculatePossibleBasicMovements(initialPosition, this);
    const auto& validMoves = initialTile->getOccupyingPiece()->getPossibleMovements();
    auto canMove = std::find(validMoves.begin(), validMoves.end(), mouvement);
    if (canMove == validMoves.end()) {
        return false;
    }

    bool originalCheck = check_;

    std::unique_ptr <config::Piece> savedPiece = nullptr;
    if (posTile->getIsOccupied()) {
        if (posTile->getOccupyingPiece()->getColor() == turn_)
            return false;
        savedPiece = posTile->changePossessingPiece();
    }

    posTile->setOccupyingPiece(initialTile->changePossessingPiece());
    initialTile->reverseOccupation();

    char KingChar = (getTurn() == config::Color::White) ? WHITE_KING : BLACK_KING;
    const std::pair<Tile*, std::pair<int, int>> king = findTile(KingChar);

    if (!king.first) {
        initialTile->setOccupyingPiece(posTile->changePossessingPiece());
        posTile->reverseOccupation();
        if (savedPiece) posTile->setOccupyingPiece(std::move(savedPiece));
        return false;
    }
     
    invertTurn();
    invertCheck();

    for (int x : range(BOARD_DIMENSION_Y)) {
        for (int y : range(BOARD_DIMENSION_X))
            if (board_[x][y]->getIsOccupied() &&
                board_[x][y]->getOccupyingPiece()->getColor() == turn_)
            {
                if (testCheckAfterMove(board_[x][y].get())) {
                    initialTile->setOccupyingPiece(posTile->changePossessingPiece());
                    posTile->reverseOccupation();
                    if (savedPiece) {
                        posTile->setOccupyingPiece(std::move(savedPiece));
                    }
                    invertCheck();
                    invertTurn();
                    return false;
                }
            }
    }
    initialTile->setOccupyingPiece(posTile->changePossessingPiece());
    posTile->reverseOccupation();
    if (savedPiece){
        posTile->setOccupyingPiece(std::move(savedPiece));
    }
    
    //check_ = originalCheck;
    invertTurn();
    return true;
}

bool config::Board::testUnprotectedCheck(const std::pair<int, int>& initialPosition, const std::pair<int, int>& mouvement)
{
    using namespace std;
    using namespace iter;

    Tile* initialTile = getTile(initialPosition);
    Tile* posTile = getTile(mouvement);
    bool				check = false;
    bool				attackingPiece = false;
    unique_ptr<Piece>	attackingPieceTemp;

    if (posTile->getIsOccupied()) {
        attackingPieceTemp = posTile->changePossessingPiece();
        attackingPiece = true;
    }

    posTile->setOccupyingPiece(initialTile->changePossessingPiece());
    initialTile->reverseOccupation();

    char enemyKingChar = (turn_ == Color::White) ? BLACK_KING : WHITE_KING;

    for (int y : range(BOARD_DIMENSION_Y)) {
        for (int x : range(BOARD_DIMENSION_X)) {
            if (board_[y][x]->getIsOccupied() && board_[y][x]->getOccupyingPiece()->getColor() != turn_ && board_[y][x]->getOccupyingPiece()->getName() != enemyKingChar) {
                if (testCheckAfterOpponentMove(board_[y][x].get())) {
                    check = true;
                    break;
                }

            }
        }
        if (check) {
            break;
        }
    }

    initialTile->setOccupyingPiece(posTile->changePossessingPiece());
    posTile->reverseOccupation();

    if (attackingPiece)
        posTile->setOccupyingPiece(std::move(attackingPieceTemp));

    return check;
}

bool config::Board::isDraw() const {
    int whiteKing = 0, blackKing = 0;
    int whiteKnights = 0, blackKnights = 0;
    int otherPieces = 0;

    for (int x = 0; x < BOARD_DIMENSION_X; ++x) {
        for (int y = 0; y < BOARD_DIMENSION_Y; ++y) {
            const auto& tile = board_[x][y];
            if (tile->getIsOccupied()) {
                char name = tile->getOccupyingPiece()->getName();
                switch (name) {
                case WHITE_KING: whiteKing++; break;
                case BLACK_KING: blackKing++; break;
                case WHITE_KNIGHT: whiteKnights++; break;
                case BLACK_KNIGHT: blackKnights++; break;
                default: otherPieces++;
                }
            }
        }
    }

    // Two kings and no other piece
    if (whiteKing == 1 && blackKing == 1 && whiteKnights == 0 && blackKnights == 0 && otherPieces == 0)
        return true;

    // Two kings and exactly one knight
    if (whiteKing == 1 && blackKing == 1 && otherPieces == 0) {
        if ((whiteKnights == 1 && blackKnights == 0) || (whiteKnights == 0 && blackKnights == 1))
            return true;
    }

    return false;
}

bool config::Board::isCheckmate(){
    using namespace iter;
    if (!check_)
        return false;  

    char kingChar = (turn_ == Color::White) ? WHITE_KING : BLACK_KING;
    auto [kingTile, kingPos] = findTile(kingChar);
    if (!kingTile) return false; 

    Piece* kingPiece = kingTile->getOccupyingPiece();
    kingPiece->calculatePossibleSimpleMovements(kingPos, this);
    const auto& kingMoves = kingPiece->getPossibleMovements();
    if (!kingMoves.empty())
        return false;  

    //for (int y : range(BOARD_DIMENSION_Y)) {
    //    for (int x : range(BOARD_DIMENSION_X)) {
    //        Tile* tile = board_[y][x].get();
    //        if (!tile->getIsOccupied()) 
    //            continue;
    //        Piece* piece = tile->getOccupyingPiece();
    //        if (piece->getColor() != turn_) 
    //            continue;
    //        if (piece->getName() == kingChar) 
    //            continue; 
    //        piece->calculatePossibleSimpleMovements({ y, x }, this);
    //        const auto& pMoves = piece->getPossibleMovements();
    //        for (const auto& to : pMoves) {

    //            if (testCheckProtection({ y, x }, to)) {
    //                return false; 
    //            }
    //        }
    //    }
    //}
    return true;
}


