/********************************************************************************
** Form generated from reading UI file 'chesswindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHESSWINDOW_H
#define UI_CHESSWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ChessWindow
{
public:
    QWidget *centralwidget;
    QGraphicsView *graphicsView;
    QComboBox *scenarioSelector;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *ChessWindow)
    {
        if (ChessWindow->objectName().isEmpty())
            ChessWindow->setObjectName("ChessWindow");
        ChessWindow->resize(763, 587);
        centralwidget = new QWidget(ChessWindow);
        centralwidget->setObjectName("centralwidget");
        graphicsView = new QGraphicsView(centralwidget);
        graphicsView->setObjectName("graphicsView");
        graphicsView->setGeometry(QRect(10, 10, 480, 480));
        graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
        graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
        scenarioSelector = new QComboBox(centralwidget);
        scenarioSelector->setObjectName("scenarioSelector");
        scenarioSelector->setGeometry(QRect(530, 50, 171, 21));
        ChessWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(ChessWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 763, 22));
        ChessWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(ChessWindow);
        statusbar->setObjectName("statusbar");
        ChessWindow->setStatusBar(statusbar);

        retranslateUi(ChessWindow);

        QMetaObject::connectSlotsByName(ChessWindow);
    } // setupUi

    void retranslateUi(QMainWindow *ChessWindow)
    {
        ChessWindow->setWindowTitle(QCoreApplication::translate("ChessWindow", "ChessWindow", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ChessWindow: public Ui_ChessWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHESSWINDOW_H
