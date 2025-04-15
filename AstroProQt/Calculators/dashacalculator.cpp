#include "dashacalculator.h"
#include <QtMath>

DashaCalculator::DashaCalculator() {}

QVector<DashaPeriod> DashaCalculator::calculateVimshottariDasha(const QDateTime& birthTime, double moonLongitude) {
    QVector<DashaPeriod> dashaPeriods;
    
    // Find starting planet based on Moon's longitude
    int startingPlanetIndex = findStartingPlanet(moonLongitude);
    QDateTime startTime = calculateDashaStartTime(birthTime, moonLongitude);
    
    // Calculate all Mahadasha periods
    for (int i = 0; i < 9; ++i) {
        int planetIndex = (startingPlanetIndex + i) % 9;
        
        DashaPeriod period;
        period.planet = planetNames[planetIndex];
        period.startTime = startTime;
        
        // Convert years to milliseconds (considering leap years)
        qint64 periodMs = dashaPeriods[planetIndex] * 365.25 * 24 * 60 * 60 * 1000;
        period.endTime = period.startTime.addMSecs(periodMs);
        
        // Calculate Antardasha for this period
        period.antarDashas = calculateAntarDasha(period);
        
        dashaPeriods.append(period);
        startTime = period.endTime;
    }
    
    return dashaPeriods;
}

QString DashaCalculator::getCurrentDasha(const QDateTime& birthTime, double moonLongitude) {
    QVector<DashaPeriod> allPeriods = calculateVimshottariDasha(birthTime, moonLongitude);
    QDateTime currentTime = QDateTime::currentDateTime();
    
    for (const DashaPeriod& period : allPeriods) {
        if (currentTime >= period.startTime && currentTime < period.endTime) {
            // Find current Antardasha
            for (const DashaPeriod& antardasha : period.antarDashas) {
                if (currentTime >= antardasha.startTime && currentTime < antardasha.endTime) {
                    return QString("%1-%2").arg(period.planet).arg(antardasha.planet);
                }
            }
            return period.planet;
        }
    }
    
    return "Unknown";
}

QVector<DashaPeriod> DashaCalculator::calculateAntarDasha(const DashaPeriod& mahadasha) {
    QVector<DashaPeriod> antarDashas;
    
    // Find starting planet index
    int startIndex = planetNames.indexOf(mahadasha.planet);
    QDateTime startTime = mahadasha.startTime;
    
    // Calculate total period in milliseconds
    qint64 totalPeriodMs = mahadasha.startTime.msecsTo(mahadasha.endTime);
    
    for (int i = 0; i < 9; ++i) {
        int planetIndex = (startIndex + i) % 9;
        
        DashaPeriod antardasha;
        antardasha.planet = planetNames[planetIndex];
        antardasha.startTime = startTime;
        
        // Calculate proportion based on planet's years
        double proportion = dashaPeriods[planetIndex] / 120.0; // 120 is total of all dasha years
        qint64 periodMs = totalPeriodMs * proportion;
        
        antardasha.endTime = antardasha.startTime.addMSecs(periodMs);
        antarDashas.append(antardasha);
        
        startTime = antardasha.endTime;
    }
    
    return antarDashas;
}

int DashaCalculator::findStartingPlanet(double moonLongitude) {
    // Each nakshatra is 13°20' (13.3333... degrees)
    double nakshatraLength = 13.3333333333;
    
    // Calculate nakshatra number (0-26)
    int nakshatra = static_cast<int>(moonLongitude / nakshatraLength);
    
    // Calculate the pada (quarter) within the nakshatra
    double remainingDegrees = moonLongitude - (nakshatra * nakshatraLength);
    int pada = static_cast<int>(remainingDegrees / (nakshatraLength / 4));
    
    // Calculate total quarters passed
    int totalQuarters = (nakshatra * 4) + pada;
    
    // Map to Vimshottari Dasha order (Ketu, Venus, Sun, Moon, Mars, Rahu, Jupiter, Saturn, Mercury)
    return totalQuarters % 9;
}

QDateTime DashaCalculator::calculateDashaStartTime(const QDateTime& birthTime, double moonLongitude) {
    // Each nakshatra is 13°20' (13.3333... degrees)
    double nakshatraLength = 13.3333333333;
    
    // Calculate position within nakshatra
    double nakshatraPosition = fmod(moonLongitude, nakshatraLength);
    
    // Calculate proportion of nakshatra traversed
    double proportion = nakshatraPosition / nakshatraLength;
    
    // Calculate total period of current nakshatra lord in milliseconds
    int startingPlanet = findStartingPlanet(moonLongitude);
    qint64 totalPeriodMs = dashaPeriods[startingPlanet] * 365.25 * 24 * 60 * 60 * 1000;
    
    // Calculate time elapsed in current dasha
    qint64 elapsedMs = totalPeriodMs * proportion;
    
    // Subtract elapsed time from birth time to get dasha start time
    return birthTime.addMSecs(-elapsedMs);
}
