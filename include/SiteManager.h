/************************
 * for site data process
 * modified 2022 10 4
 * *********************/
#pragma once
#include"TimeCoor.h"
#include"SrifPro.h"
#include"Decode.h"
#include"Utilities.h"

class SiteManager
{
    public:
    SiteManager();
    void design_mat();
    void pos_process();
    void srif_init();
    private:
    XYZ                     _xyz;
    Srif                   _srif;
    RawObs                  _raw;
};