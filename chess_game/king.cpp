#include "structure.h"
#include "utils.h"

config::King::King(const Color& color): Piece(color)
{
    if (color == Color::White)
        pieceName_ = WHITE_KING;
    else
        pieceName_ = BLACK_KING;
}


void config::King::addMovement(const std::pair<int, int>& newMove)
{
    possibleNextMoves_.push_back(newMove);
}

void config::King::clearMovements() {
    possibleNextMoves_.clear();
}

std::vector<std::pair<int, int>> config::King::getPossibleMovements() const {
    return possibleNextMoves_;
}

bool config::King::isConfrontingEnemyKing(const std::pair<int, int>& possibleMove, const std::pair<int, int>& enemyKingPosition) const
{
    int dx = abs(possibleMove.first - enemyKingPosition.first);
    int dy = abs(possibleMove.second - enemyKingPosition.second);

    return (std::max(dx, dy) <= 1);
}


void config::King::calculatePossibleSimpleMovements(const std::pair<int, int>& initialPosition, Board* board)   //poss mov lekbira
{
    calculatePossibleBasicMovements(initialPosition, board);

    const auto validMoves = getPossibleMovements();
    possibleNextMoves_.clear();

    for (const auto& move : validMoves) {
        if (!board->getCheckState()) {
            if (!board->testUnprotectedCheck(initialPosition, move))
                addMovement(move);
        }
        else {
            if (board->testCheckProtection(initialPosition, move)) {
                addMovement(move);
            }
        }
    }
}


void config::King::calculatePossibleBasicMovements(const std::pair<int, int>& initialPosition, Board* board)
{
    using namespace std;
    possibleNextMoves_.clear();  
    for (const auto& move : possibleMovements_) {
        pair<int, int> newPosition = { initialPosition.first + move.first, initialPosition.second + move.second };

        if (!isInsideBounds(newPosition))
            continue;
        auto tile = board->getTile(newPosition);

        if (tile->getIsOccupied() &&
            tile->getOccupyingPiece()->getColor() == getColor()) {
            continue;
        }
        char enemyKingChar = (getColor() == Color::White) ? BLACK_KING : WHITE_KING;
        const pair<Tile*, pair<int, int>> enemyKing = board->findTile(enemyKingChar);
        if (isConfrontingEnemyKing(newPosition, enemyKing.second)) {
            continue;
        }
        if (legalKingMove(newPosition,board)) {
                addMovement(newPosition);
        }
    }
}

bool config::King::legalKingMove(const std::pair<int, int>& mouvement,Board* board)
{
    using namespace iter;

    char enemykingChar = (getColor() == Color::White) ? BLACK_KING : WHITE_KING;
    Tile* posTile = board->getTile(mouvement);

    for (int y : range(BOARD_DIMENSION_Y)) {
        for (int x : range(BOARD_DIMENSION_X)) {
            Tile* tile = board->getTile({ y,x });
            if (tile->getIsOccupied() && tile->getOccupyingPiece()->getColor() != getColor()) {
                Piece* enemy = tile->getOccupyingPiece();
                if (enemy->getName() != enemykingChar) {
                    enemy->calculatePossibleBasicMovements({ y, x }, board);
                    for (const auto& pos : enemy->getPossibleMovements()) {
                        Tile* possibleTarget = board->getTile(pos);
                        if (possibleTarget == posTile) {
                            return false;
                        }
                    }
                }
            }
        }
    }
    return true;
}

char config::King::getName() const
{
    return pieceName_;
}
