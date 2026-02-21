#include "structure.h"
#include "utils.h"

config::Knight::Knight(const Color& color): Piece(color)
{
    if (color == Color::White)
        pieceName_ = WHITE_KNIGHT;
    else
        pieceName_ = BLACK_KNIGHT;
}

void config::Knight::clearMovements() {
    possibleNextMoves_.clear();
}

std::vector<std::pair<int, int>> config::Knight::getPossibleMovements() const {
    return possibleNextMoves_;
}
void config::Knight::addMovement(const std::pair<int, int>& newMove)
{
    possibleNextMoves_.push_back(newMove);
}


void config::Knight::calculatePossibleSimpleMovements(const std::pair<int, int>& initialPosition, Board* board)   //poss mov lekbira
{
    calculatePossibleBasicMovements(initialPosition,board);
   
    const auto validMoves = getPossibleMovements();
    possibleNextMoves_.clear();

    for (const auto& move : validMoves) {
        if (!board->getCheckState()) {
            if (!board->testUnprotectedCheck(initialPosition, move))
                addMovement(move);
        }else {
            if (board->testCheckProtection(initialPosition, move)) {
                addMovement(move);
            }
        }
    }
}


void config::Knight::calculatePossibleBasicMovements(const std::pair<int, int>& initialPosition, Board* board)
{
    using namespace std;
    possibleNextMoves_.clear(); 

    for (const auto& move : possibleMovements_) {
        pair<int, int> newPosition = { initialPosition.first + move.first, initialPosition.second + move.second };

        if (!isInsideBounds(newPosition))
            continue;
        auto tile = board->getTile(newPosition);
        if (!tile->getIsOccupied()) {
            addMovement(newPosition);
        }
        else if (tile->getOccupyingPiece()->getColor() != getColor()) {
            addMovement(newPosition);
        }
    }
}

char config::Knight::getName() const
{
    return pieceName_;
}

