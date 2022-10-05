#include"../include/SrifPro.h"
#include<cmath>

Srif::Srif():_isInit(false),_epoch(0){}

/// @brief init the square root information filter
/// @param designMat designed matrix (coef matrix)
/// @param omc      observed minus calculated
/// @param weight   
void  Srif::init(Matrix designMat,Matrix omc,Matrix weight,int*paratyoe)
{
    _AMat=Cholesky(weight)* designMat;
    _omc=Cholesky(weight)*omc;
    if(designMat.get_col()>designMat.get_row())
    {
        _isInit=false;
    }
    else
    {
        _isInit=true;
    }
}

void   Srif::time_update(Matrix stateTranMat,Matrix noise)
{
    _x=stateTranMat*_x;
    (Inv_LU(Trans(_infoMat)*_infoMat)+noise);
}

/// @brief solve the srif 
void Srif::sovle()
{
    Matrix Q,R;
    if(!_isInit)
    {
        return;
    }
    else
    {
            QR_decompose(_AMat,Q,R);
            iter_solve(R,Trans(Q)*_omc,_x);
            _infoMat=R;
            _epoch++;
    }
}

/// @brief measurement update
/// @param coef  coef of the paras
/// @param omc  
/// @param sigma sigma 
void  Srif:: meas_update(double * coef,double omc,double sigma)
{
    int ncol=_x.get_row();
    Matrix newVec(coef,1,ncol);
    _omc=MergeByRow(_omc,omc/sigma);
    _AMat=MergeByRow(_AMat,newVec/sigma);
}

Matrix Srif:: Qxx()
{
    return Inv_LU(Trans(_infoMat)*_infoMat);
}

Matrix Srif:: get_x()
{
    return _x;
}

void iter_solve(const Matrix& R,const Matrix& omc,Matrix& x)
{
    int i,j;
    int nParas=R.get_col();
    double sum=0.0;
    Matrix tempX(nParas,1);
    for(i=0;i<nParas;i++)
    {
        sum=0.0;
        for(j=0;j<i;j++)
        {
            sum+=x(nParas-j,1)*R(nParas-i,nParas-j);
        }
        tempX(nParas-i,1)=(omc(nParas-i,1)-sum)/R(nParas-i,nParas-i);
    }
    x=tempX;
}