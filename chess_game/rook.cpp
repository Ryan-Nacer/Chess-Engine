#include "structure.h"
#include "utils.h"
config::Rook::Rook(const Color& color): Piece(color)
{
    if (color == Color::White)
        pieceName_ = WHITE_ROOK;
    else
        pieceName_ = BLACK_ROOK;
}

void config::Rook::clearMovements() {
    possibleNextMoves_.clear();
}

std::vector<std::pair<int, int>> config::Rook::getPossibleMovements() const {
    return possibleNextMoves_;
}


void config::Rook::addMovement(const std::pair<int, int>& newMove)
{
    possibleNextMoves_.push_back(newMove);
}

void config::Rook::calculatePossibleSimpleMovements(const std::pair<int, int>& initialPosition, Board* board)   //poss mov lekbira
{
    calculatePossibleBasicMovements(initialPosition, board);

    const auto validMoves = getPossibleMovements();
    possibleNextMoves_.clear();

    for (const auto& move : validMoves) {
        //Debug::show("have some moves to test");
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

void config::Rook::calculatePossibleBasicMovements(const std::pair<int, int>& initialPosition, Board* board)
{
    using namespace std;
    possibleNextMoves_.clear();  //clean old moves

    for (const auto& move : possibleMovements_) {
        pair<int, int> newPosition = initialPosition;
        while (true) {
            newPosition.first += move.first;
            newPosition.second += move.second;

            if (!isInsideBounds(newPosition))
                break;

            auto tile = board->getTile(newPosition);
            //QString msg = QString("check for: (%1)")
            //    .arg(tile->getTileName());
            //Debug::show(msg);
            if (!tile->getIsOccupied()) {
                addMovement(newPosition);
            }
            else {
                if (tile->getOccupyingPiece()->getColor() != getColor()) {
                    addMovement(newPosition);
                    if (tile->getOccupyingPiece()->getName() != ((getColor() == config::Color::White) ? config::BLACK_KING : config::WHITE_KING)) {
                        break;
                    }
                }
                else {
                    break;
                }
            }
        }
    }
}


char config::Rook::getName() const
{
    return pieceName_;
}