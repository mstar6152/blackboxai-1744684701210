#ifndef YOGACALCULATOR_H
#define YOGACALCULATOR_H

#include <QString>
#include <QVector>
#include <QMap>

class YogaCalculator {
public:
    struct Yoga {
        QString name;
        QString description;
        bool isActive;
        double strength;  // 0-100%
        
        Yoga() : isActive(false), strength(0.0) {}
        Yoga(const QString& n, const QString& d) 
            : name(n), description(d), isActive(false), strength(0.0) {}
    };

    YogaCalculator();

    // Main function to detect all active yogas
    QVector<Yoga> detectActiveYogas(
        const QMap<QString, double>& planetPositions,
        const QVector<double>& housePositions,
        const QMap<QString, double>& planetaryStrengths
    );

private:
    // Yoga detection helper functions
    bool checkRajaYoga(const QMap<QString, double>& positions, 
                      const QVector<double>& houses);
    bool checkDhanaYoga(const QMap<QString, double>& positions, 
                       const QVector<double>& houses);
    bool checkGajaKesariYoga(const QMap<QString, double>& positions);
    bool checkBudhAditya(const QMap<QString, double>& positions);

    // Yoga strength calculation methods
    double calculateRajaYogaStrength(const QMap<QString, double>& positions,
                                   const QMap<QString, double>& strengths);
    double calculateDhanaYogaStrength(const QMap<QString, double>& positions,
                                    const QMap<QString, double>& strengths);
    double calculateGajaKesariStrength(const QMap<QString, double>& positions,
                                     const QMap<QString, double>& strengths);
    double calculateBudhAdityaStrength(const QMap<QString, double>& positions,
                                     const QMap<QString, double>& strengths);
    bool checkPanchaMahapurusha(const QMap<QString, double>& positions,
                               const QVector<double>& houses);
    bool checkViparitaRaja(const QMap<QString, double>& positions,
                          const QVector<double>& houses);
    bool checkHamsaYoga(const QMap<QString, double>& positions,
                       const QVector<double>& houses);
    bool checkMalavyaYoga(const QMap<QString, double>& positions,
                         const QVector<double>& houses);
    bool checkShashaYoga(const QMap<QString, double>& positions,
                        const QVector<double>& houses);
    bool checkRuchakaYoga(const QMap<QString, double>& positions,
                         const QVector<double>& houses);
    bool checkBhadraYoga(const QMap<QString, double>& positions,
                        const QVector<double>& houses);
    
    // Utility functions
    bool arePlanetsConjunct(double pos1, double pos2, double orb = 10.0);
    bool isPlanetInHouse(double planetPos, double houseStart, double houseEnd);
    int getHousePlacement(double longitude, const QVector<double>& houses);
    bool areInAspect(double pos1, double pos2, int aspect);
    bool isPlanetInOwnSign(const QString& planet, double longitude);
    bool isPlanetExalted(const QString& planet, double longitude);
    bool isPlanetInAngle(double longitude, const QVector<double>& houses);
    bool isPlanetInKendra(double pos1, double pos2);
    
    // Predefined yoga definitions
    const QVector<Yoga> yogaDefinitions = {
        Yoga("Raja Yoga", "Combination of lords of trine and quadrant houses"),
        Yoga("Dhana Yoga", "Combination indicating wealth and prosperity"),
        Yoga("Gaja Kesari", "Jupiter and Moon combination in quadrant houses"),
        Yoga("Budh-Aditya", "Mercury and Sun combination indicating intelligence"),
        Yoga("Chandra-Mangal", "Moon and Mars combination indicating courage"),
        Yoga("Neecha Bhanga", "Cancellation of debilitation"),
        Yoga("Pancha Mahapurusha", "Planets in own/exaltation sign in angles"),
        Yoga("Viparita Raja", "Lords of 6th, 8th, 12th in mutual angles"),
        Yoga("Hamsa", "Jupiter in own/exaltation sign in angle from Moon"),
        Yoga("Malavya", "Venus in own/exaltation sign in angle from Ascendant"),
        Yoga("Shasha", "Saturn in own/exaltation sign in angle from Ascendant"),
        Yoga("Ruchaka", "Mars in own/exaltation sign in angle from Ascendant"),
        Yoga("Bhadra", "Mercury in own/exaltation sign in angle from Ascendant")
    };

    // House and sign lordships
    const QMap<QString, QVector<int>> planetRulerships = {
        {"Sun", {5}},           // Leo
        {"Moon", {4}},          // Cancer
        {"Mars", {1, 8}},       // Aries, Scorpio
        {"Mercury", {3, 6}},    // Gemini, Virgo
        {"Jupiter", {9, 12}},   // Sagittarius, Pisces
        {"Venus", {2, 7}},      // Taurus, Libra
        {"Saturn", {10, 11}}    // Capricorn, Aquarius
    };

    // Exaltation and debilitation degrees
    const QMap<QString, QPair<double, double>> exaltDebil = {
        {"Sun", {10.0, 190.0}},      // Exalted in Aries, Debilitated in Libra
        {"Moon", {33.0, 213.0}},     // Exalted in Taurus, Debilitated in Scorpio
        {"Mars", {298.0, 118.0}},    // Exalted in Capricorn, Debilitated in Cancer
        {"Mercury", {165.0, 345.0}}, // Exalted in Virgo, Debilitated in Pisces
        {"Jupiter", {95.0, 275.0}},  // Exalted in Cancer, Debilitated in Capricorn
        {"Venus", {357.0, 177.0}},   // Exalted in Pisces, Debilitated in Virgo
        {"Saturn", {200.0, 20.0}}    // Exalted in Libra, Debilitated in Aries
    };
};

#endif // YOGACALCULATOR_H
