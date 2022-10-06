#include"../include/BrdcPro.h"
#include"../include/Const.h"
#include<cmath>
#include<iostream>
#include"../include/Matrix.h"
#include<cstring>
using std::cerr;
/// @brief init a broadcast ephemeris of GPS sat
/// @param prn prn num
/// @param gt  gps time
/// @param paras  list as [a,e,omega,Omega,i,M0,delN,delOmega,delI
///                        uc,us,rc,rs,ic,is,toc,clkPars[3]] 
void EPHGPS::init(int prn,const GPSTIME& gt,const double* paras)
{
    GT=gt;
    GPST2COMMON(gt,CT);
    Prn=prn;
    _sqrtA=sqrt(paras[0]);      _e=paras[1];
    _omega=paras[2];            _Omega=paras[3];
    _i=paras[4];                _M0=paras[5];
    _deltaN=paras[6];           _deltaOmega=paras[7];
    _deltaI=paras[8];           _Uc=paras[9];
    _Us=paras[10];              _Rc=paras[11];
    _Rs=paras[12];              _Ic=paras[13];
    _Is=paras[14];              _toc=paras[15];
    _tgd=paras[16];
    memcpy(_clkParas,paras+17,sizeof(double)*3);
}

/// @brief same with get_pos(const GPSTIME& gt,XYZ &xyz) but xyz is an array
/// @param gt gps time
/// @param xyz array(3)
void EPHGPS::plane_pos(const GPSTIME& gt,double* xy)
{
    double M,E,f,u,r,deltaT;
    if(gt.week-GT.week==1)
    {
        deltaT=86400.0*7+gt.sec-GT.sec;
    }
    else if (gt.week-GT.week==-1)
    {
        deltaT=gt.sec-86400.0*7-GT.sec;
    }
    else
    {
        deltaT=gt.sec-GT.sec;
    }
    if(fabs(deltaT)>6*3600)
    {
        cerr<<"Brdc Ephemeris Doesn't Update For 6 Hours,The Error Is Unbearable!!!\n";
    }
   //get f/u
    M=_M0+(sqrt(GPS_GM)/pow(_sqrtA,3)+_deltaN)*deltaT;
    EcceAno(M,_e,E);
    TrueAno(_e,E,f);
    u=_omega+f;
    r=_sqrtA*_sqrtA*(1-_e*cos(E));
    //correction r
    r=r+_Rc*cos(2*u)+_Rs*sin(2*u);
    //calculate XY
    //correction u
    u=u+_Uc*cos(2*u)+_Us*sin(2*u);
    xy[0]=r*cos(u);
    xy[1]=r*sin(u);
}

/// @brief get the rectangular pos of the sat
/// @param gt gps time
/// @param xyz position
void EPHGPS::rectangular_pos(const GPSTIME& gt,double* xyz)
{
    double M,i,f,L,PosXY[2],u,gt_given,deltaT,E;
    plane_pos(gt,PosXY);
    if(gt.week-GT.week==1)
    {
        gt_given=86400.0*7+gt.sec;
    }
    else if (gt.week-GT.week==-1)
    {
        gt_given=gt.sec-86400.0*7;
    }
    else
    {
        gt_given=gt.sec;
    }
    deltaT=gt_given-GT.sec;
    M=_M0+(sqrt(GPS_GM)/pow(_sqrtA,3)+_deltaN)*deltaT;
    EcceAno(M,_e,E);
    TrueAno(_e,E,f);
    u=_omega+f;
    i = _i + _deltaI *deltaT + _Ic * cos(2*u) + _Is * sin(2*u);
    L=_Omega+(_deltaOmega-GPS_w) * gt_given -_deltaOmega * GT.sec;
    //transform matrix
    Matrix rotate,Pos(3,1),WGSPos;
    rotate=R_x(-i);
    rotate= R_z(-L) * rotate;
    // std::cout<<rotate;
    //position of satellite in plane of satellite
    Pos(0,0)=PosXY[0];
    Pos(1,0)=PosXY[1];
    Pos(2,0)=0.0;
    
    //WGS-84 location 
    WGSPos=rotate * Pos;
    // std::cout<<WGSPos;
    xyz[0]=WGSPos(0,0);
    xyz[1]=WGSPos(1,0);
    xyz[2]=WGSPos(2,0);
}

/// @brief same with ...
/// @param gt 
/// @param xyz 
void EPHGPS::rectangular_pos(const GPSTIME& gt,XYZ &xyz)
{
    double M,i,f,L,PosXY[2],u,gt_given,deltaT,E;
    plane_pos(gt,PosXY);
    if(gt.week-GT.week==1)
    {
        gt_given=86400.0*7+gt.sec;
    }
    else if (gt.week-GT.week==-1)
    {
        gt_given=gt.sec-86400.0*7;
    }
    else
    {
        gt_given=gt.sec;
    }
    deltaT=gt_given-GT.sec;
    M=_M0+(sqrt(GPS_GM)/pow(_sqrtA,3)+_deltaN)*deltaT;
    EcceAno(M,_e,E);
    TrueAno(_e,E,f);
    u=_omega+f;
    i = _i + _deltaI *deltaT + _Ic * cos(2*u) + _Is * sin(2*u);
    L=_Omega+(_deltaOmega-GPS_w) * gt_given -_deltaOmega * GT.sec;
    //transform matrix
    Matrix rotate,Pos(3,1),WGSPos;
    rotate=R_x(-i);
    rotate= R_z(-L) * rotate;
    // std::cout<<rotate;
    //position of satellite in plane of satellite
    Pos(0,0)=PosXY[0];
    Pos(1,0)=PosXY[1];
    Pos(2,0)=0.0;
    
    //WGS-84 location 
    WGSPos=rotate * Pos;

    xyz.x=WGSPos(0,0);
    xyz.y=WGSPos(1,0);
    xyz.z=WGSPos(2,0);
}

/// @brief get clock bias and clock drift 
/// @param gt gps time
/// @param biasDrif bias and drif 
void EPHGPS::clock_bias(const GPSTIME& gt,double* biasDrif)
{
    double corrBias,deltaToc,corrDrift,M,Ecc,deltaT;
    if(gt.week-GT.week==1)
    {
        deltaT=86400.0*7+gt.sec-GT.sec;
        deltaToc=86400.0*7+gt.sec-_toc;
    }
    else if (gt.week-GT.week==-1)
    {
        deltaT=gt.sec-86400.0*7-GT.sec;
        deltaToc=gt.sec-86400.0*7-_toc;
    }
    else
    {
        deltaToc=gt.sec-_toc;
        deltaT=gt.sec-GT.sec;
    }
    M=_M0+(sqrt(GPS_GM)/pow(_sqrtA,3)+_deltaN)*deltaT;
    EcceAno(M,_e,Ecc);
    corrBias=_clkParas[0]+_clkParas[1]*(deltaToc)+_clkParas[2]*pow(deltaToc,2);           //only clock
    corrBias+=-2*sqrt(GPS_GM)/light_v/light_v*_e*_sqrtA*sin(Ecc);
    // corrBias-=_tgd;
    corrDrift=_clkParas[1]+2*_clkParas[2]*deltaToc;
    corrDrift+=-2*sqrt(GPS_GM)/light_v/light_v*_e*_sqrtA*cos(Ecc)*(_deltaN+sqrt(GPS_GM)/pow(_sqrtA,3))/(1-_e*cos(Ecc));
    biasDrif[0]=corrBias;biasDrif[1]=corrDrift;
}

/// @brief get the velocity of sat
/// @param gt gps time
/// @param xyz_v  vel of xyz
void EPHGPS::rectangular_vel(const GPSTIME&gt,double* xyz_v)
{
    double dE,dPhi,dRR,dU,dI,dOmega,phi,gt_given,Omega,deltaT,Ecc,f,M
            ,i,xy[2],u,r;
    if(gt.week-GT.week==1)
    {
        gt_given=86400.0*7+gt.sec;
    }
    else if (gt.week-GT.week==-1)
    {
        gt_given=gt.sec-86400.0*7;
    }
    else
    {
        gt_given=gt.sec;
    }
    deltaT=gt_given-GT.sec;
    M=_M0+(sqrt(GPS_GM)/pow(_sqrtA,3)+_deltaN)*deltaT;
    EcceAno(M,_e,Ecc);
    TrueAno(_e,Ecc,f);
    plane_pos(gt,xy);
    phi=_omega+f;
    u=phi;
    r=_sqrtA*_sqrtA*(1-_e*cos(Ecc));
    //correction r
    r=r+_Rc*cos(2*u)+_Rs*sin(2*u);
    u=u+_Uc*cos(2*u)+_Us*sin(2*u);
    i=_i + _deltaI *deltaT + _Ic * cos(2*phi) + _Is * sin(2*phi);
    Omega=_Omega+(_deltaOmega-GPS_w) * gt_given -_deltaOmega * GT.sec;
    dE=(_deltaN+sqrt(GPS_GM)/pow(_sqrtA,3))/(1-_e*cos(Ecc));
    dPhi=sqrt((1+_e)/(1-_e))*pow(cos(f/2)/cos(Ecc/2),2)*dE;
    dU=2*(_Us*cos(2*phi)-_Uc*sin(2*phi))*dPhi+dPhi;
    dRR=pow(_sqrtA,2)*_e*sin(Ecc)*dE+2*(_Rs*cos(2*phi)-_Rc*sin(2*phi))*dPhi;
    dI=_deltaI+2*(_Is*cos(2*phi)-_Ic*sin(2*phi))*dPhi;
    dOmega=_deltaOmega-GPS_w;
    Matrix dR(3,4),result,rightMat(4,1);
    dR(0,0)=cos(Omega);dR(1,0)=sin(Omega);dR(2,0)=0.0;
    dR(0,1)=-sin(Omega)*cos(i);dR(1,1)=cos(Omega)*cos(i);dR(2,1)=sin(i);
    dR(0,2)=-(xy[0]*sin(Omega)+xy[1]*cos(Omega)*cos(i));
    dR(1,2)=(xy[0]*cos(Omega)-xy[1]*sin(Omega)*cos(i));
    dR(2,2)=0.0;
    dR(0,3)=xy[1]*sin(Omega)*sin(i);dR(1,3)=xy[1]*cos(Omega)*sin(i);
    dR(2,3)=xy[1]*cos(i);
    rightMat(0,0)=dRR*cos(u)-r*dU*sin(u);
    rightMat(1,0)=dRR*sin(u)+r*dU*cos(u);
    rightMat(2,0)=dOmega;
    rightMat(3,0)=dI;
    result=dR*rightMat;
    xyz_v[0]=result(0,0);
    xyz_v[1]=result(1,0);
    xyz_v[2]=result(2,0);
}



/// @brief calculate the eccentric anomaly
/// @param M the mean motion angle
/// @param e e
/// @param E the EccAno
void EcceAno(double M,double e,double &E)
{
    double aux,temp,deriv,deltaE=1.0;
    E=M;
    while(fabs(deltaE)>1e-12)
    {
        aux=E;
        temp=E-e*sin(E)-M;
        deriv=1-e*cos(E);
        E=E-temp/deriv;
        deltaE=aux-E;
    }
}

/// @brief calculate the true anomaly
/// @param e e
/// @param E eccentric anomaly
/// @param f true anomaly(-pi,pi)
void TrueAno(double e,double E,double &f)
{
    double temp1,temp2;
    temp1=(cos(E)-e)/(1-e*cos(E));
    temp2=sqrt(1-e*e)*sin(E)/(1-e*cos(E));
    f=atan2(temp2,temp1);
}


/**************************
 * BDS eph part
 * ************************/

void EPHBDS::init(int prn,const GPSTIME& gt,const double* paras)
{
    GT=gt;
    GPST2COMMON(gt,CT);
    Prn=prn;
    _sqrtA=paras[0];            _e=paras[1];
    _omega=paras[2];            _Omega=paras[3];
    _i=paras[4];                _M0=paras[5];
    _deltaN=paras[6];           _deltaOmega=paras[7];
    _deltaI=paras[8];           _Uc=paras[9];
    _Us=paras[10];              _Rc=paras[11];
    _Rs=paras[12];              _Ic=paras[13];
    _Is=paras[14];              _toc=paras[15];
    _tgd=paras[16];
    _tgd2=paras[17];                            //only for bds
    memcpy(_clkParas,paras+18,sizeof(double)*3);
}

bool EPH::is_update(double Toe,double Toc)
{
    if(fabs(Toe-GT.sec)<1e-5&&fabs(Toc-_toc)<1e-5) return false;
    else                                           return true;
}


void EPHBDS::plane_pos(const GPSTIME& gt,double* xy)          //two dimen
{
    double M,E,f,u,r,deltaT;
    if(gt.week-GT.week==1)
    {
        deltaT=86400.0*7+gt.sec-GT.sec;
    }
    else if (gt.week-GT.week==-1)
    {
        deltaT=gt.sec-86400.0*7-GT.sec;
    }
    else
    {
        deltaT=gt.sec-GT.sec;
    }
    if(fabs(deltaT)>6*3600)
    {
        cerr<<"Brdc Ephemeris Doesn't Update For 6 Hours,The Error Is Unbearable!!!\n";
    }
   //get f/u
    M=_M0+(sqrt(BDS_GM)/pow(_sqrtA,3)+_deltaN)*deltaT;
    EcceAno(M,_e,E);
    TrueAno(_e,E,f);
    u=_omega+f;
    r=_sqrtA*_sqrtA*(1-_e*cos(E));
    //correction r
    r=r+_Rc*cos(2*u)+_Rs*sin(2*u);
    //calculate XY
    //correction u
    u=u+_Uc*cos(2*u)+_Us*sin(2*u);
    xy[0]=r*cos(u);
    xy[1]=r*sin(u);
}

void EPHBDS::rectangular_vel(const GPSTIME&gt,double* xyz_v)
{
    double dE,dPhi,dRR,dU,dI,dOmega,phi,gt_given,Omega,deltaT,Ecc,f,M
            ,i,xy[2],u,r;
    if(gt.week-GT.week==1)
    {
        gt_given=86400.0*7+gt.sec;
    }
    else if (gt.week-GT.week==-1)
    {
        gt_given=gt.sec-86400.0*7;
    }
    else
    {
        gt_given=gt.sec;
    }
    deltaT=gt_given-GT.sec;
    M=_M0+(sqrt(BDS_GM)/pow(_sqrtA,3)+_deltaN)*deltaT;
    EcceAno(M,_e,Ecc);
    TrueAno(_e,Ecc,f);
    plane_pos(gt,xy);
    phi=_omega+f;
    u=phi;
    r=_sqrtA*_sqrtA*(1-_e*cos(Ecc));
    //correction r
    r=r+_Rc*cos(2*u)+_Rs*sin(2*u);
    u=u+_Uc*cos(2*u)+_Us*sin(2*u);
    i=_i + _deltaI *deltaT + _Ic * cos(2*phi) + _Is * sin(2*phi);
    Omega=_Omega+(_deltaOmega-BDS_w) * gt_given -_deltaOmega * GT.sec;
    dE=(_deltaN+sqrt(BDS_GM)/pow(_sqrtA,3))/(1-_e*cos(Ecc));
    dPhi=sqrt((1+_e)/(1-_e))*pow(cos(f/2)/cos(Ecc/2),2)*dE;
    dU=2*(_Us*cos(2*phi)-_Uc*sin(2*phi))*dPhi+dPhi;
    dRR=pow(_sqrtA,2)*_e*sin(Ecc)*dE+2*(_Rs*cos(2*phi)-_Rc*sin(2*phi))*dPhi;
    dI=_deltaI+2*(_Is*cos(2*phi)-_Ic*sin(2*phi))*dPhi;
    dOmega=_deltaOmega-BDS_w;
    Matrix dR(3,4),result,rightMat(4,1);
    dR(0,0)=cos(Omega);dR(1,0)=sin(Omega);dR(2,0)=0.0;
    dR(0,1)=-sin(Omega)*cos(i);dR(1,1)=cos(Omega)*cos(i);dR(2,1)=sin(i);
    dR(0,2)=-(xy[0]*sin(Omega)+xy[1]*cos(Omega)*cos(i));
    dR(1,2)=(xy[0]*cos(Omega)-xy[1]*sin(Omega)*cos(i));
    dR(2,2)=0.0;
    dR(0,3)=xy[1]*sin(Omega)*sin(i);dR(1,3)=xy[1]*cos(Omega)*sin(i);
    dR(2,3)=xy[1]*cos(i);
    rightMat(0,0)=dRR*cos(u)-r*dU*sin(u);
    rightMat(1,0)=dRR*sin(u)+r*dU*cos(u);
    rightMat(2,0)=dOmega;
    rightMat(3,0)=dI;
    result=dR*rightMat;
    xyz_v[0]=result(0,0);
    xyz_v[1]=result(1,0);
    xyz_v[2]=result(2,0);
}


void EPHBDS::clock_bias(const GPSTIME& gt,double* bias)
{
    double corrBias,deltaToc,corrDrift,M,Ecc,deltaT;
    if(gt.week-GT.week==1)
    {
        deltaT=86400.0*7+gt.sec-GT.sec;
        deltaToc=86400.0*7+gt.sec-_toc;
    }
    else if (gt.week-GT.week==-1)
    {
        deltaT=gt.sec-86400.0*7-GT.sec;
        deltaToc=gt.sec-86400.0*7-_toc;
    }
    else
    {
        deltaToc=gt.sec-_toc;
        deltaT=gt.sec-GT.sec;
    }
    M=_M0+(sqrt(BDS_GM)/pow(_sqrtA,3)+_deltaN)*deltaT;
    EcceAno(M,_e,Ecc);
    corrBias=_clkParas[0]+_clkParas[1]*(deltaToc)+_clkParas[2]*pow(deltaToc,2);           //only clock
    corrBias+=-2*sqrt(BDS_GM)/light_v/light_v*_e*_sqrtA*sin(Ecc);
    // corrBias1=corrBias-_tgd;
    // corrBias2=corrBias-_tgd2;
    corrDrift=_clkParas[1]+2*_clkParas[2]*deltaToc;
    corrDrift+=-2*sqrt(BDS_GM)/light_v/light_v*_e*_sqrtA*cos(Ecc)*(_deltaN+sqrt(BDS_GM)/pow(_sqrtA,3))/(1-_e*cos(Ecc));
    bias[0]=corrBias;bias[1]=corrDrift;
}

void EPHBDS::rectangular_pos(const GPSTIME& gt,XYZ &xyz)
{
    double M,i,f,L,PosXY[2],u,gt_given,deltaT,E;
    plane_pos(gt,PosXY);
    if(gt.week-GT.week==1)
    {
        gt_given=86400.0*7+gt.sec;
    }
    else if (gt.week-GT.week==-1)
    {
        gt_given=gt.sec-86400.0*7;
    }
    else
    {
        gt_given=gt.sec;
    }
    deltaT=gt_given-GT.sec;
    M=_M0+(sqrt(BDS_GM)/pow(_sqrtA,3)+_deltaN)*deltaT;
    EcceAno(M,_e,E);
    TrueAno(_e,E,f);
    u=_omega+f;
    i = _i + _deltaI *deltaT + _Ic * cos(2*u) + _Is * sin(2*u);
    L=_Omega+(_deltaOmega-BDS_w) * gt_given -_deltaOmega * GT.sec;
    //transform matrix
    Matrix rotate,Pos(3,1),WGSPos;
    rotate=R_x(-i);
    rotate= R_z(-L) * rotate;
    // std::cout<<rotate;
    //position of satellite in plane of satellite
    Pos(0,0)=PosXY[0];
    Pos(1,0)=PosXY[1];
    Pos(2,0)=0.0;
    
    //WGS-84 location 
    WGSPos=rotate * Pos;

    xyz.x=WGSPos(0,0);
    xyz.y=WGSPos(1,0);
    xyz.z=WGSPos(2,0);
}

void EPHBDS::rectangular_pos(const GPSTIME& gt,double* xyz)
{
    double M,i,f,L,PosXY[2],u,gt_given,deltaT,E;
    plane_pos(gt,PosXY);
    if(gt.week-GT.week==1)
    {
        gt_given=86400.0*7+gt.sec;
    }
    else if (gt.week-GT.week==-1)
    {
        gt_given=gt.sec-86400.0*7;
    }
    else
    {
        gt_given=gt.sec;
    }
    deltaT=gt_given-GT.sec;
    M=_M0+(sqrt(BDS_GM)/pow(_sqrtA,3)+_deltaN)*deltaT;
    EcceAno(M,_e,E);
    TrueAno(_e,E,f);
    u=_omega+f;
    i = _i + _deltaI *deltaT + _Ic * cos(2*u) + _Is * sin(2*u);
    L=_Omega+(_deltaOmega-BDS_w) * gt_given -_deltaOmega * GT.sec;
    //transform matrix
    Matrix rotate,Pos(3,1),WGSPos;
    rotate=R_x(-i);
    rotate= R_z(-L) * rotate;
    // std::cout<<rotate;
    //position of satellite in plane of satellite
    Pos(0,0)=PosXY[0];
    Pos(1,0)=PosXY[1];
    Pos(2,0)=0.0;
    
    //WGS-84 location 
    WGSPos=rotate * Pos;
    // std::cout<<WGSPos;
    xyz[0]=WGSPos(0,0);
    xyz[1]=WGSPos(1,0);
    xyz[2]=WGSPos(2,0);
}