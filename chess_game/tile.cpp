#include "structure.h"

config::Tile::Tile(const std::string& name, bool isOccupied) :
    name_(name), isOccupied_(isOccupied), occupyingPiece_(nullptr)
{
}

void config::Tile::setOccupyingPiece(std::unique_ptr<Piece> newPiece)
{
    occupyingPiece_ = std::move(newPiece);
    isOccupied_ = true;
}

void config::Tile::destroyOccupyingPiece()
{
    occupyingPiece_.reset();
    isOccupied_ = false;
}

std::unique_ptr<config::Piece> config::Tile::changePossessingPiece()
{
    return std::move(occupyingPiece_);
}

void config::Tile::reverseOccupation()
{
    isOccupied_ = !isOccupied_;
}

std::string config::Tile::getTileName() const
{
    return name_;
}

config::Piece* config::Tile::getOccupyingPiece() const
{
    return occupyingPiece_.get();
}

bool config::Tile::getIsOccupied() const
{
    return isOccupied_;
}
