#include"../include/SiteManager.h"
#include<cmath>
/// @brief init
SiteManager::SiteManager()
{
    _xyz.x=_xyz.y=_xyz.z=0.0;
}


/// @brief get the pos of the Site
void SiteManager::pos_process()
{
    
}

/// @brief update design mat
void SiteManager::design_mat()
{
    int i;
    double coef[4],satXYZ[3],S0,l,omc;                             //x,y,z,clk bias  the correction
    coef[3]=-1.0;
    for(i=0;i<_raw.obsData.obsCount;i++)
    {
        _raw.eph[_raw.obsData.satellites[i]-1]->rectangular_pos(GPST2BDST(_raw.gt),satXYZ);
        S0=sqrt(pow((satXYZ[0]-_xyz.x),2)+pow((satXYZ[1]-_xyz.y),2)+pow((satXYZ[2]-_xyz.z),2));
        coef[0]=(_xyz.x-satXYZ[0])/S0;coef[1]=(_xyz.y-satXYZ[1])/S0;coef[2]=(_xyz.z-satXYZ[2])/S0;
        omc=_raw.obsData.measData[i][4*_raw.obsData.measType[i][0]]-S0;
        _srif.meas_update(coef,omc,1);
    }
}


void SiteManager::srif_init()
{
    int i,type[4];
    double coef[4],satXYZ[3],S0;                             //x,y,z,clk bias  the correction
    Matrix design_mat(_raw.obsData.obsCount,4),omc(_raw.obsData.obsCount,1);
    coef[3]=-1.0;
    type[0]=type[2]=type[1]=0;
    type[3]=1;
    for(i=0;i<_raw.obsData.obsCount;i++)
    {
        _raw.eph[_raw.obsData.satellites[i]-1]->rectangular_pos(GPST2BDST(_raw.gt),satXYZ);
        S0=sqrt(pow((satXYZ[0]-_xyz.x),2)+pow((satXYZ[1]-_xyz.y),2)+pow((satXYZ[2]-_xyz.z),2));
        coef[0]=(_xyz.x-satXYZ[0])/S0;coef[1]=(_xyz.y-satXYZ[1])/S0;coef[2]=(_xyz.z-satXYZ[2])/S0;
        omc(i+1,1)=_raw.obsData.measData[i][4*_raw.obsData.measType[i][0]]-S0;
        design_mat(i+1,1)=coef[0];design_mat(i+1,2)=coef[1];design_mat(i+1,3)=coef[2];design_mat(i+1,4)=coef[3];
    }
    _srif.init(design_mat,omc,Eye(_raw.obsData.obsCount),type);
}
