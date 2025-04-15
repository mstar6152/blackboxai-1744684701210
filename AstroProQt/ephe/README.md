# Swiss Ephemeris Data Files

This directory should contain the Swiss Ephemeris data files required for accurate astronomical calculations. Due to licensing restrictions, these files are not included in the repository.

## Required Files

The following files are needed for basic functionality:
- sepl_18.se1 (Main planetary positions)
- semo_18.se1 (Moon positions)
- sepl_24.se1 (Extended planetary positions)
- semo_24.se1 (Extended moon positions)
- seas_18.se1 (Asteroid positions)
- s136108s.se1 (Additional data)
- s136199s.se1 (Additional data)

## How to Obtain

1. Visit the Swiss Ephemeris website: https://www.astro.com/swisseph/
2. Download the ephemeris files package
3. Extract the .se1 files to this directory

## Alternative Method

You can also use the following command to download the files directly:
```bash
wget https://www.astro.com/ftp/swisseph/ephe/sepl_18.se1
wget https://www.astro.com/ftp/swisseph/ephe/semo_18.se1
wget https://www.astro.com/ftp/swisseph/ephe/sepl_24.se1
wget https://www.astro.com/ftp/swisseph/ephe/semo_24.se1
wget https://www.astro.com/ftp/swisseph/ephe/seas_18.se1
wget https://www.astro.com/ftp/swisseph/ephe/s136108s.se1
wget https://www.astro.com/ftp/swisseph/ephe/s136199s.se1
```

## Note

The Swiss Ephemeris is free for certain uses. Please check the licensing terms at:
https://www.astro.com/swisseph/swephinfo_e.htm
