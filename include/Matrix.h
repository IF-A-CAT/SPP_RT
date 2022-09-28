/**************************************
 * This for matrix calculation
 * modified 2022/9/13
 * **************************************/

#pragma once 
#include<iostream>
const static double eps=__DBL_EPSILON__*10;                                                        //minus value of double


class Matrix
{
    public:
        Matrix();
        Matrix(int r,int c);
        Matrix(double *array,int r,int c);
        Matrix(const Matrix &M);

        ~Matrix();
        /**********************the operator of the matrix**********************************/
        double operator()(int r,int c) const{return M[r][c];};
        double& operator()(int r,int c) {return M[r][c];};
        Matrix& operator=(const double &value);
        Matrix& operator=(const Matrix &M);
        friend Matrix operator+(const Matrix & M,const double &value);
        friend Matrix operator+(const double &value,const Matrix & M);
        friend Matrix operator+(const Matrix & M1,const Matrix &M2);
        friend Matrix operator-(const Matrix & M,const double &value);
        friend Matrix operator-(const double &value,const Matrix & M);
        friend Matrix operator-(const Matrix & M1,const Matrix &M2);
        friend Matrix operator-(const Matrix &M);
        friend Matrix operator*(const double &value,const Matrix &M);
        friend Matrix operator*(const Matrix & M,const double &value);
        friend Matrix operator*(const Matrix & M1,const Matrix &M2);
        friend Matrix operator|(const Matrix & M1,const Matrix &M2);                               //merge two matrix with the same row(by col)
        friend Matrix MergeByRow(const Matrix& M1,const Matrix &M2);                                //merge two matrix by row
        friend Matrix MergeByRow(const Matrix& M1,const double &M2);
        friend Matrix operator/(const Matrix &M,const double &value);
        friend Matrix operator==(const Matrix &M1,const Matrix &M2);
        /***********************************end*********************************/

        /*********************************heigher transformation of matrix*****************/
        friend Matrix Trans(const Matrix &M);
        friend void QR_decompose(const Matrix &M,Matrix &Q,Matrix &R);
        friend Matrix Inv_LU(const Matrix &M);                                                     //solve Inv by using LU-decompose
        friend Matrix Inv_QR(const Matrix &M);                                                     //solve Inv by using QR-decompose
        friend Matrix LUinv(const Matrix &M);                                                      //inv of L or U Matrix 
        friend double Norm(const Matrix &M);                                                        
        friend Matrix Cholesky(const Matrix &M);                        
        friend bool Is_L(const Matrix &M);                      
        friend bool IsColFullRank(const Matrix &M);                                                 
        friend std::ostream& operator<<(std::ostream &os,const Matrix &V);
        /*********************end********************************/     

        /****************************public operation********************/                
        int get_row()const{return r;};
        int get_col()const{return c;};
        void exchange_row(int row1,int row2);
        Matrix slice(int r1,int r2,int c1,int c2)const;                     
        /****************************end**************************/
    private:
        double **M;
        int r;
        int c;
};

Matrix Eye(int n);
Matrix R_x( double alpha);                                                                         //transfer mat for X axe
Matrix R_y( double alpha);                                                                         //transfer mat for Y axe
Matrix R_z( double alpha);                                                                         //transfer mat for Z axe