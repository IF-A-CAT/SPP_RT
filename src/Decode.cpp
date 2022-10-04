#include"../include/Decode.h"
#include<cmath>


/// @brief              for given file fp , read and store the data of fp to rawobs
/// @param fp           file stream 
/// @param rawobs       raw obs converted from fp
/// @return             true:success  false:fail
bool ReadMessage(FILE* fp,RawObs& rawobs)
{
    memset(rawobs.buffer,0x0,sizeof(rawobs.buffer));
    memset(&(rawobs.obsData),0x0,sizeof(rawobs.obsData));
    int i,data;
    i=0;
    while(1)
    {
        if((data=fgetc(fp))==EOF)   return false;   //check is file valid  
        if(SyncOem(rawobs,uint8_t(data)))   break;  
        if(i>4096)                  return false;   //normal <=4096
        i++;
    }
    fread(rawobs.buffer+3,25,1,fp);
    rawobs.headerL=*(rawobs.buffer+3);                  //read header lenght
    rawobs.len=U2(rawobs.buffer+8);                     //len without crc and header
    rawobs.gt.week=U2(rawobs.buffer+14);                //gps week
    rawobs.gt.sec=U4(rawobs.buffer+16)*1e-3;            //gps sec
    rawobs.obsData.gt=rawobs.gt;
    fread(rawobs.buffer+28,rawobs.len+4,1,fp);          //read main data and crc
    if(rawobs.headerL!=28)
    {
        std::cerr<<"Oem7 Header Length isn't 28, please modified your code!!!\n";
        exit(1);
    }
    if(Crc32Check(rawobs.buffer,rawobs.len+rawobs.headerL)) 
        if(DecodeOme4(rawobs))
            return true;
    return false;
}


/// @brief              find the pos which message start
/// @param rawobs       obs to be synchronized
/// @param thibuffer    the thibuffer of sync 
/// @return             success(true) or failed(false) 
bool SyncOem(RawObs& rawobs,uint8_t thibuffer)
{
    rawobs.buffer[0]=rawobs.buffer[1];
    rawobs.buffer[1]=rawobs.buffer[2];
    rawobs.buffer[2]=thibuffer;
    return rawobs.buffer[0]==OEM4SYNC1&&rawobs.buffer[1]==OEM4SYNC2&&rawobs.buffer[2]==OEM4SYNC3;
}

/// @brief put buffer into stucture
/// @param rawobs 
bool DecodeOme4(RawObs& rawobs)
{
    rawobs.messID=U2(rawobs.buffer+4);
    switch (rawobs.messID)
    {
        case ID_RANGE:      DecodeRange(rawobs); break;
        case ID_EPHGPS:     DecodeGPSEph(rawobs);break; 
        case ID_EPHBDS:     DecodeBDSEph(rawobs);break;
        default:
        return false;
    }
    return true;
}

void DecodeRange(RawObs& rawobs)
{
    uint8_t* pt,*aux;
    uint32_t tracking;   
    SYS sys;
    SIGNALTYPE sign;                         
    double psr,psrSigma,adr,adrSigma,dopp,snr,lockt,mainObs[4];
    int i,iObsNum,prn,parity; 
    pt=rawobs.buffer+rawobs.headerL;
    rawobs.obsNum=iObsNum=U4(pt);
    pt=pt+4;
    for(i=0;i<iObsNum;i++,pt+=44)
    {
        aux=pt;
        tracking=U4(pt+40);
        TrackStat(tracking,sys,parity,sign);
        prn=U2(aux);             aux+=4;              //not glonass
        psr=R8(aux);             aux+=8;
        psrSigma=R4(aux);        aux+=4;
        adr=fabs(R8(aux));       aux+=8;
        adrSigma=R4(aux);        aux+=4;
        dopp=R4(aux);            aux+=4;
        snr=R4(aux);             aux+=4;
        lockt=R4(aux);           aux+=4;
        mainObs[0]=psr;mainObs[1]=adr;mainObs[2]=dopp;mainObs[3]=snr;
        rawobs.obsData.measType[rawobs.obsData.obsCount][0]=sign;
        switch(sys)
        {
            case GPS:   
                rawobs.obsData.satellites[rawobs.obsData.obsCount]=prn;
                memcpy(rawobs.obsData.measData[rawobs.obsData.obsCount]+sign*4,mainObs,32);
                break;
            case BDS:
                prn+=NGLO;
                rawobs.obsData.satellites[rawobs.obsData.obsCount]=prn;   
                memcpy(rawobs.obsData.measData[rawobs.obsData.obsCount]+sign*4,mainObs,32);
                break;
            case GLONASS:
                continue;
                prn+=NGAL;
                break;
            case GALILEO:
                continue;
                prn+=NGPS;
                rawobs.obsData.satellites[rawobs.obsData.obsCount]=prn;   
                memcpy(rawobs.obsData.measData[rawobs.obsData.obsCount]+sign*4,mainObs,32);
                break;
            default:
                continue;
                break;
        }
        rawobs.obsData.obsCount++;
    }
}

/// @brief finish crc check
/// @param buff all data including crc
/// @param len  the length with out crc
/// @return 
bool Crc32Check(uint8_t* buff,int len)
{
    uint32_t crc=0;
    int i,j;

    for (i=0;i<len;i++) {
        crc^=buff[i];
        for (j=0;j<8;j++) {
            if (crc&1) crc=(crc>>1)^POLYCRC32; else crc>>=1;
        }
    }
    if(crc==U4(buff+len))   return true;
    else                    return false;
}

void TrackStat(uint32_t stat,SYS& sys,int& parity,SIGNALTYPE& freq)
{
    int systype,signaltype;
    parity=(stat>>11)&1;
    systype=(stat>>16)&7;                   //>>(n) ,n is connected with tracking type?            
    signaltype=(stat>>21)&0x1F;
    switch (systype)
    {
        case 0: sys=GPS; break;
        case 1: sys=GLONASS; break;
        // case 2: sys=SBS; break;
        case 3: sys=GALILEO; break; /* OEM6 */
        case 4: sys=BDS; break; /* OEM6 F/W 6.400 */
        // case 5: sys=QZS; break; /* OEM6 */
        // case 6: sys=IRN; break; /* OEM7 */
        default:
                sys=OTHER; break;
    }
    if(sys==GPS)
    switch (signaltype)
    {
        case 0:  freq=C1ENTRY;break; 
        case 9:  freq=P2ENTRY;break;
        case 16: freq=P1ENTRY;break;
        case 5:  freq=P2ENTRY;break;
        case 14: freq=Q5ENTRY;break;
        case 17: freq=C2ENTRY;break;
        default:
                freq=C1ENTRY;break;
    }
    if(sys==BDS)
    {
        switch(signaltype)
        {
            case 0: freq=C1ENTRY;break;
            case 1: freq=P1ENTRY;break;
            case 2: freq=C2ENTRY;break;
            case 4: freq=C1ENTRY;break;
            case 5: freq=P1ENTRY;break;
            case 6: freq=C2ENTRY;break;
            case 7: freq=P2ENTRY;break;
            case 9: freq=Q5ENTRY;break;
            default:
                    break;
        }
    }

    //to be finished
    if(sys==GALILEO)
    {
        switch (signaltype)
        {
            case 2: freq; break;
            default:
                break;
        }
    }
    if(sys==GLONASS)
    {
        
    }
}

/// @brief decode the brdc file of gps
/// @param rawobs 
void DecodeGPSEph(RawObs& rawobs)
{
    uint8_t *p=rawobs.buffer+rawobs.headerL;
    int prn,health;
    double tow,A,delN,M0,e,omega,cuc,cus,crc,crs,cic,cis
        ,i0,iRate,Omega,OmegaRate,toc,tgd,a0,a1,a2,correctedN
        ,ura;
    GPSTIME gt;
    prn=U4(p);      p+=4;
    tow==R8(p);     p+=8;
    health=U4(p);   p+=4;
                    p+=8;       //IODE1&IODE2
    gt.week=U4(p);  p+=4;
                    p+=4;       //Z week
    gt.sec=R8(p);   p+=8;
    A=R8(p);        p+=8;
    delN=R8(p);     p+=8;
    M0=R8(p);       p+=8;
    e=R8(p);        p+=8;
    omega=R8(p);    p+=8;
    cuc=R8(p);      p+=8;
    cus=R8(p);      p+=8;
    crc=R8(p);      p+=8;
    crs=R8(p);      p+=8;
    cic=R8(p);      p+=8;
    cis=R8(p);      p+=8;
    i0=R8(p);       p+=8;
    iRate=R8(p);    p+=8;
    Omega=R8(p);    p+=8;
    OmegaRate=R8(p);p+=8;
    toc=R8(p);      p+=8;
    tgd=R8(p);      p+=8;
    a0=R8(p);       p+=8;
    a1=R8(p);       p+=8;
    a2=R8(p);       p+=8;
    correctedN=R8(p);p+=8;
    ura=R8(p);      p+=8;
    double sortedData[]={A,e,omega,Omega,i0,M0,delN,OmegaRate,iRate,cuc,cus,
                         crc,crs,cic,cis,toc,tgd,a0,a1,a2};
    if(rawobs.eph[prn-1]!=NULL)
        if(rawobs.eph[prn-1]->is_update(gt.sec,toc))
            delete   rawobs.eph[prn-1];
        else
            return; 
    rawobs.eph[prn-1]=new EPHGPS();
    rawobs.eph[prn-1]->init(prn,gt,sortedData);
}

/// @brief decode the brdc of bds
/// @param rawobs 
void DecodeBDSEph(RawObs& rawobs)
{
    uint8_t *p;
    p=rawobs.buffer+rawobs.headerL;
    int prn,health;
    double sqrtA,delN,M0,e,omega,cuc,cus,crc,crs,cic,cis
        ,i0,iRate,Omega,OmegaRate,toc,tgd1,a0,a1,a2,correctedN
        ,ura,tgd2;
    GPSTIME gt;
    prn=U4(p);      p+=4;
    gt.week==U4(p); p+=4;
    ura=R8(p);      p+=8;
    health=U4(p);   p+=4;      
    tgd1=R8(p);     p+=8;
    tgd2=R8(p);     p+=8;
                    p+=4;       //clock age
    toc=U4(p);      p+=4;
    a0=R8(p);       p+=8;
    a1=R8(p);       p+=8;
    a2=R8(p);       p+=8;
                    p+=4;       //data age
    gt.sec=U4(p);   p+=4;
    sqrtA=R8(p);    p+=8;
    e=R8(p);        p+=8;
    omega=R8(p);    p+=8;
    delN=R8(p);     p+=8;
    M0=R8(p);       p+=8;
    Omega=R8(p);    p+=8;
    OmegaRate=R8(p);p+=8;
    i0=R8(p);       p+=8;
    iRate=R8(p);    p+=8;
    cuc=R8(p);      p+=8;
    cus=R8(p);      p+=8;
    crc=R8(p);      p+=8;
    crs=R8(p);      p+=8;
    cic=R8(p);      p+=8;
    cis=R8(p);      p+=8;
    double sortedData[]={sqrtA,e,omega,Omega,i0,M0,delN,OmegaRate,iRate,cuc,cus,
                         crc,crs,cic,cis,toc,tgd1,tgd2,a0,a1,a2};
    if(rawobs.eph[prn+NGLO-1]!=NULL)
        if(rawobs.eph[prn+NGLO-1]->is_update(gt.sec,toc))
            delete   rawobs.eph[prn+NGLO-1];
        else
            return;       
    rawobs.eph[prn+NGLO-1]=new EPHBDS();
    rawobs.eph[prn+NGLO-1]->init(prn,gt,sortedData);
}

/// @brief decode the pseudo range position
/// @param rawobs 
void DecodePSRPos(RawObs& rawobs)
{
    uint8_t* p;
    p=rawobs.buffer+rawobs.headerL;
    double lat,lon,height,sigmab,sigmal,sigmah;
    p+=8;
    lat=R8(p)*M_PI/180.0;      p+=8;
    lon=R8(p)*M_PI/180.0;      p+=8;
    height=R8(p);              p+=16;
    sigmab=R4(p);              p+=4;
    sigmal=R4(p);              p+=4;
    sigmah=R4(p);              p+=4;
    rawobs.obsData.blh.B=lat;
    rawobs.obsData.blh.L=lon;
    rawobs.obsData.blh.H=height;
    rawobs.obsData.blh.sigmab=sigmab;
    rawobs.obsData.blh.sigmal=sigmal;
    rawobs.obsData.blh.sigmah=sigmah;
}