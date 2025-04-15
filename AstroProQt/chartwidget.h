#ifndef CHARTWIDGET_H
#define CHARTWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QDateTime>
#include <QMap>
#include "Calculators/dashacalculator.h"
#include "Calculators/strengthcalculator.h"
#include "Calculators/yogacalculator.h"

class ChartWidget : public QWidget {
    Q_OBJECT

public:
    enum ChartStyle {
        NorthIndian,
        SouthIndian
    };

    struct ChartData {
        QDateTime birthTime;
        QString birthPlace;
        double latitude;
        double longitude;
        QMap<QString, double> planetPositions;
        QVector<double> housePositions;
        QMap<QString, double> planetaryStrengths;
        QVector<YogaCalculator::Yoga> activeYogas;
        QVector<DashaPeriod> dashaPeriods;
    };

    explicit ChartWidget(QWidget *parent = nullptr);

    // Chart configuration
    void setChartStyle(ChartStyle style);
    void setShowAspects(bool show);
    void enableZoomAndPan(bool enable);

    // Data setters
    void setBirthData(const QDateTime& birthTime, const QString& place,
                     double lat, double lon);
    void setPlanetPositions(const QMap<QString, double>& positions);
    void setHousePositions(const QVector<double>& positions);

    // Chart operations
    void generateChart();
    void exportChart(const QString& filePath);
    void resetView();

    // Getters for calculated data
    QVector<DashaPeriod> getDashaPeriods() const;
    QMap<QString, StrengthCalculator::PlanetaryStrength> getPlanetaryStrengths() const;
    QVector<YogaCalculator::Yoga> getActiveYogas() const;

signals:
    void chartGenerated();
    void errorOccurred(const QString& error);
    void calculationsUpdated();

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private:
    // Drawing functions
    void drawNorthIndianChart(QPainter& painter);
    void drawSouthIndianChart(QPainter& painter);
    void drawPlanets(QPainter& painter);
    void drawAspects(QPainter& painter);
    void drawHouseNumbers(QPainter& painter);
    void drawZodiacSymbols(QPainter& painter);
    
    // Calculation functions
    void calculateDasha();
    void calculateStrengths();
    void calculateYogas();
    
    // Utility functions
    QPointF calculatePlanetPosition(double longitude);
    int getHouseNumber(double longitude);
    QString getPlanetSymbol(const QString& planet);
    QColor getPlanetColor(const QString& planet);
    
    // Helper functions
    void initializePlanetSymbols();
    void initializePlanetColors();
    void updateTransformation();
    QRectF getChartRect() const;

    // Member variables
    ChartStyle m_style;
    bool m_showAspects;
    bool m_enableZoomPan;
    double m_zoom;
    QPointF m_pan;
    QPointF m_lastMousePos;
    
    // Chart data
    ChartData m_chartData;
    
    // Calculators
    DashaCalculator m_dashaCalculator;
    StrengthCalculator m_strengthCalculator;
    YogaCalculator m_yogaCalculator;
    
    // Visual properties
    QMap<QString, QString> m_planetSymbols;
    QMap<QString, QColor> m_planetColors;
    
    // Constants
    const int CHART_PADDING = 20;
    const int HOUSE_NUMBER_SIZE = 12;
    const int PLANET_SYMBOL_SIZE = 14;
    const double MIN_ZOOM = 0.5;
    const double MAX_ZOOM = 3.0;
};

#endif // CHARTWIDGET_H
