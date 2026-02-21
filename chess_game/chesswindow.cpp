#include "chesswindow.h"
#include "ui_chesswindow.h"
#include "structure.h"
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsPixmapItem>
#include <QMessageBox>
#include <QAbstractButton>
#include <memory>
#include <QPoint>
#include <QString>
#include "utils.h"
#include <QDialog>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>


ChessWindow::ChessWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ChessWindow), board(config::Color::White), scene(new QGraphicsScene(this))
{
    ui->setupUi(this);

    scene->setSceneRect(0, 0, config::BOARD_DIMENSION_X * tileSize, config::BOARD_DIMENSION_Y * tileSize);
    ui->graphicsView->setScene(scene);

    for (int row = 0; row < config::BOARD_DIMENSION_Y; ++row) {
        for (int col = 0; col < config::BOARD_DIMENSION_X; ++col) {
            QColor color = (row + col) % 2 == 0 ? QColor(255, 238, 210) : QColor(222, 180, 130);
            QGraphicsRectItem* square = scene->addRect(row * tileSize, col * tileSize, tileSize, tileSize, QPen(QColor(190, 190, 190)), QBrush(color));
            tileRects[row][col] = square;
            square->setZValue(0);
        }
    }

    createScenarios();

    connect(this, &ChessWindow::clicked, this, &ChessWindow::PieceMoved);
    connect(ui->scenarioSelector, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ChessWindow::scenarioSelector);
}

ChessWindow::~ChessWindow()
{
    delete ui;
}

void ChessWindow::createScenarios() {
    possiblescenarios = {
        { { {1, 3}, {6, 6}, {0, 5}, {7, 0}, {3, 4}, {4, 4} }, {config::WHITE_ROOK, config::WHITE_ROOK, config::WHITE_KING,config::BLACK_ROOK, config::BLACK_ROOK,config::BLACK_KING} },
        { { {1, 5}, {6, 6}, {4, 6}, {4, 0}, {2, 3}, {1, 2} }, {config::WHITE_KNIGHT, config::WHITE_KNIGHT, config::WHITE_KING, config::BLACK_KNIGHT, config::BLACK_KNIGHT,config::BLACK_KING} },
        { { {4, 4}, {6, 7}, {4, 7}, {1, 5}, {0, 3}, {2, 0} }, {config::WHITE_KNIGHT, config::WHITE_ROOK, config::WHITE_KING,config::BLACK_KNIGHT,config::BLACK_KNIGHT,config::BLACK_KING} },
        { { {1, 7}, {4, 7}, {2, 6}, {5, 4}, {3, 3}, {2, 0} }, {config::WHITE_KNIGHT, config::WHITE_ROOK, config::WHITE_KING,config::BLACK_ROOK,config::BLACK_ROOK,config::BLACK_KING} },
        { { {3, 7}, {2, 5}, {5, 7}, {7, 4}, {0, 7}, {1, 2} }, {config::WHITE_KNIGHT, config::WHITE_ROOK, config::WHITE_KING,config::BLACK_KNIGHT,config::BLACK_ROOK,config::BLACK_KING} },
    };
    ui->scenarioSelector->addItem("🧩 Chosir scenario");
    ui->scenarioSelector->addItem("2 tours vs 2 tours");
    ui->scenarioSelector->addItem("2 cavaliers vs 2 cavaliers");
    ui->scenarioSelector->addItem("1 cavalier-1 tour vs 2 cavaliers");
    ui->scenarioSelector->addItem("1 cavalier-1 tour vs 2 tours");
    ui->scenarioSelector->addItem("1 cavalier-1 tour vs 1 cavalier-1 tour");

}


void ChessWindow::setScenario(const ChessWindow::Scenario& scenario) {
    //clear
    QGraphicsScene* scene = ui->graphicsView->scene();
    scene->clear();
    pieceGraphics.clear();
    board.reset();

    for (int row = 0; row < config::BOARD_DIMENSION_Y; ++row) {
        for (int col = 0; col < config::BOARD_DIMENSION_X; ++col) {
            QColor color = (row + col) % 2 == 0 ? QColor(255, 238, 210) : QColor(222, 180, 130);
            QGraphicsRectItem* square = scene->addRect(col * tileSize, row * tileSize, tileSize, tileSize, QPen(QColor(190, 190, 190)), QBrush(color));
            tileRects[row][col] = square;
            square->setZValue(0);
        }
    }

    std::vector<char> charVector;
    std::vector<std::pair<int, int>> posPairs;
    for (int i = 0; i < scenario.position.size(); ++i) {
        QPoint pos = scenario.position[i];
        posPairs.push_back(std::make_pair(pos.x(), pos.y()));
        charVector.push_back(scenario.piece[i].toLatin1());
        QString pieceImagePath;

        switch (scenario.piece[i].toLatin1()) {
        case config::WHITE_ROOK: pieceImagePath = "images/rookw"; break;
        case config::BLACK_ROOK: pieceImagePath = "images/rookb"; break;
        case config::WHITE_KNIGHT: pieceImagePath = "images/horsew"; break;
        case config::BLACK_KNIGHT: pieceImagePath = "images/horseb"; break;
        case config::BLACK_KING: pieceImagePath = "images/kingb"; break;
        case config::WHITE_KING: pieceImagePath = "images/kingw"; break;
        }

        QPixmap pixmap(pieceImagePath);
        auto pieceItem = new QGraphicsPixmapItem(pixmap);
        pieceItem->setPos(pos.x() * tileSize, pos.y() * tileSize);
        scene->addItem(pieceItem);
        pieceGraphics[pos] = pieceItem;
        pieceItem->setZValue(0);

    }
    board.create(posPairs, charVector);
    SecondClickOn = false;
    gameOn = true;
    startingSide();


}

void ChessWindow::resetTileColors() {
    for (int row = 0; row < config::BOARD_DIMENSION_Y; ++row) {
        for (int col = 0; col < config::BOARD_DIMENSION_X; ++col) {
            QColor color = (row + col) % 2 == 0
                ? QColor(255, 238, 210)   // light beige
                : QColor(222, 180, 130);  // brownish
            tileRects[row][col]->setBrush(QBrush(color));
        }
    }
}

void ChessWindow::scenarioSelector(int index) {
    if (index > 0 && index <= possiblescenarios.size()) {
        setScenario(possiblescenarios[index - 1]);
    }
}

void ChessWindow::startingSide() {
    QDialog dialog(this);
    dialog.setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint);
    dialog.setWindowTitle("");

    // Set dialog background color
    dialog.setStyleSheet("background-color: #eeeeee;");  
    QVBoxLayout* layout = new QVBoxLayout(&dialog);
    QLabel* label = new QLabel("Qui doit commencer la partie ? ");
    QPushButton* whiteBtn = new QPushButton("Blanc");
    QPushButton* blackBtn = new QPushButton("Noir");

    whiteBtn->setStyleSheet("background-color: #ffffff; color: black; font-weight: bold; padding: 8px;");
    blackBtn->setStyleSheet("background-color: #000000; color: white; font-weight: bold; padding: 8px;");

    layout->addWidget(label);
    layout->addWidget(whiteBtn);
    layout->addWidget(blackBtn);

    dialog.setWindowModality(Qt::ApplicationModal);
    dialog.setWindowFlag(Qt::WindowCloseButtonHint, false);

    connect(whiteBtn, &QPushButton::clicked, [&]() {
        board.setTurn(config::Color::White);
        dialog.accept();
        });

    connect(blackBtn, &QPushButton::clicked, [&]() {
        board.setTurn(config::Color::Black);
        dialog.accept();
        });

    dialog.exec();  
}


void ChessWindow::PieceMoved(const QPoint& from, const QPoint& to) {
    if (!pieceGraphics.contains(from)) return;
    auto item = pieceGraphics[from];


    if (pieceGraphics.contains({ to.x(), to.y() })) {
        QGraphicsPixmapItem* capturedItem = pieceGraphics[to];
        scene->removeItem(capturedItem);
        delete capturedItem;
        pieceGraphics.remove(to);
    }
    item->setPos(to.x() * tileSize, to.y() * tileSize);
    pieceGraphics.remove(from);
    pieceGraphics[to] = item;

    resetTileColors();

    if (board.isDraw()) {
        gameOn = false;
        auto msg = QString("Partie nulle !");
        DrawDialog("matériel insuffisant", msg);

    }
}

void ChessWindow::movePiece(const QPoint& from, const QPoint& to) {
    auto tile = board.getTile({ from.x(), from.y() });
    if (!tile->getIsOccupied()) {
        return;
    }
    if (tile->getOccupyingPiece()->getColor() != board.getTurn()) {
        Debug::show("Ce n’est pas votre tour");
        return;
    }
    auto piece = tile->getOccupyingPiece();
    piece->calculatePossibleSimpleMovements({ from.x(), from.y() }, &board);
    auto validMoves = piece->getPossibleMovements();
    auto canMove = std::find(validMoves.begin(), validMoves.end(), std::make_pair(to.x(), to.y()));

    if (board.getCheckState() && !board.testCheckProtection({ from.x(), from.y() }, { to.x(), to.y() })) {
        if (board.isCheckmate()) {
            gameOn = false;
            QString msg = (board.getTurn() == config::Color::White)
                ? "Victoire des noirs !"  
                : "Victoire des blancs !";
            DrawDialog("Échec et mat !",msg);
            return;
        }
        for (const auto& move : validMoves) {
            QGraphicsRectItem* tile = tileRects[move.second][move.first];
            tile->setBrush(QColor(0, 255, 0, 127));  // semi-transparent green
            tile->setZValue(-1);
        }
        Debug::show("Vous devez parer l’échec");
        return;
    }

    if (canMove != validMoves.end()) {
        board.movePiece(tile, board.getTile({ to.x(), to.y() }));
        emit clicked(from, to);
        if (board.getCheckState()) {
            if (board.isCheckmate()) {
                gameOn = false;
                QString msg = (board.getTurn() == config::Color::White)
                    ? "Victoire de noir !"
                    : "Victoire de blanc !";
                DrawDialog("Échec et mat !", msg);
                return;
            }
            char KingChar = (board.getTurn() == config::Color::White) ? config::WHITE_KING : config::BLACK_KING;
            const auto king = board.findTile(KingChar);
            if (king.first) {
                QGraphicsRectItem* kingTile = tileRects[king.second.second][king.second.first];
                kingTile->setBrush(QColor(255, 0, 0, 127));
                kingTile->setZValue(-1);
            }
        }
        board.resetValidPiecePositions();
    }
    else{
        Debug::show(board.getCheckState()
            ? "Vous devez parer l’échec"
            : "Vous ne pouvez pas vous déplacer ici");
        for (const auto& move : validMoves) {
            QGraphicsRectItem* tile = tileRects[move.second][move.first];  
            tile->setBrush(QColor(0, 255, 0, 127));  // semi-transparent green
            tile->setZValue(-1);  
        }
    }

}

void ChessWindow::mousePressEvent(QMouseEvent* event) 
{
    resetTileColors();
    if (!gameOn) {
        return;
    }
    if (board.getCheckState()) {
        const std::pair<config::Tile*, std::pair<int, int>> king = board.findTile((board.getTurn() == config::Color::White) ? config::WHITE_KING : config::BLACK_KING);
        QGraphicsRectItem* kingTile = tileRects[king.second.second][king.second.first];
        kingTile->setBrush(QColor(255, 0, 0, 127));  // semi-transparent red
        kingTile->setZValue(-1);  
    }
    if (!ui->graphicsView->underMouse())
        return;

    QPoint viewPos = ui->graphicsView->mapFrom(this, event->pos());
    QPointF scenePos = ui->graphicsView->mapToScene(viewPos);

    QGraphicsItem* clickedItem = scene->itemAt(scenePos, QTransform());

    if (!clickedItem) {
        return;
    }

    QPoint tileCoord;  

    if (auto* pieceItem = dynamic_cast<QGraphicsPixmapItem*>(clickedItem)) {

        //piece 
        tileCoord.setX(pieceItem->pos().x() / tileSize);
        tileCoord.setY(pieceItem->pos().y() / tileSize);
    }
    else if (auto* rectItem = dynamic_cast<QGraphicsRectItem*>(clickedItem)) {
        //tile
        tileCoord.setX(rectItem->rect().x() / tileSize);
        tileCoord.setY(rectItem->rect().y() / tileSize);
    }

    // 2-click logic
    if (!SecondClickOn) {
        firstClickPos = tileCoord;
        tileRects[firstClickPos.y()][firstClickPos.x()]->setBrush(QColor(230, 180, 70, 100));  // semi-transparent blue
        SecondClickOn = true;

    }
    else {
        QPoint secClickPos = tileCoord;
        SecondClickOn = false;
        movePiece(firstClickPos, secClickPos);
    }
}

void ChessWindow::DrawDialog(const QString& reason, QString res) {
    QDialog dialog(this);
    dialog.setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint);
    dialog.setWindowTitle("Fin du jeux");

    dialog.setStyleSheet("background-color: #eeeeee;");
    QVBoxLayout* layout = new QVBoxLayout(&dialog);

    QLabel* label = new QLabel(res+"\n"+ reason);
    label->setAlignment(Qt::AlignCenter);
    label->setStyleSheet("font-size: 16px; font-weight: bold; padding: 10px;");

    QPushButton* closeBtn = new QPushButton("Close");
    closeBtn->setStyleSheet("background-color: #cccccc; font-weight: bold; padding: 8px;");

    layout->addWidget(label);
    layout->addWidget(closeBtn);

    dialog.setWindowModality(Qt::ApplicationModal);
    dialog.setWindowFlag(Qt::WindowCloseButtonHint, false);

    connect(closeBtn, &QPushButton::clicked, [&]() {
        dialog.accept();
        });

    dialog.exec();  
}
