#include "swephexp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* Global variables */
static char ephepath[256] = SE_EPHE_PATH;
static int ephe_path_is_set = 0;
static double ayanamsa = 0;
static int sid_mode = SE_SIDM_FAGAN_BRADLEY;

/* Implementation of the exported functions */

int swe_calc(double tjd, int ipl, int iflag, double *xx, char *serr) {
    /* Basic position calculation */
    switch (ipl) {
        case SE_SUN:
            /* Simplified solar position calculation */
            xx[0] = fmod(tjd * 0.985647 + 279.403303, 360.0); // Longitude
            xx[1] = 0.0; // Latitude
            xx[2] = 1.0; // Distance in AU
            return 0;
            
        case SE_MOON:
            /* Simplified lunar position calculation */
            double D = tjd - 2451545.0; // Days since J2000
            double L = fmod(218.316 + 13.176396 * D, 360.0); // Mean longitude
            double M = fmod(134.963 + 13.064993 * D, 360.0); // Mean anomaly
            
            xx[0] = L + 6.289 * sin(M * M_PI / 180.0); // Longitude
            xx[1] = 5.128 * sin(M * M_PI / 180.0); // Latitude
            xx[2] = 60.27; // Distance in Earth radii
            return 0;
            
        default:
            if (serr) {
                sprintf(serr, "Planet number %d not implemented", ipl);
            }
            return -1;
    }
}

int swe_calc_ut(double tjd_ut, int ipl, int iflag, double *xx, char *serr) {
    /* Convert UT to ET and calculate */
    double tjd_et = tjd_ut + 0.0;  // Simplified time conversion
    return swe_calc(tjd_et, ipl, iflag, xx, serr);
}

int swe_fixstar(char *star, double tjd, int iflag, double *xx, char *serr) {
    /* Basic fixed star position calculation */
    if (serr) {
        sprintf(serr, "Fixed star calculations not implemented");
    }
    return -1;
}

int swe_fixstar_ut(char *star, double tjd_ut, int iflag, double *xx, char *serr) {
    double tjd_et = tjd_ut + 0.0;  // Simplified time conversion
    return swe_fixstar(star, tjd_et, iflag, xx, serr);
}

double swe_sidtime0(double tjd_ut, double ecl, double nut) {
    /* Basic sidereal time calculation */
    double T = (tjd_ut - 2451545.0) / 36525.0;
    return fmod(100.46061837 + 36000.770053608 * T + 0.000387933 * T * T, 360.0) / 15.0;
}

double swe_sidtime(double tjd_ut) {
    return swe_sidtime0(tjd_ut, 0.0, 0.0);
}

void swe_set_ephe_path(char *path) {
    if (path == NULL || *path == '\0') {
        strcpy(ephepath, SE_EPHE_PATH);
    } else {
        strncpy(ephepath, path, sizeof(ephepath)-1);
        ephepath[sizeof(ephepath)-1] = '\0';
    }
    ephe_path_is_set = 1;
}

void swe_close(void) {
    /* Cleanup function */
}

void swe_set_sid_mode(int sid_mode_arg, double t0, double ayan_t0) {
    sid_mode = sid_mode_arg;
    /* Store ayanamsa parameters */
    if (t0 != 0) {
        ayanamsa = ayan_t0;
    }
}

int swe_get_ayanamsa_ex(double tjd_et, int32 iflag, double *daya, char *serr) {
    /* Calculate ayanamsa based on selected mode */
    switch (sid_mode) {
        case SE_SIDM_FAGAN_BRADLEY:
            *daya = (tjd_et - 2433282.5) * 0.0001396209 + 24.816666667;
            break;
        case SE_SIDM_LAHIRI:
            *daya = (tjd_et - 2435553.5) * 0.0001396209 + 23.85;
            break;
        default:
            if (serr) {
                sprintf(serr, "Ayanamsa mode %d not implemented", sid_mode);
            }
            return -1;
    }
    return 0;
}

double swe_get_ayanamsa(double tjd_et) {
    double daya;
    char serr[256];
    swe_get_ayanamsa_ex(tjd_et, 0, &daya, serr);
    return daya;
}

double swe_get_ayanamsa_ut(double tjd_ut) {
    return swe_get_ayanamsa(tjd_ut + 0.0); // Simplified time conversion
}

char *swe_version(char *s) {
    strcpy(s, "2.10.03");
    return s;
}
