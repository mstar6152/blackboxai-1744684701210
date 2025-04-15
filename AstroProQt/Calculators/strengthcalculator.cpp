#include "strengthcalculator.h"
#include <QtMath>

StrengthCalculator::StrengthCalculator() {
    initializeRulershipTable();
    initializeExaltationTable();
    initializeAspectTable();
}

void StrengthCalculator::initializeRulershipTable() {
    // Planet rulership in signs (0 = Aries, 11 = Pisces)
    rulershipTable = {
        {"Sun", {4}},          // Leo
        {"Moon", {3}},         // Cancer
        {"Mars", {0, 7}},      // Aries, Scorpio
        {"Mercury", {2, 5}},   // Gemini, Virgo
        {"Jupiter", {8, 11}},  // Sagittarius, Pisces
        {"Venus", {1, 6}},     // Taurus, Libra
        {"Saturn", {9, 10}},   // Capricorn, Aquarius
        {"Rahu", {}},          // No rulership
        {"Ketu", {}}           // No rulership
    };
}

void StrengthCalculator::initializeExaltationTable() {
    // Exaltation points for planets (degree, sign)
    exaltationTable = {
        {"Sun", {10, 0}},      // 10° Aries
        {"Moon", {3, 1}},      // 3° Taurus
        {"Mars", {28, 9}},     // 28° Capricorn
        {"Mercury", {15, 5}},  // 15° Virgo
        {"Jupiter", {5, 3}},   // 5° Cancer
        {"Venus", {27, 11}},   // 27° Pisces
        {"Saturn", {20, 6}},   // 20° Libra
        {"Rahu", {20, 2}},     // 20° Gemini
        {"Ketu", {20, 8}}      // 20° Sagittarius
    };
}

void StrengthCalculator::initializeAspectTable() {
    // Special aspects for planets (degrees and strength)
    aspectTable = {
        {"Mars", {{90, 0.5}, {120, 0.75}, {180, 1.0}}},
        {"Jupiter", {{60, 0.5}, {120, 0.75}, {180, 1.0}}},
        {"Saturn", {{60, 0.5}, {90, 0.75}, {180, 1.0}}}
    };
}

QMap<QString, PlanetaryStrength> StrengthCalculator::calculateStrengths(
    const QMap<QString, double>& planetPositions,
    const QVector<double>& housePositions) {
    
    QMap<QString, PlanetaryStrength> strengths;
    
    // Calculate strength for each planet
    for (auto it = planetPositions.begin(); it != planetPositions.end(); ++it) {
        QString planet = it.key();
        double position = it.value();
        
        PlanetaryStrength strength;
        
        // Calculate Shadbala (six-fold strength)
        strength.positional = calculatePositionalStrength(planet, position, housePositions);
        strength.directional = calculateDirectionalStrength(planet, position);
        strength.temporal = calculateTemporalStrength(planet, position);
        strength.motional = calculateMotionalStrength(planet, position);
        strength.aspectual = calculateAspectualStrength(planet, position, planetPositions);
        strength.natural = calculateNaturalStrength(planet);
        
        // Calculate total strength
        strength.total = strength.positional + strength.directional + 
                        strength.temporal + strength.motional + 
                        strength.aspectual + strength.natural;
                        
        strengths[planet] = strength;
    }
    
    return strengths;
}

double StrengthCalculator::calculatePositionalStrength(
    const QString& planet, double position, const QVector<double>& houses) {
    
    double strength = 0.0;
    int sign = static_cast<int>(position / 30);
    
    // Check ownership
    if (rulershipTable[planet].contains(sign)) {
        strength += 5.0;
    }
    
    // Check exaltation
    if (exaltationTable.contains(planet)) {
        QPair<int, int> exaltPoint = exaltationTable[planet];
        double distance = qAbs(position - (exaltPoint.second * 30 + exaltPoint.first));
        if (distance < 30) {
            strength += (30 - distance) / 6.0; // Maximum 5 points for exact exaltation
        }
    }
    
    // Check house placement
    int house = 1;
    for (int i = 0; i < houses.size(); ++i) {
        if (position >= houses[i] && position < houses[(i + 1) % 12]) {
            house = i + 1;
            break;
        }
    }
    
    // Add house-based strength
    switch (house) {
        case 1: case 4: case 7: case 10: // Angular houses
            strength += 3.0;
            break;
        case 2: case 5: case 8: case 11: // Succedent houses
            strength += 2.0;
            break;
        default: // Cadent houses
            strength += 1.0;
            break;
    }
    
    return strength;
}

double StrengthCalculator::calculateDirectionalStrength(const QString& planet, double position) {
    // Calculate strength based on planetary dignities in different directions
    double strength = 0.0;
    int sign = static_cast<int>(position / 30);
    
    // Cardinal directions strength
    switch (sign) {
        case 0: case 1: case 2: // East (Aries to Gemini)
            if (planet == "Jupiter" || planet == "Mercury") strength += 2.0;
            break;
        case 3: case 4: case 5: // North (Cancer to Virgo)
            if (planet == "Moon" || planet == "Venus") strength += 2.0;
            break;
        case 6: case 7: case 8: // West (Libra to Sagittarius)
            if (planet == "Saturn" || planet == "Sun") strength += 2.0;
            break;
        case 9: case 10: case 11: // South (Capricorn to Pisces)
            if (planet == "Mars") strength += 2.0;
            break;
    }
    
    return strength;
}

double StrengthCalculator::calculateTemporalStrength(const QString& planet, double position) {
    // Calculate strength based on time of day/night
    double strength = 0.0;
    int sign = static_cast<int>(position / 30);
    bool isDiurnal = (sign >= 0 && sign <= 5); // Above horizon
    
    // Diurnal planets
    if (isDiurnal && (planet == "Sun" || planet == "Jupiter" || planet == "Saturn")) {
        strength += 2.0;
    }
    // Nocturnal planets
    else if (!isDiurnal && (planet == "Moon" || planet == "Venus" || planet == "Mars")) {
        strength += 2.0;
    }
    // Mercury is strong in both
    else if (planet == "Mercury") {
        strength += 1.0;
    }
    
    return strength;
}

double StrengthCalculator::calculateMotionalStrength(const QString& planet, double position) {
    // Base implementation - would need actual planetary motion data
    // Returns a default value for now
    return 1.0;
}

double StrengthCalculator::calculateAspectualStrength(
    const QString& planet, double position,
    const QMap<QString, double>& planetPositions) {
    
    double strength = 0.0;
    
    // Check aspects from other planets
    for (auto it = planetPositions.begin(); it != planetPositions.end(); ++it) {
        if (it.key() == planet) continue;
        
        double aspect = calculateAspect(position, it.value());
        
        // Check if this aspect is special for the aspecting planet
        if (aspectTable.contains(it.key())) {
            const QVector<QPair<double, double>>& specialAspects = aspectTable[it.key()];
            for (const auto& specialAspect : specialAspects) {
                if (qAbs(aspect - specialAspect.first) < 1.0) {
                    strength += specialAspect.second;
                    break;
                }
            }
        }
        
        // Standard aspects
        if (qAbs(aspect - 120) < 1.0) strength += 0.5; // Trine
        else if (qAbs(aspect - 60) < 1.0) strength += 0.3; // Sextile
        else if (qAbs(aspect - 0) < 1.0) strength += 0.2; // Conjunction
    }
    
    return strength;
}

double StrengthCalculator::calculateNaturalStrength(const QString& planet) {
    // Base natural strengths for planets
    static const QMap<QString, double> naturalStrengths = {
        {"Sun", 5.0},
        {"Moon", 4.0},
        {"Jupiter", 4.0},
        {"Venus", 3.0},
        {"Mercury", 3.0},
        {"Mars", 2.0},
        {"Saturn", 2.0},
        {"Rahu", 1.0},
        {"Ketu", 1.0}
    };
    
    return naturalStrengths.value(planet, 0.0);
}

double StrengthCalculator::calculateAspect(double pos1, double pos2) {
    double diff = qAbs(pos1 - pos2);
    if (diff > 180) diff = 360 - diff;
    return diff;
}
