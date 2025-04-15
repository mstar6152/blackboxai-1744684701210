# AstroProQt

A professional astrology software built with Qt and Swiss Ephemeris, featuring both North and South Indian chart styles.

## Features

- Birth chart calculation and visualization
  - North Indian chart style
  - South Indian chart style
  - Interactive zoom and pan
  - Aspect lines display
  - Planet symbols with traditional colors

- Dasha (Planetary Period) Analysis
  - Vimshottari Dasha calculation
  - Detailed Antardasha (sub-periods)
  - Current running period indication
  - Period strength assessment

- Planetary Strength Calculations
  - Shadbala (six-fold strength)
  - Directional strength (Dig Bala)
  - Temporal strength (Kala Bala)
  - Motional strength (Cheshta Bala)
  - Natural strength (Naisargika Bala)
  - Aspect-based strength (Drishti Bala)

- Yoga (Planetary Combinations) Detection
  - Raja Yoga identification
  - Dhana Yoga analysis
  - Mahapurusha Yoga detection
  - Strength assessment of combinations

- Modern User Interface
  - Dark theme with modern aesthetics
  - Responsive layout
  - Location search with geocoding
  - Chart export functionality
  - Interactive tables and filters

## Requirements

- Qt 5.x
- CMake 3.1 or higher
- C++17 compliant compiler
- Swiss Ephemeris data files

## Building from Source

1. Install dependencies:
```bash
sudo apt-get update
sudo apt-get install -y qt5-default qtbase5-dev qttools5-dev cmake
```

2. Clone the repository:
```bash
git clone https://github.com/yourusername/AstroProQt.git
cd AstroProQt
```

3. Create build directory and compile:
```bash
mkdir build && cd build
cmake ..
make
```

4. Download ephemeris files:
```bash
cd ephe
wget https://www.astro.com/ftp/swisseph/ephe/sepl_18.se1
wget https://www.astro.com/ftp/swisseph/ephe/semo_18.se1
```

5. Run the application:
```bash
./AstroProQt
```

## Usage

1. Enter birth details:
   - Name
   - Date and time
   - Place (with automatic geocoding)

2. Select chart preferences:
   - Chart style (North/South Indian)
   - Aspect display
   - Zoom/Pan enable

3. Generate chart:
   - View birth chart
   - Check Dasha periods
   - Analyze planetary strengths
   - Review active Yogas

4. Export or save:
   - Export chart as image
   - Save calculations
   - Print reports

## Directory Structure

```
AstroProQt/
├── Calculators/           # Astrological calculation modules
├── Forms/                 # UI form files
├── swiss/                 # Swiss Ephemeris integration
├── icons/                 # Application icons
├── styles/               # QSS style sheets
├── ephe/                 # Ephemeris data files
└── docs/                 # Documentation
```

## Contributing

1. Fork the repository
2. Create a feature branch
3. Commit your changes
4. Push to the branch
5. Create a Pull Request

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Acknowledgments

- Swiss Ephemeris for astronomical calculations
- Qt framework for the GUI
- OpenCage for geocoding services

## Support

For support, please open an issue in the GitHub repository or contact the maintainers directly.
