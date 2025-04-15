#include "mainwindow.h"
#include <QApplication>
#include <QStyleFactory>
#include <QFile>
#include <QDebug>

void setupStyle(QApplication& app)
{
    // Set fusion style for a modern look
    app.setStyle(QStyleFactory::create("Fusion"));
    
    // Set modern color palette
    QPalette palette;
    palette.setColor(QPalette::Window, QColor(53, 53, 53));
    palette.setColor(QPalette::WindowText, Qt::white);
    palette.setColor(QPalette::Base, QColor(25, 25, 25));
    palette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
    palette.setColor(QPalette::ToolTipBase, Qt::white);
    palette.setColor(QPalette::ToolTipText, Qt::white);
    palette.setColor(QPalette::Text, Qt::white);
    palette.setColor(QPalette::Button, QColor(53, 53, 53));
    palette.setColor(QPalette::ButtonText, Qt::white);
    palette.setColor(QPalette::BrightText, Qt::red);
    palette.setColor(QPalette::Link, QColor(42, 130, 218));
    palette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    palette.setColor(QPalette::HighlightedText, Qt::black);
    
    app.setPalette(palette);
    
    // Load and apply stylesheet
    QFile styleFile(":/styles/dark.qss");
    if (styleFile.exists() && styleFile.open(QFile::ReadOnly | QFile::Text)) {
        QString style = QLatin1String(styleFile.readAll());
        app.setStyleSheet(style);
        styleFile.close();
    } else {
        // Fallback stylesheet for modern look
        app.setStyleSheet(R"(
            QMainWindow {
                background-color: #353535;
            }
            QWidget {
                background-color: #353535;
                color: white;
            }
            QPushButton {
                background-color: #2A82DA;
                border: none;
                color: white;
                padding: 5px 15px;
                border-radius: 3px;
            }
            QPushButton:hover {
                background-color: #3292EA;
            }
            QPushButton:pressed {
                background-color: #1A72CA;
            }
            QLineEdit, QComboBox, QSpinBox, QDateEdit, QTimeEdit {
                background-color: #252525;
                border: 1px solid #555555;
                border-radius: 3px;
                padding: 2px 5px;
                color: white;
            }
            QTableWidget {
                background-color: #252525;
                alternate-background-color: #2A2A2A;
                gridline-color: #555555;
            }
            QHeaderView::section {
                background-color: #404040;
                color: white;
                padding: 5px;
                border: 1px solid #555555;
            }
            QTabWidget::pane {
                border: 1px solid #555555;
            }
            QTabBar::tab {
                background-color: #353535;
                color: white;
                padding: 8px 15px;
                border: 1px solid #555555;
                border-bottom: none;
            }
            QTabBar::tab:selected {
                background-color: #2A82DA;
            }
            QGroupBox {
                border: 1px solid #555555;
                border-radius: 3px;
                margin-top: 1em;
                padding-top: 1em;
            }
            QGroupBox::title {
                color: white;
                subcontrol-origin: margin;
                left: 10px;
                padding: 0 3px;
            }
        )");
    }
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // Set application information
    QApplication::setApplicationName("AstroProQt");
    QApplication::setApplicationVersion("1.0.0");
    QApplication::setOrganizationName("AstroProQt");
    QApplication::setOrganizationDomain("astroproqt.org");
    
    // Setup modern style
    setupStyle(app);
    
    // Create and show main window
    MainWindow mainWindow;
    mainWindow.show();
    
    // Start event loop
    return app.exec();
}
