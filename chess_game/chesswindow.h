#ifndef CHESSWINDOW_H
#define CHESSWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include "structure.h"
#include <vector>
#include <Qhash>
#include <QMouseEvent>

QT_BEGIN_NAMESPACE
namespace Ui {class ChessWindow;}
QT_END_NAMESPACE

class ChessWindow : public QMainWindow
{
    Q_OBJECT

public:
    ChessWindow(QWidget *parent = nullptr);
    ~ChessWindow();
public slots:
    void PieceMoved(const QPoint& from, const QPoint& to);

signals:
    void clicked(const QPoint& from, const QPoint& to);

private:
    Ui::ChessWindow* ui;
    bool gameOn = true;
    config::Board board;
    const int tileSize = 60;
    QGraphicsScene* scene;
    QGraphicsRectItem* tileRects[config::BOARD_DIMENSION_Y][config::BOARD_DIMENSION_X];

    struct Scenario {
        std::vector<QPoint> position;
        std::vector<QChar> piece;
    };
    std::vector<Scenario> possiblescenarios;
    QHash<QPoint, QGraphicsPixmapItem*> pieceGraphics;
    //bool firstGame = false;
    bool SecondClickOn = false;
    QPoint firstClickPos;

    //void drawBoard();
    void scenarioSelector(int index);
    void createScenarios();
    void setScenario(const Scenario& possiblescenario);
    void movePiece(const QPoint& from, const QPoint& to);
    void mousePressEvent(QMouseEvent* event) override;
    void DrawDialog(const QString& reason,QString res);
    void startingSide();
    void resetTileColors();

};
#endif // CHESSWINDOW_H
