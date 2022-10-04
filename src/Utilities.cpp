#include"../include/Utilities.h"
double AtanAzim(double delx,double dely)
{
    double result=0.0;
    if(delx==0)
    {
        if(dely>0) result=M_PI/2.0;
        else       result=M_PI*3/2.0;
    }
    else if(delx>0)
    {
        if(dely>=0)result=atan(dely/delx);
        else       result=2*M_PI+atan(dely/delx);
    }
    else
    {
        result=M_PI+atan(dely/delx);
    }
    return result;
}

/// @brief get system throught the prn
/// @param prn 
/// @return sys(GERC)
string GetSysByPrn(int prn)
{
    string sysP;
    if(prn<=NGPS)
    {
        sysP="G"+std::to_string(prn);
    }
    else if(prn<=NGAL)
    {
        sysP="E"+std::to_string(prn-NGPS);
    }
    else if(prn<NGLO)
    {
        sysP="R"+std::to_string(prn-NGAL);
    }
    else
    {
        sysP="C"+std::to_string(prn-NGLO);
    }
    return sysP;
}

/// @brief for binary decode :merge single obs  
/// @param data 
/// @return 
stringstream ObsFormat(const GNSSDATA& data)
{
    int i,j,nObs=0,prn=999,measCount=0;
    COMMONTIME ct;
    GNSSDATA  Sorted;
    stringstream   formatStr;
    GPST2COMMON(data.gt,ct);
    for(i=0;i<data.obsCount;i++)
    {
        if(prn!=data.satellites[i]||prn==999)
        {
            prn=data.satellites[i];
            nObs++;
            measCount=0;
        }
        Sorted.satellites[nObs-1]=prn;
        Sorted.measType[nObs-1][measCount]=data.measType[i][0];
        memcpy(Sorted.measData[nObs-1]+data.measType[i][0]*4,data.measData[i]+data.measType[i][0]*4,32);
        measCount++;
    }
    formatStr<<"> "<<to_string(ct.year)<<" "<<to_string(ct.month)<<" "
             <<to_string(ct.day)+" "<<to_string(ct.hour)+" "<<
             to_string(ct.min)+" "<<fixed<<setprecision(3)<<ct.sec<<"\n";
    for(i=0;i<nObs;i++)
    {
        formatStr<<setw(4)<<GetSysByPrn(Sorted.satellites[i]);
        for(j=0;j<5;j++)
        {
            if(Sorted.measType[i][j]!=99)
            {
                formatStr<<setw(16)<<fixed<<setprecision(3)<<Sorted.measData[i][Sorted.measType[i][j]*4]<<
                         setw(16)<<fixed<<setprecision(3)<<Sorted.measData[i][Sorted.measType[i][j]*4+1]<<
                         setw(16)<<fixed<<setprecision(3)<<Sorted.measData[i][Sorted.measType[i][j]*4+2]<<
                         setw(16)<<fixed<<setprecision(3)<<Sorted.measData[i][Sorted.measType[i][j]*4+3];
            }
            else
            {
                formatStr<<setfill(' ')<<setw(16)<<setfill(' ')<<setw(16)
                        <<setfill(' ')<<setw(16)<<setfill(' ')<<setw(16);
            }
        }
        formatStr<<"\n";
    }
    return formatStr;
}

void RangeOut(string filename,string outname)
{
    RawObs obs;
    int i=0;
    std::ofstream fileOut(outname);
    FILE *fp=fopen(filename.c_str(),"rb");
    fileOut<<"GPS"<<setw(16)<<"C1C"<<setw(16)<<"L1C"<<setw(16)<<"D1C"<<setw(16)<<"S1C"
           <<setw(16)<<"C1P"<<setw(16)<<"L1P"<<setw(16)<<"D1P"<<setw(16)<<"S1P"
           <<setw(16)<<"C2C"<<setw(16)<<"L2C"<<setw(16)<<"D2C"<<setw(16)<<"S2C"
           <<setw(16)<<"C2P"<<setw(16)<<"L2P"<<setw(16)<<"D2P"<<setw(16)<<"S2P"
           <<setw(16)<<"C1Q"<<setw(16)<<"L1Q"<<setw(16)<<"D1Q"<<setw(16)<<"S1Q\n";
    fileOut<<"GPS"<<setw(16)<<"C1C"<<setw(16)<<"L1C"<<setw(16)<<"D1C"<<setw(16)<<"S1C"
           <<setw(16)<<"C1P"<<setw(16)<<"L1P"<<setw(16)<<"D1P"<<setw(16)<<"S1P"
           <<setw(16)<<"C2C"<<setw(16)<<"L2C"<<setw(16)<<"D2C"<<setw(16)<<"S2C"
           <<setw(16)<<"C2P"<<setw(16)<<"L2P"<<setw(16)<<"D2P"<<setw(16)<<"S2P"
           <<setw(16)<<"C1Q"<<setw(16)<<"L1Q"<<setw(16)<<"D1Q"<<setw(16)<<"S1Q\n";       
    while(!feof(fp))
    {
        i++;
        if(ReadMessage(fp,obs)&&obs.obsData.obsCount!=0)
            fileOut<<ObsFormat(obs.obsData).str();
    }
    fileOut.close();
}