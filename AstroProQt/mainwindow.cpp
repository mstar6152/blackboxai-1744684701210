#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QNetworkRequest>
#include <QUrlQuery>
#include <QJsonObject>
#include <QJsonArray>
#include <QDateTime>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_networkManager(new QNetworkAccessManager(this))
{
    ui->setupUi(this);
    setupConnections();
    setupTables();
    
    // Set current date/time as default
    QDateTime currentDateTime = QDateTime::currentDateTime();
    ui->dateInput->setDateTime(currentDateTime);
    ui->timeInput->setTime(currentDateTime.time());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupConnections()
{
    // Connect chart signals
    connect(ui->chartWidget, &ChartWidget::chartGenerated,
            this, &MainWindow::handleChartGenerated);
    connect(ui->chartWidget, &ChartWidget::errorOccurred,
            this, &MainWindow::handleChartError);
    connect(ui->chartWidget, &ChartWidget::calculationsUpdated,
            this, &MainWindow::handleCalculationsUpdated);
            
    // Connect network manager
    connect(m_networkManager, &QNetworkAccessManager::finished,
            this, &MainWindow::handleGeocodeResponse);
}

void MainWindow::setupTables()
{
    // Setup Dasha table
    ui->dashaTable->setColumnCount(4);
    ui->dashaTable->setHorizontalHeaderLabels(
        {"Planet", "Start Time", "End Time", "Sub-periods"}
    );
    
    // Setup Strength table
    ui->strengthTable->setColumnCount(7);
    ui->strengthTable->setHorizontalHeaderLabels(
        {"Planet", "Shadbala", "Sthanabala", "Digbala", 
         "Drishti Bala", "Kala Bala", "Naisargika Bala"}
    );
    
    // Setup Yoga table
    ui->yogaTable->setColumnCount(3);
    ui->yogaTable->setHorizontalHeaderLabels(
        {"Yoga Name", "Description", "Strength"}
    );
    
    // Enable sorting
    ui->dashaTable->setSortingEnabled(true);
    ui->strengthTable->setSortingEnabled(true);
    ui->yogaTable->setSortingEnabled(true);
}

void MainWindow::on_generateButton_clicked()
{
    if (!validateInputs()) {
        return;
    }
    generateChart();
}

void MainWindow::on_searchButton_clicked()
{
    QString place = ui->placeInput->text().trimmed();
    if (place.isEmpty()) {
        showError("Please enter a place name");
        return;
    }
    searchLocation(place);
}

void MainWindow::searchLocation(const QString& place)
{
    QUrlQuery query;
    query.addQueryItem("q", place);
    query.addQueryItem("key", OPENCAGE_API_KEY);
    query.addQueryItem("limit", "1");
    
    QUrl url(OPENCAGE_API_URL);
    url.setQuery(query);
    
    QNetworkRequest request(url);
    m_networkManager->get(request);
    
    ui->statusbar->showMessage("Searching location...");
}

void MainWindow::handleGeocodeResponse(QNetworkReply* reply)
{
    reply->deleteLater();
    
    if (reply->error() != QNetworkReply::NoError) {
        showError("Network error: " + reply->errorString());
        return;
    }
    
    QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
    QJsonObject root = doc.object();
    
    if (root["total_results"].toInt() == 0) {
        showError("Location not found");
        return;
    }
    
    QJsonArray results = root["results"].toArray();
    QJsonObject result = results.first().toObject();
    QJsonObject geometry = result["geometry"].toObject();
    
    double lat = geometry["lat"].toDouble();
    double lon = geometry["lng"].toDouble();
    
    ui->latInput->setText(QString::number(lat, 'f', 6));
    ui->lonInput->setText(QString::number(lon, 'f', 6));
    
    ui->statusbar->showMessage("Location found", 3000);
}

void MainWindow::generateChart()
{
    QDateTime birthTime(ui->dateInput->date(), ui->timeInput->time());
    QString place = ui->placeInput->text();
    double lat = ui->latInput->text().toDouble();
    double lon = ui->lonInput->text().toDouble();
    
    ui->chartWidget->setBirthData(birthTime, place, lat, lon);
    ui->chartWidget->generateChart();
}

void MainWindow::handleChartGenerated()
{
    updateDashaTable();
    updateStrengthTable();
    updateYogaTable();
    ui->statusbar->showMessage("Chart generated successfully", 3000);
}

void MainWindow::handleChartError(const QString& error)
{
    showError("Chart generation error: " + error);
}

void MainWindow::handleCalculationsUpdated()
{
    updateDashaTable();
    updateStrengthTable();
    updateYogaTable();
}

void MainWindow::updateDashaTable()
{
    ui->dashaTable->setRowCount(0);
    
    QVector<DashaPeriod> dashaPeriods = ui->chartWidget->getDashaPeriods();
    for (const DashaPeriod& period : dashaPeriods) {
        int row = ui->dashaTable->rowCount();
        ui->dashaTable->insertRow(row);
        
        ui->dashaTable->setItem(row, 0, 
            new QTableWidgetItem(period.planet));
        ui->dashaTable->setItem(row, 1, 
            new QTableWidgetItem(period.startTime.toString("dd-MM-yyyy hh:mm")));
        ui->dashaTable->setItem(row, 2, 
            new QTableWidgetItem(period.endTime.toString("dd-MM-yyyy hh:mm")));
        
        QStringList subPeriods;
        for (const DashaPeriod& sub : period.antarDashas) {
            subPeriods << sub.planet;
        }
        ui->dashaTable->setItem(row, 3, 
            new QTableWidgetItem(subPeriods.join(" → ")));
    }
    
    ui->dashaTable->resizeColumnsToContents();
}

void MainWindow::updateStrengthTable()
{
    ui->strengthTable->setRowCount(0);
    
    auto strengths = ui->chartWidget->getPlanetaryStrengths();
    for (auto it = strengths.begin(); it != strengths.end(); ++it) {
        int row = ui->strengthTable->rowCount();
        ui->strengthTable->insertRow(row);
        
        ui->strengthTable->setItem(row, 0, 
            new QTableWidgetItem(it.key()));
        ui->strengthTable->setItem(row, 1, 
            new QTableWidgetItem(QString::number(it.value().shadbala, 'f', 2)));
        ui->strengthTable->setItem(row, 2, 
            new QTableWidgetItem(QString::number(it.value().sthanaBala, 'f', 2)));
        ui->strengthTable->setItem(row, 3, 
            new QTableWidgetItem(QString::number(it.value().digBala, 'f', 2)));
        ui->strengthTable->setItem(row, 4, 
            new QTableWidgetItem(QString::number(it.value().drishtisBala, 'f', 2)));
        ui->strengthTable->setItem(row, 5, 
            new QTableWidgetItem(QString::number(it.value().kalaBala, 'f', 2)));
        ui->strengthTable->setItem(row, 6, 
            new QTableWidgetItem(QString::number(it.value().naisargikaBala, 'f', 2)));
    }
    
    ui->strengthTable->resizeColumnsToContents();
}

void MainWindow::updateYogaTable()
{
    ui->yogaTable->setRowCount(0);
    
    QVector<YogaCalculator::Yoga> yogas = ui->chartWidget->getActiveYogas();
    for (const auto& yoga : yogas) {
        int row = ui->yogaTable->rowCount();
        ui->yogaTable->insertRow(row);
        
        ui->yogaTable->setItem(row, 0, 
            new QTableWidgetItem(yoga.name));
        ui->yogaTable->setItem(row, 1, 
            new QTableWidgetItem(yoga.description));
        ui->yogaTable->setItem(row, 2, 
            new QTableWidgetItem(QString::number(yoga.strength, 'f', 1) + "%"));
    }
    
    ui->yogaTable->resizeColumnsToContents();
}

void MainWindow::on_chartStyleCombo_currentIndexChanged(int index)
{
    ui->chartWidget->setChartStyle(
        static_cast<ChartWidget::ChartStyle>(index)
    );
}

void MainWindow::on_showAspectsCheck_stateChanged(int state)
{
    ui->chartWidget->setShowAspects(state == Qt::Checked);
}

void MainWindow::on_enableZoomCheck_stateChanged(int state)
{
    ui->chartWidget->enableZoomAndPan(state == Qt::Checked);
}

void MainWindow::on_actionExport_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        "Export Chart", "", "PNG Image (*.png);;JPEG Image (*.jpg)");
        
    if (!fileName.isEmpty()) {
        ui->chartWidget->exportChart(fileName);
        ui->statusbar->showMessage("Chart exported successfully", 3000);
    }
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this, "About AstroProQt",
        "AstroProQt - Professional Astrology Software\n\n"
        "A comprehensive astrology application featuring:\n"
        "• Birth chart calculation and visualization\n"
        "• Dasha period analysis\n"
        "• Planetary strength calculations\n"
        "• Yoga (planetary combinations) detection\n\n"
        "Built with Qt and Swiss Ephemeris"
    );
}

void MainWindow::on_actionExit_triggered()
{
    close();
}

bool MainWindow::validateInputs()
{
    if (ui->nameInput->text().trimmed().isEmpty()) {
        showError("Please enter a name");
        return false;
    }
    
    if (ui->placeInput->text().trimmed().isEmpty()) {
        showError("Please enter a place");
        return false;
    }
    
    if (ui->latInput->text().isEmpty() || ui->lonInput->text().isEmpty()) {
        showError("Please search for a valid location");
        return false;
    }
    
    return true;
}

void MainWindow::showError(const QString& message)
{
    QMessageBox::warning(this, "Error", message);
    ui->statusbar->showMessage("Error: " + message, 3000);
}
