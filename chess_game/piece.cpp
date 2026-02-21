#include "structure.h"

config::Piece::Piece(const Color& color): color_(color){}

config::Color config::Piece::getColor() const
{
    return color_;
}

bool config::Piece::isInsideBounds(const std::pair<int, int>& move) const
{
    if (move.first >= 0 && move.first <= (BOARD_DIMENSION_X - 1) && move.second >= 0 && move.second <= (BOARD_DIMENSION_Y - 1))
        return true;
    return false;

}

void config::Piece::setColor(const Color& color)
{
    color_ = color;
}