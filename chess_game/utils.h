#pragma once
#ifndef UTILS_H
#define UTILS_H

#include <QString>
#include <QMessageBox>
#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QIcon>
#include <QPixmap>

namespace Debug {
    inline void show(const QString& message) {
        QDialog dialog;


        dialog.setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);

        // 🎨 Custom styling
        dialog.setStyleSheet(R"(
            QDialog {
                background-color: #f0e6d6;  /* Light chessboard beige */
                border: 2px solid #9c8a62;
                border-radius: 10px;
            }
            QLabel {
                font-size: 16px;
                color: #3e2f1c;
                padding: 10px;
            }
            QPushButton {
                background-color: #d6b980;
                color: #2b1b00;
                font-weight: bold;
                padding: 8px 16px;
                border-radius: 6px;
            }
            QPushButton:hover {
                background-color: #e3c897;
            }
        )");

        QVBoxLayout* layout = new QVBoxLayout(&dialog);

        QLabel* iconLabel = new QLabel;
        QPixmap icon("images/attention");
        iconLabel->setPixmap(icon.scaled(60, 60, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        iconLabel->setAlignment(Qt::AlignCenter);

        QLabel* messageLabel = new QLabel(message);
        messageLabel->setAlignment(Qt::AlignCenter);
        messageLabel->setWordWrap(true);

        QPushButton* okButton = new QPushButton("OK");
        QObject::connect(okButton, &QPushButton::clicked, [&dialog]() { dialog.accept(); });

        layout->addWidget(iconLabel);
        layout->addWidget(messageLabel);
        layout->addWidget(okButton, 0, Qt::AlignCenter);

        dialog.setFixedSize(300, 200);
        dialog.setWindowModality(Qt::ApplicationModal);
        dialog.exec();
    }
}
#endif