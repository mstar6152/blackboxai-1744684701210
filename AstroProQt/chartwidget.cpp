#include "chartwidget.h"
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QDebug>
#include <cmath>

ChartWidget::ChartWidget(QWidget *parent)
    : QWidget(parent)
    , m_style(NorthIndian)
    , m_showAspects(true)
    , m_enableZoomPan(true)
    , m_zoom(1.0)
    , m_pan(0, 0)
{
    setMinimumSize(400, 400);
    initializePlanetSymbols();
    initializePlanetColors();
    setMouseTracking(true);
}

void ChartWidget::initializePlanetSymbols() {
    m_planetSymbols = {
        {"Sun", "☉"},
        {"Moon", "☽"},
        {"Mars", "♂"},
        {"Mercury", "☿"},
        {"Jupiter", "♃"},
        {"Venus", "♀"},
        {"Saturn", "♄"},
        {"Rahu", "☊"},
        {"Ketu", "☋"}
    };
}

void ChartWidget::initializePlanetColors() {
    m_planetColors = {
        {"Sun", QColor(255, 128, 0)},    // Orange
        {"Moon", QColor(192, 192, 192)},  // Silver
        {"Mars", QColor(255, 0, 0)},      // Red
        {"Mercury", QColor(0, 255, 0)},   // Green
        {"Jupiter", QColor(255, 255, 0)}, // Yellow
        {"Venus", QColor(128, 0, 128)},   // Purple
        {"Saturn", QColor(0, 0, 128)},    // Dark Blue
        {"Rahu", QColor(128, 128, 128)},  // Gray
        {"Ketu", QColor(64, 64, 64)}      // Dark Gray
    };
}

void ChartWidget::setChartStyle(ChartStyle style) {
    m_style = style;
    update();
}

void ChartWidget::setShowAspects(bool show) {
    m_showAspects = show;
    update();
}

void ChartWidget::enableZoomAndPan(bool enable) {
    m_enableZoomPan = enable;
}

void ChartWidget::setBirthData(const QDateTime& birthTime, const QString& place,
                              double lat, double lon) {
    m_chartData.birthTime = birthTime;
    m_chartData.birthPlace = place;
    m_chartData.latitude = lat;
    m_chartData.longitude = lon;
}

void ChartWidget::setPlanetPositions(const QMap<QString, double>& positions) {
    m_chartData.planetPositions = positions;
    calculateStrengths();
    calculateYogas();
    update();
}

void ChartWidget::setHousePositions(const QVector<double>& positions) {
    m_chartData.housePositions = positions;
    update();
}

void ChartWidget::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // Apply zoom and pan transformations
    painter.translate(width() / 2 + m_pan.x(), height() / 2 + m_pan.y());
    painter.scale(m_zoom, m_zoom);
    painter.translate(-width() / 2, -height() / 2);
    
    // Draw chart based on style
    if (m_style == NorthIndian) {
        drawNorthIndianChart(painter);
    } else {
        drawSouthIndianChart(painter);
    }
    
    // Draw aspects if enabled
    if (m_showAspects) {
        drawAspects(painter);
    }
    
    // Draw planets
    drawPlanets(painter);
}

void ChartWidget::drawNorthIndianChart(QPainter& painter) {
    QRectF chartRect = getChartRect();
    
    // Draw outer square
    painter.setPen(QPen(Qt::black, 2));
    painter.drawRect(chartRect);
    
    // Draw diagonal lines
    painter.drawLine(chartRect.topLeft(), chartRect.bottomRight());
    painter.drawLine(chartRect.topRight(), chartRect.bottomLeft());
    
    // Draw inner square
    double innerSize = chartRect.width() * 0.5;
    QRectF innerRect(
        chartRect.center().x() - innerSize/2,
        chartRect.center().y() - innerSize/2,
        innerSize, innerSize
    );
    painter.drawRect(innerRect);
    
    // Draw house numbers
    drawHouseNumbers(painter);
}

void ChartWidget::drawSouthIndianChart(QPainter& painter) {
    QRectF chartRect = getChartRect();
    
    // Draw main grid (3x3)
    double cellWidth = chartRect.width() / 3;
    double cellHeight = chartRect.height() / 3;
    
    painter.setPen(QPen(Qt::black, 2));
    
    // Draw horizontal lines
    for (int i = 0; i <= 3; ++i) {
        painter.drawLine(
            chartRect.left(), chartRect.top() + i * cellHeight,
            chartRect.right(), chartRect.top() + i * cellHeight
        );
    }
    
    // Draw vertical lines
    for (int i = 0; i <= 3; ++i) {
        painter.drawLine(
            chartRect.left() + i * cellWidth, chartRect.top(),
            chartRect.left() + i * cellWidth, chartRect.bottom()
        );
    }
    
    // Draw house numbers
    drawHouseNumbers(painter);
}

void ChartWidget::drawPlanets(QPainter& painter) {
    for (auto it = m_chartData.planetPositions.begin(); 
         it != m_chartData.planetPositions.end(); ++it) {
        
        const QString& planet = it.key();
        double longitude = it.value();
        
        QPointF pos = calculatePlanetPosition(longitude);
        
        // Draw planet symbol
        painter.setPen(m_planetColors[planet]);
        painter.setFont(QFont("Arial", PLANET_SYMBOL_SIZE));
        painter.drawText(
            QRectF(pos.x() - PLANET_SYMBOL_SIZE/2,
                   pos.y() - PLANET_SYMBOL_SIZE/2,
                   PLANET_SYMBOL_SIZE,
                   PLANET_SYMBOL_SIZE),
            Qt::AlignCenter,
            m_planetSymbols[planet]
        );
    }
}

void ChartWidget::drawAspects(QPainter& painter) {
    for (auto it1 = m_chartData.planetPositions.begin(); 
         it1 != m_chartData.planetPositions.end(); ++it1) {
        
        for (auto it2 = it1 + 1; 
             it2 != m_chartData.planetPositions.end(); ++it2) {
            
            double angle = std::abs(it1.value() - it2.value());
            if (angle > 180) angle = 360 - angle;
            
            // Check for major aspects
            if (std::abs(angle - 60) < 6 ||   // Sextile
                std::abs(angle - 90) < 6 ||   // Square
                std::abs(angle - 120) < 6 ||  // Trine
                std::abs(angle - 180) < 6) {  // Opposition
                
                QPointF pos1 = calculatePlanetPosition(it1.value());
                QPointF pos2 = calculatePlanetPosition(it2.value());
                
                // Draw aspect line
                QPen aspectPen(Qt::gray, 1, Qt::DashLine);
                painter.setPen(aspectPen);
                painter.drawLine(pos1, pos2);
            }
        }
    }
}

void ChartWidget::drawHouseNumbers(QPainter& painter) {
    if (m_chartData.housePositions.isEmpty()) return;
    
    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial", HOUSE_NUMBER_SIZE));
    
    for (int i = 0; i < 12; ++i) {
        double longitude = m_chartData.housePositions[i];
        QPointF pos = calculatePlanetPosition(longitude);
        
        QString number = QString::number(i + 1);
        painter.drawText(
            QRectF(pos.x() - HOUSE_NUMBER_SIZE/2,
                   pos.y() - HOUSE_NUMBER_SIZE/2,
                   HOUSE_NUMBER_SIZE,
                   HOUSE_NUMBER_SIZE),
            Qt::AlignCenter,
            number
        );
    }
}

QPointF ChartWidget::calculatePlanetPosition(double longitude) {
    QRectF chartRect = getChartRect();
    
    if (m_style == NorthIndian) {
        // Calculate position in North Indian style
        int house = static_cast<int>(longitude / 30) + 1;
        double radius = chartRect.width() * 0.25;
        
        // Adjust angle based on house position
        double angle = (house - 1) * 30.0;
        angle = qDegreesToRadians(angle);
        
        return QPointF(
            chartRect.center().x() + radius * std::cos(angle),
            chartRect.center().y() + radius * std::sin(angle)
        );
    } else {
        // Calculate position in South Indian style
        int house = static_cast<int>(longitude / 30) + 1;
        double cellWidth = chartRect.width() / 3;
        double cellHeight = chartRect.height() / 3;
        
        // Convert house number to grid position
        int row = (house - 1) / 3;
        int col = (house - 1) % 3;
        
        return QPointF(
            chartRect.left() + (col + 0.5) * cellWidth,
            chartRect.top() + (row + 0.5) * cellHeight
        );
    }
}

QRectF ChartWidget::getChartRect() const {
    int size = std::min(width(), height()) - 2 * CHART_PADDING;
    return QRectF(
        (width() - size) / 2,
        (height() - size) / 2,
        size, size
    );
}

void ChartWidget::mousePressEvent(QMouseEvent* event) {
    if (m_enableZoomPan) {
        m_lastMousePos = event->pos();
    }
}

void ChartWidget::mouseMoveEvent(QMouseEvent* event) {
    if (m_enableZoomPan && event->buttons() & Qt::LeftButton) {
        QPointF delta = event->pos() - m_lastMousePos;
        m_pan += delta;
        m_lastMousePos = event->pos();
        update();
    }
}

void ChartWidget::wheelEvent(QWheelEvent* event) {
    if (m_enableZoomPan) {
        double delta = event->angleDelta().y() / 120.0;
        double newZoom = m_zoom * (1.0 + delta * 0.1);
        
        // Limit zoom range
        newZoom = std::max(MIN_ZOOM, std::min(MAX_ZOOM, newZoom));
        
        if (newZoom != m_zoom) {
            // Adjust pan to zoom towards mouse position
            QPointF mousePos = event->pos();
            QPointF relativePos = mousePos - QPointF(width()/2, height()/2) - m_pan;
            QPointF newPan = m_pan + relativePos * (1 - newZoom/m_zoom);
            
            m_zoom = newZoom;
            m_pan = newPan;
            update();
        }
    }
}

void ChartWidget::calculateStrengths() {
    m_chartData.planetaryStrengths = 
        m_strengthCalculator.calculateAllStrengths(
            m_chartData.planetPositions,
            m_chartData.housePositions,
            m_chartData.birthTime
        );
}

void ChartWidget::calculateYogas() {
    m_chartData.activeYogas = 
        m_yogaCalculator.detectActiveYogas(
            m_chartData.planetPositions,
            m_chartData.housePositions,
            m_chartData.planetaryStrengths
        );
}

void ChartWidget::calculateDasha() {
    if (m_chartData.planetPositions.contains("Moon")) {
        m_chartData.dashaPeriods = 
            m_dashaCalculator.calculateVimshottariDasha(
                m_chartData.birthTime,
                m_chartData.planetPositions["Moon"]
            );
    }
}

void ChartWidget::generateChart() {
    try {
        calculateDasha();
        calculateStrengths();
        calculateYogas();
        update();
        emit chartGenerated();
    } catch (const std::exception& e) {
        emit errorOccurred(QString("Error generating chart: %1").arg(e.what()));
    }
}

void ChartWidget::exportChart(const QString& filePath) {
    QImage image(size(), QImage::Format_ARGB32);
    image.fill(Qt::white);
    
    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // Draw chart on the image
    if (m_style == NorthIndian) {
        drawNorthIndianChart(painter);
    } else {
        drawSouthIndianChart(painter);
    }
    
    if (m_showAspects) {
        drawAspects(painter);
    }
    drawPlanets(painter);
    
    // Save image
    image.save(filePath);
}

void ChartWidget::resetView() {
    m_zoom = 1.0;
    m_pan = QPointF(0, 0);
    update();
}

QVector<DashaPeriod> ChartWidget::getDashaPeriods() const {
    return m_chartData.dashaPeriods;
}

QMap<QString, StrengthCalculator::PlanetaryStrength> 
ChartWidget::getPlanetaryStrengths() const {
    return m_chartData.planetaryStrengths;
}

QVector<YogaCalculator::Yoga> ChartWidget::getActiveYogas() const {
    return m_chartData.activeYogas;
}
