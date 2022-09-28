#include<iostream>
#include<cmath>
#include<fstream>
#include<stdlib.h>
#include"Decode.h"
#include"TimeCoor.h"
#include"BrdcPro.h"
#include"Utilities.h"
using namespace std;

int main()
{
    ifstream binRead;
    // binRead.open("/home/zsj/SelfFile/SPP_RT/data/NovatelOEM20211114-02.log",ios::binary);
    FILE* fp;
    fp=fopen("/home/zsj/SelfFile/SPP_RT/data/NovatelOEM20211114-02.log","rb");
    RawObs      obs;
    int i=0;
    RangeOut("/home/zsj/SelfFile/SPP_RT/data/NovatelOEM20211114-02.log","out.txt");
    // COMMONTIME ct;
    // JULIANTIME mjd;
    // ct.year=2022;
    // ct.month=9;
    // ct.day=15;
    // ct.hour=3;
    // ct.min=47;
    // ct.sec=46;
    // GPSTIME gt;
    // COMMONTIME utc;
    // COMMON2GPST(ct,gt);
    // COMMON2MJD(ct,mjd);
    // MJD2COMMON(mjd,utc);
    // GPST2COMMON(gt,utc);
    // double blh[3]={38.8,113.6,100}; 
    // XYZ xyz;
    // BLH blh_;
    // blh_.B=38.8/180.0*M_PI;
    // blh_.L=113.6/180.0*M_PI;
    // blh_.H=100;
    // // BLH2XYZ(blh_,xyz,'G');
    // BLH2XYZ(blh_,xyz,'C');
    // XYZ2BLH(xyz,blh_,'C');
    // XYZ2BLH(xyz,blh_,'G');
    
    // cout<<ct.year<<"\n"<<ct.month<<"\n"<<ct.day<<"\n"<<ct.hour<<"\n"<<ct.min
    // <<"\n"<<ct.sec<<endl;
}