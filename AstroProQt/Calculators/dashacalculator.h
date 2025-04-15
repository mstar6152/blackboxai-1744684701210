#ifndef DASHACALCULATOR_H
#define DASHACALCULATOR_H

#include <QDateTime>
#include <QString>
#include <QVector>

class DashaPeriod {
public:
    QString planet;
    QDateTime startTime;
    QDateTime endTime;
    QVector<DashaPeriod> antarDashas;
};

class DashaCalculator {
public:
    DashaCalculator();
    
    // Calculate Vimshottari Dasha periods
    QVector<DashaPeriod> calculateVimshottariDasha(const QDateTime& birthTime, double moonLongitude);
    
    // Get current running Dasha
    QString getCurrentDasha(const QDateTime& birthTime, double moonLongitude);
    
    // Calculate Antardasha periods
    QVector<DashaPeriod> calculateAntarDasha(const DashaPeriod& mahadasha);

private:
    // Dasha periods in years for each planet
    const QVector<int> dashaPeriods = {6, 10, 7, 18, 16, 19, 17, 7, 20}; // Sun to Ketu
    
    // Planet names in order
    const QVector<QString> planetNames = {
        "Sun", "Moon", "Mars", "Rahu", "Jupiter",
        "Saturn", "Mercury", "Ketu", "Venus"
    };
    
    // Helper methods
    int findStartingPlanet(double moonLongitude);
    QDateTime calculateDashaStartTime(const QDateTime& birthTime, double moonLongitude);
};

#endif // DASHACALCULATOR_H
