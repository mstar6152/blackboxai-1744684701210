/* SWISSEPH astrological calculations */
#ifndef _SWEPHEXP_INCLUDED
#define _SWEPHEXP_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#define SE_AUNIT_TO_KM        (149597870.700)
#define SE_MOON_MEAN_DIST     (384400.0)
#define SE_MOON_MEAN_INCL     (5.1453964)
#define SE_MOON_MEAN_ECC      (0.054900489)

/* planet numbers for the ipl parameter in swe_calc() */
#define SE_ECL_NUT             -1      
#define SE_SUN                  0       
#define SE_MOON                 1       
#define SE_MERCURY             2       
#define SE_VENUS               3       
#define SE_MARS                4       
#define SE_JUPITER             5       
#define SE_SATURN              6       
#define SE_URANUS              7       
#define SE_NEPTUNE             8       
#define SE_PLUTO               9       
#define SE_MEAN_NODE           10      
#define SE_TRUE_NODE           11
#define SE_MEAN_APOG          12     
#define SE_OSCU_APOG          13     
#define SE_EARTH              14     
#define SE_CHIRON             15     
#define SE_PHOLUS             16     
#define SE_CERES              17      
#define SE_PALLAS             18      
#define SE_JUNO               19      
#define SE_VESTA              20      
#define SE_INTP_APOG         21     
#define SE_INTP_PERG         22    

#define SE_NPLANETS           23      

#define SE_AST_OFFSET         10000
#define SE_VARUNA             (SE_AST_OFFSET + 20000)

#define SE_FICT_OFFSET        40
#define SE_FICT_OFFSET_1     39
#define SE_FICT_MAX          999 
#define SE_NFICT_ELEM        15

#define SE_COMET_OFFSET      1000

#define SE_NALL_NAT_POINTS     (SE_NPLANETS + SE_NFICT_ELEM)

/* Ephemeris path */
#define SE_EPHE_PATH          "ephemeris"

/* Ayanamsa definitions */
#define SE_SIDM_FAGAN_BRADLEY    0
#define SE_SIDM_LAHIRI           1
#define SE_SIDM_DELUCE           2
#define SE_SIDM_RAMAN            3
#define SE_SIDM_KRISHNAMURTI     4
#define SE_SIDM_DJWHAL_KHUL      5
#define SE_SIDM_YUKTESHWAR       6
#define SE_SIDM_JN_BHASIN        7
#define SE_SIDM_BABYL_KUGLER1    8
#define SE_SIDM_BABYL_KUGLER2    9
#define SE_SIDM_BABYL_KUGLER3   10
#define SE_SIDM_BABYL_HUBER     11
#define SE_SIDM_BABYL_ETPSC     12
#define SE_SIDM_ALDEBARAN_15TAU 13
#define SE_SIDM_HIPPARCHOS      14
#define SE_SIDM_SASSANIAN       15
#define SE_SIDM_GALCENT_0SAG    16
#define SE_SIDM_J2000           17
#define SE_SIDM_J1900           18
#define SE_SIDM_B1950           19
#define SE_SIDM_USER            255

/* Calendar types */
#define SE_JUL_CAL            0
#define SE_GREG_CAL           1

/* Function declarations */
extern int swe_calc(
    double tjd, int ipl, int iflag,
    double *xx, char *serr);

extern int swe_calc_ut(
    double tjd_ut, int ipl, int iflag,
    double *xx, char *serr);

extern int swe_fixstar(
    char *star, double tjd, int iflag,
    double *xx, char *serr);

extern int swe_fixstar_ut(
    char *star, double tjd_ut, int iflag,
    double *xx, char *serr);

extern double swe_sidtime0(
    double tjd_ut, double ecl, double nut);

extern double swe_sidtime(double tjd_ut);

extern void swe_set_ephe_path(char *path);

extern void swe_close(void);

extern void swe_set_sid_mode(
    int sid_mode, double t0, double ayan_t0);

extern int swe_get_ayanamsa_ex(
    double tjd_et, int32 iflag,
    double *daya, char *serr);

extern double swe_get_ayanamsa(double tjd_et);

extern double swe_get_ayanamsa_ut(double tjd_ut);

extern char *swe_version(char *);

#ifdef __cplusplus
}
#endif

#endif /* _SWEPHEXP_INCLUDED */
