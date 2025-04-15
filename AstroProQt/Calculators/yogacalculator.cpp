#include "yogacalculator.h"
#include <cmath>
#include <QDebug>

YogaCalculator::YogaCalculator() {}

// Strength calculation methods
double YogaCalculator::calculateRajaYogaStrength(
    const QMap<QString, double>& positions,
    const QMap<QString, double>& strengths) {
    
    double baseStrength = 75.0; // Base strength for Raja Yoga
    double totalStrength = 0.0;
    int count = 0;
    
    // Consider strengths of involved planets
    for (auto it = positions.begin(); it != positions.end(); ++it) {
        if (strengths.contains(it.key())) {
            totalStrength += strengths[it.key()];
            count++;
        }
    }
    
    if (count > 0) {
        return baseStrength * (1.0 + (totalStrength / (count * 100.0)));
    }
    return baseStrength;
}

double YogaCalculator::calculateDhanaYogaStrength(
    const QMap<QString, double>& positions,
    const QMap<QString, double>& strengths) {
    
    double baseStrength = 65.0; // Base strength for Dhana Yoga
    double totalStrength = 0.0;
    int count = 0;
    
    // Consider strengths of involved planets
    for (auto it = positions.begin(); it != positions.end(); ++it) {
        if (strengths.contains(it.key())) {
            totalStrength += strengths[it.key()];
            count++;
        }
    }
    
    if (count > 0) {
        return baseStrength * (1.0 + (totalStrength / (count * 100.0)));
    }
    return baseStrength;
}

double YogaCalculator::calculateGajaKesariStrength(
    const QMap<QString, double>& positions,
    const QMap<QString, double>& strengths) {
    
    double baseStrength = 70.0; // Base strength for Gaja Kesari Yoga
    
    if (strengths.contains("Jupiter") && strengths.contains("Moon")) {
        double jupiterStrength = strengths["Jupiter"];
        double moonStrength = strengths["Moon"];
        return baseStrength * (1.0 + ((jupiterStrength + moonStrength) / 200.0));
    }
    return baseStrength;
}

double YogaCalculator::calculateBudhAdityaStrength(
    const QMap<QString, double>& positions,
    const QMap<QString, double>& strengths) {
    
    double baseStrength = 70.0; // Base strength for Budh-Aditya Yoga
    
    if (strengths.contains("Mercury") && strengths.contains("Sun")) {
        double mercuryStrength = strengths["Mercury"];
        double sunStrength = strengths["Sun"];
        return baseStrength * (1.0 + ((mercuryStrength + sunStrength) / 200.0));
    }
    return baseStrength;
}

QVector<YogaCalculator::Yoga> YogaCalculator::detectActiveYogas(
    const QMap<QString, double>& planetPositions,
    const QVector<double>& housePositions,
    const QMap<QString, double>& planetaryStrengths) {
    
    QVector<Yoga> activeYogas;
    
    try {
        // Check each predefined yoga
        for (Yoga yoga : yogaDefinitions) {
            bool isActive = false;
            double strength = 0.0;
            
            if (yoga.name == "Raja Yoga") {
                isActive = checkRajaYoga(planetPositions, housePositions);
                if (isActive) {
                    strength = calculateRajaYogaStrength(planetPositions, planetaryStrengths);
                }
            }
            else if (yoga.name == "Dhana Yoga") {
                isActive = checkDhanaYoga(planetPositions, housePositions);
                if (isActive) {
                    strength = calculateDhanaYogaStrength(planetPositions, planetaryStrengths);
                }
            }
            else if (yoga.name == "Gaja Kesari") {
                isActive = checkGajaKesariYoga(planetPositions);
                if (isActive) {
                    strength = calculateGajaKesariStrength(planetPositions, planetaryStrengths);
                }
            }
            else if (yoga.name == "Budh-Aditya") {
                isActive = checkBudhAditya(planetPositions);
                if (isActive) {
                    strength = calculateBudhAdityaStrength(planetPositions, planetaryStrengths);
                }
            }
            else if (yoga.name == "Pancha Mahapurusha") {
                isActive = checkPanchaMahapurusha(planetPositions, housePositions);
                if (isActive) {
                    strength = 75.0; // Base strength for Mahapurusha Yoga
                }
            }
            else if (yoga.name == "Viparita Raja") {
                isActive = checkViparitaRaja(planetPositions, housePositions);
                if (isActive) {
                    strength = 80.0; // Strong yoga that turns negative into positive
                }
            }
            else if (yoga.name == "Hamsa") {
                isActive = checkHamsaYoga(planetPositions, housePositions);
                if (isActive) {
                    strength = 70.0;
                }
            }
            else if (yoga.name == "Malavya") {
                isActive = checkMalavyaYoga(planetPositions, housePositions);
                if (isActive) {
                    strength = 70.0;
                }
            }
            else if (yoga.name == "Shasha") {
                isActive = checkShashaYoga(planetPositions, housePositions);
                if (isActive) {
                    strength = 70.0;
                }
            }
            else if (yoga.name == "Ruchaka") {
                isActive = checkRuchakaYoga(planetPositions, housePositions);
                if (isActive) {
                    strength = 70.0;
                }
            }
            else if (yoga.name == "Bhadra") {
                isActive = checkBhadraYoga(planetPositions, housePositions);
                if (isActive) {
                    strength = 70.0;
                }
            }
            
            if (isActive) {
                yoga.isActive = true;
                yoga.strength = strength;
                activeYogas.append(yoga);
            }
        }
    } catch (const std::exception& e) {
        qDebug() << "Error detecting yogas:" << e.what();
    }
    
    return activeYogas;
}

// Utility Methods
bool YogaCalculator::arePlanetsConjunct(double pos1, double pos2, double orb) {
    double diff = std::abs(pos1 - pos2);
    if (diff > 180) diff = 360 - diff;
    return diff <= orb;
}

bool YogaCalculator::isPlanetInHouse(double planetPos, double houseStart, double houseEnd) {
    // Normalize positions to 0-360 range
    planetPos = std::fmod(planetPos + 360.0, 360.0);
    houseStart = std::fmod(houseStart + 360.0, 360.0);
    houseEnd = std::fmod(houseEnd + 360.0, 360.0);
    
    if (houseStart < houseEnd) {
        return planetPos >= houseStart && planetPos < houseEnd;
    } else {
        // House spans 0° Aries
        return planetPos >= houseStart || planetPos < houseEnd;
    }
}

int YogaCalculator::getHousePlacement(double longitude, const QVector<double>& houses) {
    for (int i = 0; i < 12; i++) {
        double houseStart = houses[i];
        double houseEnd = houses[(i + 1) % 12];
        
        if (isPlanetInHouse(longitude, houseStart, houseEnd)) {
            return i + 1; // Houses are 1-based
        }
    }
    return 1; // Default to 1st house if not found (shouldn't happen)
}

bool YogaCalculator::areInAspect(double pos1, double pos2, int aspect) {
    double diff = std::abs(pos1 - pos2);
    if (diff > 180) diff = 360 - diff;
    
    double orb = 10.0; // Default orb of 10 degrees
    return std::abs(diff - aspect) <= orb;
}

bool YogaCalculator::isPlanetInOwnSign(const QString& planet, double longitude) {
    int sign = static_cast<int>(longitude / 30);
    if (!planetRulerships.contains(planet)) return false;
    
    QVector<int> ruledSigns;
    for (int house : planetRulerships[planet]) {
        ruledSigns.append((house - 1) % 12); // Convert house number to sign index
    }
    
    return ruledSigns.contains(sign);
}

bool YogaCalculator::isPlanetExalted(const QString& planet, double longitude) {
    if (!exaltDebil.contains(planet)) return false;
    
    double exaltDegree = exaltDebil[planet].first;
    double orb = 10.0; // Consider within 10 degrees of exact exaltation
    
    double diff = std::abs(longitude - exaltDegree);
    if (diff > 180) diff = 360 - diff;
    
    return diff <= orb;
}

bool YogaCalculator::isPlanetInAngle(double longitude, const QVector<double>& houses) {
    int house = getHousePlacement(longitude, houses);
    return (house == 1 || house == 4 || house == 7 || house == 10);
}

bool YogaCalculator::isPlanetInKendra(double pos1, double pos2) {
    int houseDiff = std::abs(static_cast<int>(pos1 / 30) - 
                            static_cast<int>(pos2 / 30));
    return (houseDiff == 0 || houseDiff == 3 || houseDiff == 6 || houseDiff == 9);
}

// Basic Yoga Detection Methods
bool YogaCalculator::checkRajaYoga(
    const QMap<QString, double>& positions, 
    const QVector<double>& houses) {
    
    // Raja Yoga occurs when lords of trine houses (1,5,9) combine with
    // lords of quadrant houses (1,4,7,10)
    QVector<QString> trineLords, quadrantLords;
    
    // Get lords of trine and quadrant houses
    for (const QString& planet : planetRulerships.keys()) {
        for (int house : planetRulerships[planet]) {
            if (house == 1 || house == 5 || house == 9) {
                trineLords.append(planet);
            }
            if (house == 1 || house == 4 || house == 7 || house == 10) {
                quadrantLords.append(planet);
            }
        }
    }
    
    // Check if any trine lord is conjunct with any quadrant lord
    for (const QString& trineLord : trineLords) {
        if (!positions.contains(trineLord)) continue;
        
        for (const QString& quadrantLord : quadrantLords) {
            if (!positions.contains(quadrantLord)) continue;
            if (trineLord == quadrantLord) continue; // Same planet
            
            if (arePlanetsConjunct(positions[trineLord], positions[quadrantLord])) {
                return true;
            }
        }
    }
    
    return false;
}

bool YogaCalculator::checkDhanaYoga(
    const QMap<QString, double>& positions, 
    const QVector<double>& houses) {
    
    // Dhana Yoga occurs when benefics occupy 2nd, 5th, 9th or 11th houses
    QVector<QString> benefics = {"Jupiter", "Venus", "Mercury", "Moon"};
    QVector<int> dhanaHouses = {2, 5, 9, 11};
    
    for (const QString& benefic : benefics) {
        if (!positions.contains(benefic)) continue;
        
        int house = getHousePlacement(positions[benefic], houses);
        if (dhanaHouses.contains(house)) {
            return true;
        }
    }
    
    return false;
}

bool YogaCalculator::checkGajaKesariYoga(const QMap<QString, double>& positions) {
    if (!positions.contains("Jupiter") || !positions.contains("Moon")) {
        return false;
    }
    
    return isPlanetInKendra(positions["Jupiter"], positions["Moon"]);
}

bool YogaCalculator::checkBudhAditya(const QMap<QString, double>& positions) {
    if (!positions.contains("Mercury") || !positions.contains("Sun")) {
        return false;
    }
    
    return arePlanetsConjunct(positions["Mercury"], positions["Sun"]);
}

bool YogaCalculator::checkPanchaMahapurusha(
    const QMap<QString, double>& positions,
    const QVector<double>& houses) {
    
    QVector<QString> planets = {"Mars", "Mercury", "Jupiter", "Venus", "Saturn"};
    
    for (const QString& planet : planets) {
        if (!positions.contains(planet)) continue;
        
        double pos = positions[planet];
        if ((isPlanetInOwnSign(planet, pos) || isPlanetExalted(planet, pos)) &&
            isPlanetInAngle(pos, houses)) {
            return true;
        }
    }
    return false;
}

bool YogaCalculator::checkViparitaRaja(
    const QMap<QString, double>& positions,
    const QVector<double>& houses) {
    
    // Get lords of 6th, 8th, and 12th houses
    QString lord6th, lord8th, lord12th;
    
    for (const QString& planet : planetRulerships.keys()) {
        QVector<int> houses = planetRulerships[planet];
        if (houses.contains(6)) lord6th = planet;
        if (houses.contains(8)) lord8th = planet;
        if (houses.contains(12)) lord12th = planet;
    }
    
    // Check if these lords are in mutual kendras (angles)
    if (!positions.contains(lord6th) || !positions.contains(lord8th) || 
        !positions.contains(lord12th)) return false;
    
    return isPlanetInKendra(positions[lord6th], positions[lord8th]) &&
           isPlanetInKendra(positions[lord8th], positions[lord12th]) &&
           isPlanetInKendra(positions[lord6th], positions[lord12th]);
}

bool YogaCalculator::checkHamsaYoga(
    const QMap<QString, double>& positions,
    const QVector<double>& houses) {
    
    if (!positions.contains("Jupiter") || !positions.contains("Moon")) 
        return false;
    
    double jupiterPos = positions["Jupiter"];
    double moonPos = positions["Moon"];
    
    return (isPlanetInOwnSign("Jupiter", jupiterPos) || 
            isPlanetExalted("Jupiter", jupiterPos)) &&
           isPlanetInKendra(jupiterPos, moonPos);
}

bool YogaCalculator::checkMalavyaYoga(
    const QMap<QString, double>& positions,
    const QVector<double>& houses) {
    
    if (!positions.contains("Venus")) return false;
    
    double venusPos = positions["Venus"];
    return (isPlanetInOwnSign("Venus", venusPos) || 
            isPlanetExalted("Venus", venusPos)) &&
           isPlanetInAngle(venusPos, houses);
}

bool YogaCalculator::checkShashaYoga(
    const QMap<QString, double>& positions,
    const QVector<double>& houses) {
    
    if (!positions.contains("Saturn")) return false;
    
    double saturnPos = positions["Saturn"];
    return (isPlanetInOwnSign("Saturn", saturnPos) || 
            isPlanetExalted("Saturn", saturnPos)) &&
           isPlanetInAngle(saturnPos, houses);
}

bool YogaCalculator::checkRuchakaYoga(
    const QMap<QString, double>& positions,
    const QVector<double>& houses) {
    
    if (!positions.contains("Mars")) return false;
    
    double marsPos = positions["Mars"];
    return (isPlanetInOwnSign("Mars", marsPos) || 
            isPlanetExalted("Mars", marsPos)) &&
           isPlanetInAngle(marsPos, houses);
}

bool YogaCalculator::checkBhadraYoga(
    const QMap<QString, double>& positions,
    const QVector<double>& houses) {
    
    if (!positions.contains("Mercury")) return false;
    
    double mercuryPos = positions["Mercury"];
    return (isPlanetInOwnSign("Mercury", mercuryPos) || 
            isPlanetExalted("Mercury", mercuryPos)) &&
           isPlanetInAngle(mercuryPos, houses);
}
