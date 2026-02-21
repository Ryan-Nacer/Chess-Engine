 #include "structure.h"
 #include <memory>


 config::RAII::RAII(const std::pair<int, int>& from, const std::pair<int, int>& to, Board* board)
     :board_(board), startTile_(board_->getTile(from)),
     newTile_(board_->getTile(to))
 {
     // Sauvegarder la pièce présente sur la case de destination
     if (newTile_->getIsOccupied()) {
        savedPiece_ = newTile_->changePossessingPiece();
     }

     // Déplacer temporairement la pièce source vers la destination
     ourPiece_ = startTile_->changePossessingPiece();
     newTile_->setOccupyingPiece(std::move(ourPiece_));
     startTile_->reverseOccupation();
 }

 config::RAII::~RAII()
 {
     // Remettre la pièce déplacée à sa position initiale
     startTile_->setOccupyingPiece(newTile_->changePossessingPiece());
     newTile_->reverseOccupation();

     /* Restaurer la pièce précédemment présente sur la case de destination (s'il y en avait une)*/
     if (savedPiece_) {
         newTile_->setOccupyingPiece(std::move(savedPiece_));
     }
 }
