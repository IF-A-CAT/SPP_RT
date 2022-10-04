/*********************
 * in this part the process of the Broadcast navigation data
 * modified 2022/9/20
 * *******************/
#pragma once
#include"TimeCoor.h"
/// @brief  the broadcast of GPS satellite and it's process 
class EPH
{
    public:
    EPH(){};
    virtual void init(int prn,const GPSTIME &gt,const double* paras){return ;};
    virtual void rectangular_pos(const GPSTIME& gt,XYZ &xyz){return ;};
    virtual void rectangular_pos(const GPSTIME& gt,double* xyz){ return;};
    virtual void clock_bias(const GPSTIME& gt,double* bias){return;};
    virtual void rectangular_vel(const GPSTIME&gt,double* xyz_v){return ;};
    bool is_update(double Toe,double Toc);
    protected:
    virtual void plane_pos(const GPSTIME& gt,double* xy){return ;};
    //***************** base information**********************
    unsigned short int              Prn;
    COMMONTIME                      CT;
    GPSTIME                         GT;
    protected:
    //*****************data information***********************
    //**************temp value***********
    //********kepler_6********
    double                          _Omega;
    double                          _M0;
    double                          _e;
    double                          _sqrtA;
    double                          _omega;
    double                          _i;
    //********perturbation paras**********
    double                          _deltaN;
    double                          _deltaOmega;
    double                          _deltaI;
    double                          _Uc,_Us;
    double                          _Ic,_Is;
    double                          _Rc,_Rs;
    //***********clock related ************
    double                          _tgd;       //tgd1 for bds
    double                          _toc;
    double                          _clkParas[3];
};

class EPHGPS:public EPH
{
    public:
    void init(int prn,const GPSTIME& gt,const double* paras);
    void rectangular_pos(const GPSTIME& gt,XYZ &xyz);
    void rectangular_pos(const GPSTIME& gt,double* xyz);
    void plane_pos(const GPSTIME& gt,double* xy);          //two dimen
    void clock_bias(const GPSTIME& gt,double* bias);
    void rectangular_vel(const GPSTIME&gt,double* xyz_v);
};

class EPHBDS:public EPH
{
    public:
    void init(int prn,const GPSTIME& gt,const double* paras);
    void rectangular_pos(const GPSTIME& gt,XYZ &xyz);
    void rectangular_pos(const GPSTIME& gt,double* xyz);
    void plane_pos(const GPSTIME& gt,double* xy);          //two dimen
    void clock_bias(const GPSTIME& gt,double* bias);
    void rectangular_vel(const GPSTIME&gt,double* xyz_v);

    private:
    double                          _tgd2;
};


void EcceAno(double M,double e,double &E);

void TrueAno(double e,double E,double &f);