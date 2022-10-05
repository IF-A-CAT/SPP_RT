/***************************
 * All Utilities used in Process
 * *************************/

/***************************
 * Atan to solve Azimuth which can't be solved by normal atan
 * delx+dely
 * *************************/
#pragma once
#include"../include/Const.h"
#include"../include/Decode.h"
#include"../include/TimeCoor.h"
#include<cmath>
#include<fstream>
#include<string>
#include<iomanip>
#include<string>
#include<sstream>
using std::fixed;
using std::setprecision;
using std::setw;
using std::setfill;
using std::to_string;
using std::stringstream;
using std::string;

double AtanAzim(double delx,double dely);

string GetSysByPrn(int prn);

void FormatOut(string dataname,string outname);

stringstream ObsFormat(const GNSSDATA& data);

void RangeOut(string filename,string outname);

GPSTIME GPST2BDST(const GPSTIME& gt);

