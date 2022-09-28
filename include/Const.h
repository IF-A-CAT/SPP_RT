/************************
 * const paras in of geodesy field
 * modified 2022/9/13
 * **********************/
#pragma once

#define GPS_A                               6378137.0                                                                             //wgs-84 long semi length
#define GPS_F                               1.0/298.257223563
#define BDS_A                               6378137.0
#define BDS_F                               1.0/298.257222101

#define MAXSYS                              4   
#define MAXOBSPERSYS                        18
#define MAXOBS                              MAXSYS*MAXOBSPERSYS*4
#ifdef WIN32
#define pi                                  3.1415926535898
#endif

#define pi                                  M_PI
#define light_v                             299792458.0             //speed of light
#define GPS_GM                              3.986005e14
#define GPS_w                               7.2921151467e-5
#define BDS_GM                              3.986004418e14          // m3/s2
#define BDS_w                               7.2921150e-5            // rad/s

enum SYS{GPS=0,GLONASS,GALILEO,BDS,OTHER};

/*****************observation type*************/
enum OBSTYPE{C1CG=0,D1CG,L1CG,S1CG,C2LG,D2LG,L2LG,S2LG,C2IC,D2IC,L2IC,S2IC,C7IC,D7IC,L7IC,S7IC};

enum FREQ{L1=0,L2,L3,L4,L5};//for bds L1/2/3/4/5  B1 B2 B3 B1C B2a

enum SIGNALTYPE{C1ENTRY=0,P1ENTRY,C2ENTRY,P2ENTRY,Q5ENTRY};

enum   {NGPS=32,NGAL=NGPS+28,NGLO=NGAL+26,NBDS=NGLO+49};             //the count of satellite