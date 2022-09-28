#include"../include/Matrix.h"
#include<iostream>
#include<cmath>
#include<iomanip>
using namespace std;

Matrix::Matrix():r(0),c(0){
    M=new double *[1]; 
}

Matrix::Matrix(int R,int C):r(R),c(C){
    M=new double*[r];
    int i,j;
    for(i=0;i<r;i++)    M[i]=new double [c];
    for(i=0;i<r;i++){
        for(j=0;j<c;j++)
            M[i][j]=0.0;
    }
}
Matrix::Matrix(const Matrix &M1):c(M1.c),r(M1.r){
    M=new double*[r];
    int i,j;
    for(i=0;i<r;i++)    M[i]=new double [c];
    for(i=0;i<r;i++){
        for(j=0;j<c;j++)
            M[i][j]=M1.M[i][j];
    }
}

Matrix::Matrix(double *m,int R,int C):r(R),c(C){
    M=new double*[r];
    int i;
    for(i=0;i<r;i++)    M[i]=new double [c];
    for(i=0;i<r;i++){
        for(int j=0;j<c;j++)
            M[i][j]=m[i*c+j];
    }
}

Matrix& Matrix::operator=(const double &value){
    for(int i=0;i<r;i++)
        delete [] M[i];
    delete [] M;
    M=new double *[1];
    M[0]=new double [1];
    M[0][0]=value;
    r=1;
    c=1;
    return *this;
}

Matrix::~Matrix(){
    for(int i=0;i<r;i++)
        delete [] M[i];
    delete [] M;
}

Matrix operator+(const Matrix & M,const double &value){
    Matrix M1(M);
    for(int i=0;i<M1.r;i++){
        for(int j=0;j<M1.c;j++)
            M1(i,j)=M(i,j)+value;
    }
    return M1;
}
Matrix operator+(const double &value,const Matrix & M){
    Matrix M1(M);
    for(int i=0;i<M1.r;i++){
        for(int j=0;j<M1.c;j++)
            M1(i,j)=M(i,j)+value;
    }
    return M1;
}

Matrix operator+(const Matrix & M1,const Matrix &M2){
    if(M1.r!=M2.r||M1.c!=M2.c){
        cerr<<"ERROR:Tow matrixes' dims are different,please check them"<<endl;
        exit(1);
    }
    Matrix M3(M1);
    for(int i=0;i<M3.r;i++){
        for(int j=0;j<M3.c;j++)
            M3(i,j)=M2(i,j)+M1(i,j);
    }
    return M3;
}

Matrix operator-(const Matrix & M,const double &value){
    Matrix M1(M);
    for(int i=0;i<M1.r;i++){
        for(int j=0;j<M.c;j++)
            M1(i,j)=M(i,j)-value;
    }
    return M1;
}

Matrix operator-(const double &value,const Matrix & M){
   Matrix M1(M);
    for(int i=0;i<M1.r;i++){
        for(int j=0;j<M.c;j++)
            M1(i,j)=-M(i,j)+value;
    }
    return M1; 
}
Matrix operator-(const Matrix & M1,const Matrix &M2){
    if(M1.r!=M2.r||M1.c!=M2.c){
        cerr<<"ERROR:Tow matrixes' dims are different,please check them"<<endl;
        exit(1);
    }
    Matrix M3(M1);
    for(int i=0;i<M3.r;i++){
        for(int j=0;j<M3.c;j++)
            M3(i,j)=M1(i,j)-M2(i,j);
    }
    return M3;
}
Matrix operator-(const Matrix &M){
    Matrix M1(M);
    for(int i=0;i<M1.r;i++){
        for(int j=0;j<M1.c;j++)
            M1(i,j)=-M(i,j);
    }
    return M1;
}

Matrix operator*(const double &value,const Matrix &M){
    Matrix M1(M);
    for(int i=0;i<M1.r;i++){
        for(int j=0;j<M1.c;j++)
            M1(i,j)=value*M(i,j);
    }
    return M1;
}
Matrix operator*(const Matrix & M,const double &value){
    Matrix M1(M);
    for(int i=0;i<M1.r;i++){
        for(int j=0;j<M1.c;j++)
            M1(i,j)=value*M(i,j);
    }
    return M1;
}
Matrix operator*(const Matrix & M1,const Matrix &M2){
    if(M1.c!=M2.r){
        cerr<<"ERROR:The column of Matrix 1 is different from the row of Matrix 2"<<endl;
        exit(1);
    }
    Matrix M3(M1.r,M2.c);
    int i,j,k;
    for(i=0;i<M3.r;i++){
        for(j=0;j<M3.c;j++)
            for(k=0;k<M1.c;k++)
                M3(i,j)+=M1(i,k)*M2(k,j);
    }
    return M3;
}

Matrix operator/(const Matrix &M,const double &value){
    Matrix M1(M);
    for (int i=0;i<M1.r;i++){
        for(int j=0;j<M1.c;j++)
            M1(i,j)=M(i,j)/value;
    }
    return M1;
}

Matrix& Matrix::operator=(const Matrix &M1){
    for(int i=0;i<r;i++)
        delete [] M[i];
    delete [] M;
    r=M1.r;
    c=M1.c;
    M=new double*[r];
    int i,j;
    for(i=0;i<r;i++)    M[i]=new double [c];
    for(i=0;i<r;i++){
        for(j=0;j<c;j++)
            M[i][j]=M1.M[i][j];
    }
    return *this;
}
Matrix MergeByRow(const Matrix& M1,const double &M2)
{
    int i;
    Matrix mergeMat(M1.get_row()+1,1);
    for(i=0;i<M1.get_row();i++)
    {
        mergeMat(i+1,1)=M1(i+1,1);
    }
    mergeMat(M1.get_row()+1,1)=M2;
}
Matrix MergeByRow(const Matrix& M1,const Matrix &M2)
{
    int i,j;
    if(M1.get_col()!=M2.get_col())
    {
        std::cerr<<"Mat1's columns is different from Mat2's"<<std::endl;
        exit(1);
    }
    Matrix mergedMat(M1.get_row()+M2.get_row(),M1.get_col());
    for(i=0;i<mergedMat.get_row();i++)
    {
        for(j=0;j<M1.get_col();j++)
        {
            if(i<M1.get_row())
            {
                mergedMat(i+1,j+1)=M1(i+1,j+1);
            }
            else
            {
                mergedMat(i+1,j+1)=M2(i+1-M1.get_row(),j+1);
            }
        }
    }
    return mergedMat;
}

ostream& operator<<(ostream &os,const Matrix &M){
    for(int i=0;i<M.r;i++){
        for(int j=0;j<M.c;j++)
            if(j!=M.c-1){
                os<<fixed<<setprecision(4)<<setw(10)<<M(i,j)<<' ';
            }
            else{
                os<<fixed<<setprecision(4)<<setw(10)<<M(i,j)<<endl;
            }
    }
    return os;
}



Matrix Trans(const Matrix &M){
    Matrix M1(M.c,M.r);
    for(int i=0;i<M1.r;i++){
        for(int j=0;j<M1.c;j++)
            M1(i,j)=M(j,i);
    }
    return M1;
}

Matrix Matrix::slice(int r1,int r2,int c1,int c2)const {
    if(r1<0||r2<0||c1<0||c2<0||r2>r-1||c2>c-1||r1>r2||c1>c2){
        cerr<<"ERROR:Index is out of range!"<<endl;
        exit(1);
    }
    int R=r2-r1+1,C=c2-c1+1;
    Matrix back(R,C);
    for(int i=0;i<R;i++){
        for(int j=0;j<C;j++)
            back(i,j)=M[r1+i][c1+j];
    }
    return back;
}

double Norm(const Matrix &M){
    if(M.c!=1){
        cerr<<"ERROR:Matrix's column is not 1,Norm fail"<<endl;
        exit(1);
    }
    double norm=0;
    for(int i=0;i<M.r;i++){
        norm+=M(i,0)*M(i,0);
    }
    return sqrt(norm);
}
//calculate inv of a matrix through lu_decompose and LuInv
Matrix Inv_LU(const Matrix &M){         
    if(M.r!=M.c){
        cerr<<"ERROR:Input matrix is not a square matrix in Inv_LU"<<endl;
        exit(1);
    }
    int n=M.r;
    double max=0.0;
    for(int i=0;i<n;i++){                                          
        for(int j=0;j<n;j++){
            if(fabs(M(i,j))>max)    max=fabs(M(i,j));
        }
        if(max<eps){
            cerr<<"ERROR:Singular matrix in Inv_LU"<<endl;
            exit(1);
        }
    }
     
    Matrix L(n,n),U(n,n);
    int k,i,j,m;
    double sum;
    L=Eye(n);
    for(k=0;k<n;k++){                                    //LU_decompose:start
        for(j=k;j<n;j++){
            sum=0.0;
            for(m=0;m<=k-1;m++)
                sum+=L(k,m)*U(m,j);
            U(k,j)=M(k,j)-sum;
        }
        for(i=k+1;i<n;i++){                             //采用待定系数法求解先求U，然后根据求解的一行U计算L的一列
            sum=0.0;                                    //由此迭代求解，前提为L矩阵的正对角线元素都为1.
            for(m=0;m<=k-1;m++){
                sum+=L(i,m)*U(m,k);
            }
            L(i,k)=(M(i,k)-sum)/U(k,k);
        }
    }                                                     //LU_decompose::end
    L=LUinv(L);U=LUinv(U);
    // cout<<''
    L=U*L;
    for(i=0;i<M.r;i++)
        {
            for(j=0;j<M.c;j++)
            {
                if(isnan(L(i,j)))
                {
                    cerr<<"ERROR:Matrix is almost a singular matrix !"<<endl;
                }
            }
        }
    return L;
}
//return upper mat
Matrix Cholesky(const Matrix &M)                                //for cholesky-decompose in Pxx
{
    int col=M.c,row=M.r;
    if(col!=row)
    {
        cerr<<"ERROR:Matrix is not square"<<endl;
        exit(1); 
    }
    Matrix L(row,col);
    double sum=0.0;

    for(int k=0;k<row;k++)
    {   sum=0.0;
        for(int i=0;i<=k-1;i++)
            sum+=L(k,i)*L(k,i);
        L(k,k)=sqrt(M(k,k)-sum);
        for(int i=0;i<col;i++)
        {   sum=0.0;
            for(int j=0;j<=k-1;j++)
                sum+=L(i,j)*L(k,j);
            L(i,k)=(M(i,k)-sum)/L(k,k);
        }
    }
    return Trans(L);
}


void QR_decompose(const Matrix &M,Matrix &Q,Matrix &R){
    int r=M.get_row(),c=M.get_col();
    double norm;
    Matrix w(r,1),Aux(r,1),trans(r,r),M1(M);
    // Matrix* QR=new Matrix [2];
    Q=Eye(r);
    for(int i=0;i<r&&i<c;i++){
        Matrix  e(r-i,1);
        for(int j=0;j<r-i;j++){
            if(j!=0){
                e(j,0)=0;
            }
            else{
                e(j,0)=1;
            }
        }
        Aux=M1.slice(i,r-1,i,i);
        norm=Norm(Aux);
        if(norm<eps){
            cerr<<"ERROR:The matrix is not column full rank,or the square matrix is singular"<<endl;
            exit(1);
        }
        // cout<<Aux;
        Aux=Aux/norm;
        // cout<<Aux;
        Aux=Aux-e;
        // cout<<Aux;
        norm=Norm(Aux);
        if(norm<eps){
            continue;
        }
        Aux=Aux/norm;
        trans=Eye(r-i);

        Aux=(trans-2*Aux*Trans(Aux));
        // cout<<Aux*Trans(Aux);
        // std::cout<<Aux<<Trans(Aux)<<endl;
        trans=Eye(r);
        for(int k=0;k<r-i;k++){
            for(int m=0;m<r-i;m++)
                trans(i+k,m+i)=Aux(k,m);
        }
        Q=trans*Q;
        M1=trans*M1;
    }
    Q=Trans(Q);
    R=M1;
}

Matrix Inv_QR(const Matrix &M){                                 //if you want to solve the inv-problem you'd better us Inv_LU
    if(M.c!=M.r){
        cerr<<"ERROR:Matrix is not a square matrix in Inv_QR"<<endl;
        exit(1);
    }
    Matrix Q,R;
    QR_decompose(M,Q,R);
    Matrix Inv(M.r,M.c);
    Inv=LUinv(R);
    // cout<<QR[1]<<'\n'<<Inv<<endl;
    Inv=Inv*Trans(Q);
    // delete [] QR;
    return Inv;
}

Matrix LUinv(const Matrix &M){  
    int r=M.r,c=M.c,i,j,k;
    double sum=0.0;
    // cout<<M<<endl;
    for(i=0;i<r;i++)
    {   sum=0.0;
        for(j=0;j<c;j++)
        {   
            if(isnan(M(i,j)))
            {
                cerr<<"ERROR:Matrix is almost a singular matrix in LUinv"<<endl;
                exit(1);
            }
            if(i==j&&M(i,j)<eps)
            {
                cerr<<"ERROR:Matrix is a singular matrix in LUinv"<<endl;
                exit(1);
            }
        }
    }                                //solve the inv-problem by using LU-decompose
    if(Is_L(M))
    {
        Matrix L_inv(r,c);
        for(i=0;i<r;i++){
            for(j=0;j<c;j++){
                if(i<j) L_inv(i,j)=0;
                else if(i==j)    L_inv(i,j)=1/M(i,i);
                else{
                    for(k=j;k<i;k++){
                        L_inv(i,j)+=(-1/M(i,i))*M(i,k)*L_inv(k,j);
                    }
                }
            }
        }
        return L_inv;
    }
    else{
        Matrix U_inv(r,c),Mt(Trans(M));
        for(i=0;i<r;i++){
            for(j=0;j<c;j++){
                if(i<j) {
                    U_inv(i,j)=0;}
                else if(i==j){   
                    U_inv(i,j)=1/Mt(i,i);}
                else{
                    for(k=j;k<i;k++){
                        U_inv(i,j)+=(-1/Mt(i,i))*Mt(i,k)*U_inv(k,j);
                    }
                }
            }
        }
        // cout<<U_inv<<endl;
        return Trans(U_inv);
    }
}

bool Is_L(const Matrix &M){
    int i,j;
    double sum=0;
    bool isL=true;
    for(j=1;j<M.c;j++){
        sum=0.0;
        for(i=0;i<j;i++){
            sum+=M(i,j);
        }        
            if(abs(sum)>eps*100){ 
                isL=false;
                break;
            }
    }
    return isL;
}


Matrix R_x(double alpha){
    double rx[9]={1.0,0.0,0.0,0.0,1.0,1.0,0.0,1.0,1.0};
    rx[4]=cos(alpha);rx[5]=sin(alpha);
    rx[7]=-sin(alpha);rx[8]=cos(alpha);
    Matrix a(rx,3,3);
    return a;
}

Matrix R_y(double alpha){
    double ry[9]={1.0,0.0,1.0,0.0,1.0,0.0,1.0,0.0,1.0};
    ry[0]=cos(alpha);
    ry[2]=-sin(alpha);
    ry[6]=sin(alpha);
    ry[8]=cos(alpha);
    Matrix b(ry,3,3);
    return b;
}

Matrix R_z(double alpha){
    double rz[9]={1.0,1.0,0.0,1.0,1.0,0.0,0.0,0.0,1.0};
    rz[0]=cos(alpha);
    rz[1]=sin(alpha);
    rz[3]=-sin(alpha);
    rz[4]=cos(alpha);
    Matrix c(rz,3,3);
    return c;
}

Matrix Eye(int n)
{
    Matrix eye(n,n);
    for(int i=0;i<n;i++)
    {
        eye(i,i)=1;
    }
    return eye;
}
Matrix operator==(const Matrix &M1,const Matrix &M2)
{
    if(M1.r!=M2.r||M1.c!=M2.c)
    {
        cerr<<"ERROR:Two matrix in == are different in column ans row"<<endl;
        exit(1);
    }
    Matrix M_Bool(M1.r,M2.c);
    for(int i=0;i<M1.r;i++)
        for(int j=0;j<M2.c;j++)
            {
                if(abs(M1(i,j)-M2(i,j))<eps)
                {
                    M_Bool(i,j)=1.0;
                }
            }
    return M_Bool;
}

bool IsColFullRank(const Matrix &M)                       //Orthogonal transformation doesn't change the rank of the matrix
{
    int i,j;
    // double sum;
    Matrix Q,R;
    QR_decompose(M,Q,R);
    for(i=0;i<M.r;i++)
    {   
        for(j=0;j<M.c;j++)
        {
            if(i==j&&R(i,j)<eps)
            {
                return false;
            }
            // sum+=M1[1](i,j);
        }
    }
    return true;
    //try{
    // }
    // catch(exception e)
    // {
    //     for(i=0;i<M.r;i++)
    //     {
    //         sum=0.0;
    //         for(j=0;j<M.c;j++)
    //         {
    //             sum+=M(i,j);
    //         }
    //         if(sum<eps)
    //         {
    //             break;
    //         }
    //     }
    //     return i;
}
Matrix operator|(const Matrix & M1,const Matrix &M2)
{try{
    if(M1.r!=M2.r)
    {
        // cerr<<"ERROR:The matrixes that will be emerged are different in rows"<<endl;
        throw"ERROR:The matrixes that will be emerged are different in rows";
    }
}
catch(const char* e)
{
    cerr<<e<<endl;
    exit(1);
}
    Matrix M(M1.r,M2.c+M1.c);
    for(int i=0;i<M.r;i++)
    {
        for(int j=0;j<M.c;j++)
        {
            if(j<M1.c)
            {
                M(i,j)=M1(i,j);
            }
            else
            {
                M(i,j)=M2(i,j-M1.c);
            }
        }
    }
    return M;
}

void Matrix::exchange_row(int row1,int row2)
{
    if(row1>r||row2>r)
    {
        std::cerr<<"ERROR:Row to be exchanged is out of range!"<<std::endl;
        exit(1);
    }
    double temp;
    for(int i=0;i<c;i++)
    {
        temp=M[row1][i];
        M[row1][i]=M[row2][i];
        M[row2][i]=temp;
    }
}

// int* Matrix::shape() const
// {
//     int *p=new int [2];
//     p[0]=r;
//     p[1]=c;
//     return p;
// }