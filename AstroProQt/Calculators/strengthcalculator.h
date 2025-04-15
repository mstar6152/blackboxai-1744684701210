#ifndef STRENGTHCALCULATOR_H
#define STRENGTHCALCULATOR_H

#include <QString>
#include <QMap>
#include <QVector>

class StrengthCalculator {
public:
    // Structure to hold various strength parameters
    struct PlanetaryStrength {
        double shadbala;      // Total Shadbala strength
        double sthanaBala;    // Positional strength
        double digBala;       // Directional strength
        double drishtisBala;  // Aspectual strength
        double kalaBala;      // Temporal strength
        double naisargikaBala; // Natural strength
        
        // Initialize with default values
        PlanetaryStrength() 
            : shadbala(0), sthanaBala(0), digBala(0),
              drishtisBala(0), kalaBala(0), naisargikaBala(0) {}
    };

    StrengthCalculator();

    // Calculate complete strength for all planets
    QMap<QString, PlanetaryStrength> calculateAllStrengths(
        const QMap<QString, double>& planetaryPositions,
        const QVector<double>& housePositions,
        const QDateTime& birthTime
    );

    // Individual strength calculations
    double calculateShadbala(const QString& planet, 
                           double longitude, 
                           const QVector<double>& housePositions,
                           const QDateTime& birthTime);
    
    double calculateDigBala(const QString& planet, double longitude);
    double calculateDrishtiBala(const QString& planet, 
                               const QMap<QString, double>& planetaryPositions);

private:
    // Constants for calculations
    const QVector<QString> planets = {
        "Sun", "Moon", "Mars", "Mercury", 
        "Jupiter", "Venus", "Saturn", "Rahu", "Ketu"
    };

    // Exaltation degrees for planets
    const QMap<QString, double> exaltationDegrees = {
        {"Sun", 10},     // Aries 10°
        {"Moon", 33},    // Taurus 3°
        {"Mars", 298},   // Capricorn 28°
        {"Mercury", 165},// Virgo 15°
        {"Jupiter", 95}, // Cancer 5°
        {"Venus", 357},  // Pisces 27°
        {"Saturn", 200}  // Libra 20°
    };

    // Helper methods
    double calculateSthanaBala(const QString& planet, double longitude);
    double calculateKalaBala(const QString& planet, 
                           double longitude, 
                           const QDateTime& birthTime);
    double calculateNaisargikaBala(const QString& planet);
    
    // Utility functions
    double getExaltationStrength(const QString& planet, double longitude);
    double getMoulaTrikonaStrength(const QString& planet, double longitude);
    bool isInOwnSign(const QString& planet, double longitude);
};

#endif // STRENGTHCALCULATOR_H
