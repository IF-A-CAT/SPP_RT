/********************************************
 * time structures and coordinate transformation 
 * modified 2022/9/13
 * ******************************************/
#pragma once

/***************
 * const paras
 * JD_MJD julian date - modified julian date
 * GPS_UTC GPST-UTC(s)  changed by leap second 
 * ************/
#define JD_MJD 2400000.5
#define GPS_UTC 18

/********************
 * UTC Structure
 * year month day hour minute second frac_second
 * ******************/
typedef struct COMMONTIME
{
    unsigned short year;
    unsigned short month;
    unsigned short day;
    unsigned short hour;
    unsigned short min;
    double sec;
    COMMONTIME()
    {
        year=0;month=0;day=0;hour=0;min=0;sec=0;
    }
} UTCTIME;

/*********************
 * Julian Data
 * int(day)+double(less than 1 day)
 * ******************/
struct JULIANTIME
{
    int iJD;
    double fracJD;
    JULIANTIME():iJD(0),fracJD(.0){}
};

/********************
 * GPS TIME
 * GPS week(int) + second(double)  -> second can be divide into int+double
 * *****************/
typedef struct GPSTIME
{
    unsigned short week;
    double sec;
    GPSTIME():week(0),sec(.0){}
} BDSTIME;


/*********************
 * XYZ- Cartesian -rectangular coordinate system
 * XYZ(double)
 * ******************/
struct XYZ
{
    double x;
    double y;
    double z;
    double sigmax,sigmay,sigmaz;
    XYZ():x(.0),y(.0),z(.0){}
    XYZ(double * xyz):x(xyz[0]),y(xyz[1]),z(xyz[2]){}
};

/*********************
 * BLH geodetic coordinate system
 * B(rad) L(rad) H(m)
 * *******************/
struct BLH
{
    double B;
    double L;
    double H;
    double sigmab,sigmal,sigmah;
    BLH():B(.0),L(.0),H(.0){}
};

/**************************
 * DOY day of year
 * year+ day of year+second of day
 * ************************/
struct DOYTIME
{
    unsigned short year;
    unsigned short day;
    double fracDay;
    DOYTIME():year(0),day(0),fracDay(.0){}
};
 
/**************************
 * XYZ rectangular to BLH geodetic 
 * XYZ(m) BLH(rad,rad,m)
 * paraType the type of the system (GBER)
 * *************************/
void XYZ2BLH(const XYZ& xyz,BLH& blh,char paraType);

/**************************
 * BLH to XYZ 
 * BLH(rad,rad,m) XYZ(m) 
 * paraType the type of the system (GCER)
 * *************************/
void BLH2XYZ(const BLH& blh, XYZ& xyz,char paraType);

/***************************
 * commonTime to modified julian date
 * UTCTIME(common time) MJD(Modified julian date)
 * ************************/
void COMMON2MJD(const UTCTIME& utc,JULIANTIME& mjd);

/***************************
 * modified julian date to common time
 * modified julian date(mjd) common time (utc)
 * *************************/
void MJD2COMMON(const JULIANTIME& mjd,UTCTIME& utc);

/****************************
 * modified julian date to gps time
 * julian date structure +gps time structure
 * **************************/
void MJD2GPST(const JULIANTIME& mjd,GPSTIME& gt);

/***************************
 * gps time to modified julian date
 * gps time structure + julian date structure
 * *************************/
void GPST2MJD(const GPSTIME& gt,JULIANTIME& mjd);

/**************************
 * utc to gps time
 * utc structure + gps time structure
 * ***********************/
void COMMON2GPST(const UTCTIME& utc,GPSTIME& gt);

/*************************
 * gps time to utc time  (leap sec)
 * gps time structure + utc structure
 * ***********************/
void GPST2COMMON(const GPSTIME& gt,UTCTIME& utc);

/**************************
 * gps time to day of year
 * gps time structure + doy structure
 * ***********************/
void GPST2DOY(const GPSTIME& gt,DOYTIME& doy);

/************************
 * day of year to gps time
 * doy structure + gps time structure
 * **********************/
void DOY2GPST(const DOYTIME& doy,GPSTIME& gt);


