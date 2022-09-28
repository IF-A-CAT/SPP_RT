/***********************
 * Square root information filter
 * modified 2022/9/13
 * *********************/
#pragma once
#include"../include/Matrix.h"


class Srif
{
    public:
    Srif();

    /***************************
     * init Srif
     * design mat + observation minus calculated + covariance mat
     * ************************/
    void   init(Matrix designMat,Matrix omc,Matrix weight);

    /**************************
     * time update of state 
     * state transformation matrix + noise matrix
     * ************************/
    void   time_update(Matrix stateTranMat,Matrix noise);

    /**************************
     * measurement update
     * coef list+omc + sigma
     * ************************/                                  
    void   meas_update(double *,double omc,double sigma); 

    /**************************
     * solve estimation at one epoch
     * **********************/
    void   sovle();

    /**************************
     * return covariance of _x
     * ************************/                                                                                                                                
    Matrix Qxx(); 
                                                                                      //get covariance matrix
    /**************************
     * return estimated _x
     * ***********************/
    Matrix get_x();
    private:
    int    _epoch;                                                                                  //epoch of one solved
    bool   _isInit;


    Matrix _x;                                                                                      //parameters to be estimated
    Matrix _AMat;                                                                                   //design matrix
    Matrix _omc;                                                                                    //observe - calculated
    Matrix _infoMat;                                                                                //information mat
};
/************************
 * solve equation by iteration
 * used in Srif solve
 * **********************/
void iter_solve(const Matrix& R,const Matrix& omc,Matrix& x);