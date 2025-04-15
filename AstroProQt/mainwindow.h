#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include "chartwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // UI Event Handlers
    void on_generateButton_clicked();
    void on_searchButton_clicked();
    void on_chartStyleCombo_currentIndexChanged(int index);
    void on_showAspectsCheck_stateChanged(int state);
    void on_enableZoomCheck_stateChanged(int state);
    void on_actionExport_triggered();
    void on_actionAbout_triggered();
    void on_actionExit_triggered();

    // Network Response Handlers
    void handleGeocodeResponse(QNetworkReply* reply);

    // Chart Event Handlers
    void handleChartGenerated();
    void handleChartError(const QString& error);
    void handleCalculationsUpdated();

private:
    Ui::MainWindow *ui;
    QNetworkAccessManager* m_networkManager;
    
    // Helper Methods
    void setupConnections();
    void setupTables();
    void generateChart();
    void updateDashaTable();
    void updateStrengthTable();
    void updateYogaTable();
    void showError(const QString& message);
    void searchLocation(const QString& place);
    
    // Data validation
    bool validateInputs();
    
    // Constants
    const QString OPENCAGE_API_KEY = "YOUR_API_KEY"; // Replace with actual API key
    const QString OPENCAGE_API_URL = "https://api.opencagedata.com/geocode/v1/json";
};

#endif // MAINWINDOW_H
