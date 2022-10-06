/************************
 * decode binary file or data stream when using real time data stream and binary file
 * From this part on , the annotation is used like 
 ///@
 ///@
 ///@
 *modified 2022/9/20
 **********************/
#pragma once
#include<iostream>
#include<cstring>
#include<string>
#include<stdint.h>
#include"TimeCoor.h"
#include"BrdcPro.h"
#include"Const.h"
#include<stdlib.h>
using std::cin;
using std::string;


// typedef signed char     uint8_t;// 8 bits
// typedef short int       uint16_t;//16 bits
// typedef unsigned int    uint32_t;//32 bits
// typedef int             int32_t;//signed int

#define MAXBINARYLEN    16384
#define POLYCRC32       0xEDB88320L //crc correct 
#define OEM4SYNC1       0xAA    /* oem7/6/4 message start sync code 1 */
#define OEM4SYNC2       0x44    /* oem7/6/4 message start sync code 2 */
#define OEM4SYNC3       0x12    /* oem7/6/4 message start sync code 3 */
#define OME4HLEN        28      //the header length(bytes) may changed in the future

#define ID_RANGE        43      // the range measurement
#define ID_EPHGPS       7      // the ephemeris
#define ID_IONUTC       8       //iono and utc data
#define ID_EPHBDS       1696    //BDS ephemeris pos

// uint8_t  U1(uint8_t* p);
uint16_t U2(uint8_t* p){uint16_t u; memcpy(&u,p,2); return u;}// read unsigned short int
uint32_t U4(uint8_t* p){uint32_t u; memcpy(&u,p,4); return u;}// read unsigned int
int32_t  I4(uint8_t* p){int32_t  i; memcpy(&i,p,4); return i;}// read signed int
float    R4(uint8_t* p){float    f; memcpy(&f,p,4); return f;}// read float
double   R8(uint8_t* p){double   d; memcpy(&d,p,8); return d;}// read double


/// @brief To store the data fo one epoch
struct GNSSDATA
{
    string                         siteName;
    int                            siteCode;
    int                            obsCount=0;
    GPSTIME                        gt;
    int                            satellites[MAXOBS*4];
    double                         measData[MAXOBS*4][20];                      //measure data
    int                            measType[MAXOBS*4][5];                       //measuretype of one obs C1 P1 C2 ...
    XYZ                            xyz;
    BLH                            blh;                                         //approximate position
    GNSSDATA()
    {
        for(int i=0;i<MAXOBS*4;i++)
        {
            for(int j=0;j<5;j++)
            {
                measType[i][j]=99;
            }
        }
    }
};

/// @brief Raw observation of a message
struct RawObs
{
    GPSTIME                     gt;             //message sent time                 
    GNSSDATA               obsData;             //GNSS DATA
    //星历结构
    int                  iSatNum=0;             //number of satellites      
    int                      len=0;             // not including header and 4 crc
    uint8_t   buffer[MAXBINARYLEN];             //Max of a message
    int                      iByte;             //bytes of message
    int                    headerL;             //length of header
    int                   obsNum=0;             //obs count differ by fre and sat
    int                     messID;             //message ID
    EPH**                       eph;
    RawObs()
    {
        eph=new EPH* [200];
        for(int i=0;i<200;i++)
        {
            eph[i]=new EPH();
        }
    }
};
bool SyncOem(RawObs& rawobs,uint8_t thibuffer);

bool ReadMessage(FILE* fp,RawObs& rawSet);

bool DecodeOme4(RawObs& rawobs);

bool Crc32Check(uint8_t* buffer,int len);

void TrackStat(uint32_t stat,SYS& sys,int& parity,SIGNALTYPE& freq);

void DecodeRange(RawObs& rawobs);

void DecodeGPSEph(RawObs& rawobs);

void DecodeBDSEph(RawObs& rawobs);

void DecodePSRPos(RawObs& rawobs);