#include"../include/TimeCoor.h"
#include"../include/Const.h"
#include<cmath>

void XYZ2BLH(const XYZ& xyz,BLH& blh,char paraType)
{
    double x=xyz.x,y=xyz.y,z=xyz.z;
    double a,f,r,phi,w,n,e,temp;
    if(paraType=='G')                                                                               //use GPS geodetic paras
    {
        a=GPS_A;
        f=GPS_F;
    }
    else                                                                                            // if (paraType=='C') only two system G C
    {
        a=BDS_A;
        f=BDS_F;
    }
    e=sqrt(f*(2-f));
    phi=atan(z/sqrt(pow(x,2)+pow(y,2)));
    r=sqrt(pow(x,2)+pow(y,2)+pow(z,2));
    temp=10.0;
    blh.L=atan2(y,x);
    blh.H=0.0;
    blh.B=atan(z/sqrt(x*x+y*y));
    while(fabs(temp-blh.B)>1e-10)
    {
        temp=blh.B;
        w=sqrt(1-e*e*sin(blh.B)*sin(blh.B));
        n=a/w;
        blh.B=atan(tan(phi)*(1+a*e*e/z*sin(blh.B)/w));
        blh.H=r*cos(phi)/cos(blh.B)-n;
    }
}

void BLH2XYZ(const BLH& blh, XYZ& xyz,char paraType)
{
    double a,f,e2,n;
    if(paraType=='G')
    {
        a=GPS_A;
        f=GPS_F;
    }
    else                                                                                            // if (paraType=='C') only two system G C
    {
        a=BDS_A;
        f=BDS_F;
    }
    e2=f*(2-f);
    n=a/sqrt(1-e2*sin(blh.B)*sin(blh.B));
    xyz.x=(n+blh.H)*cos(blh.B)*cos(blh.L);
    xyz.y=(n+blh.H)*cos(blh.B)*sin(blh.L);
    xyz.z=(n*(1-e2)+blh.H)*sin(blh.B);
}

void COMMON2MJD(const UTCTIME& utc,JULIANTIME& mjd)
{
    int iJd,year,month;
    double fracJd,hour;
    if(utc.month<=2)
    {
        year=utc.year-1;
        month=12+utc.month;
    }
    else
    {
        year=utc.year;
        month=utc.month;
    }
    hour=utc.hour+utc.min/60.0+(utc.sec)/3600.0;
    iJd=floor(int(365.25*year)+int(30.6001*(1+month))+
        utc.day+hour/24+1720981.5-2400000.5);
    fracJd=int(365.25*year)+int(30.6001*(1+month))+
        utc.day+hour/24+1720981.5-iJd-2400000.5;
    mjd.iJD=iJd;
    mjd.fracJD=fracJd;
}

void MJD2COMMON(const JULIANTIME& mjd,UTCTIME& cm)
{
    int iJd,b,d,e,c;
    double fracJd;
    iJd=2400000+mjd.iJD;
    fracJd=0.5+mjd.fracJD;
    b=int(iJd+fracJd+0.5)+1537;
    c=int((b-122.1)/365.25);
    d=int(365.25*c);
    e=int((b-d)/30.6001);
    cm.day=b-d-int(30.6001*e);
    cm.month=e-1-12*int(e/14.0);
    cm.year=c-4715-int((7+cm.month)/10.0);
    cm.hour=int(mjd.fracJD*24);
    cm.min=int(60.0*(mjd.fracJD*24-cm.hour));                                                                                      //can't solve second
    cm.sec=((mjd.fracJD*24-cm.hour)*60.0-cm.min)*60.0;
}


void MJD2GPST(const JULIANTIME& mjd,GPSTIME& gt)
{
    gt.week=int((mjd.iJD+mjd.fracJD-44244)/7);
    gt.sec=(mjd.fracJD+mjd.iJD-44244-gt.week*7)*86400.0;
}

void GPST2MJD(const GPSTIME& gt,JULIANTIME& mjd)
{
    mjd.iJD=floor(44244+gt.week*7+gt.sec/86400.0);
    mjd.fracJD=44244+gt.week*7+gt.sec/86400.0-mjd.iJD;
}

void COMMON2GPST(const UTCTIME& utc,GPSTIME& gt)
{
    int i,totalDay=0;
    for(i=1980;i<utc.year;i++)
    {
        if(i%400==0||(i%4==0&&i%100!=0)) totalDay+=366;
        else                             totalDay+=365;
    } 
    for(i=1;i<utc.month;i++)
    {
        if(i==1||i==3||i==5||i==7||i==8||i==10||i==12)
        {
            totalDay+=31;
        }
        else if(i==4||i==6||i==9||i==11)
        {
            totalDay+=30;
        }
        else
        {
            if((utc.year%4==0&&utc.year%100!=0)||utc.year%400==0)
            {
                totalDay+=29;
            }
            else
            {
                totalDay+=28;
            }
        }
    }
    totalDay+=utc.day-6;
    gt.week=totalDay/7;
    gt.sec=totalDay%7*86400.0+utc.hour*3600.0+utc.min*60.0+utc.sec;
}

void GPST2COMMON(const GPSTIME& gt,UTCTIME& utc)
{
    JULIANTIME tempJD;
    GPST2MJD(gt,tempJD);
    MJD2COMMON(tempJD,utc);
}

void GPST2DOY(const GPSTIME& gt,DOYTIME& doy)
{
    int i,totalDay=0;
    UTCTIME utc;
    GPST2COMMON(gt,utc);
    doy.year=utc.year;

    for(i=1;i<utc.month;i++)
    {
        if(i==1||i==3||i==5||i==7||i==8||i==10||i==12)
        {
            totalDay+=31;
        }
        else if(i==4||i==6||i==9||i==11)
        {
            totalDay+=30;
        }
        else
        {
            if((utc.year%4==0&&utc.year%100!=0)||utc.year%400==0)
            {
                totalDay+=29;
            }
            else
            {
                totalDay+=28;
            }
        }
    }
    totalDay+=utc.day;
    doy.day=int(totalDay+(utc.hour*3600.0+utc.min*60.0+utc.sec)/86400.0);
    doy.fracDay=totalDay+(utc.hour*3600.0+utc.min*60.0+utc.sec)/86400.0-doy.day;
}

void DOY2GPST(const DOYTIME& doy,GPSTIME& gt)
{
    int i;
    double totalDay=0;
    for(i=1980;i<doy.year;i++)
    {
        if(i%400==0||(i%4==0&&i%100!=0)) totalDay+=366;
        else                             totalDay+=365;
    }
    totalDay+=doy.day;
    gt.week=floor((totalDay+doy.fracDay)/7.0);
    gt.sec=fmod(totalDay+doy.fracDay,7.0)*86400.0;
}



