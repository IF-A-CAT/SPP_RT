# from dataclasses import replace
import os
import sys

def AddZero(doy):
    if(doy<10):
        return '00'+str(doy)
    elif(doy<100):
        return '0'+str(doy)
    else:
        return str(doy)
# def ReadRcvFile(filedir):
#     if(not os.path.exists(filedir)):
#         print("Rcvinfo does'not exist !!!")
#         exit()
#     with open(filedir,'r') as f:
#         list=f.readlines()
#     for i in range(len(list)):
#         list[i]=list[i].replace("\n","")
#         list[i]=list[i].replace(' ','')
#     return list



information=sys.argv                    #year day of year
if(len(information)!=5):
    print("\n**********************************")
    print("Usage: year fdoy edoy target_dir ")
    print("Instance: Python3 BrdcDownload.py 2021 88 99 ./brdc")
    print("Author: ZSJ From WHU")
    print("Only for GPS")
    print("**********************************\n")
    exit()
year=information[1]
sdoy=information[2]
edoy=information[3]
target_dir=information[4]
# print(target_dir[len(target_dir)-1])
if(target_dir[len(target_dir)-1]!='/'):
    target_dir+='/'
# site_list=information[5]
# ReadRcvFile(site_list)
# print(len(information))
# base_link="https://cddis.nasa.gov/archive/gnss/data/daily/"
# ftp://igs.gnsswhu.cn/pub/gps/data/daily/2022/brdc/BRDM00DLR_S_20220250000_01D_MN.rnx.gz
base_link="ftp://igs.gnsswhu.cn/pub/gps/data/daily/"
# year_link=base_link+year+'/brdc'
year_link=base_link+year+"brdc/"
year1=information[1][2:]
# Site=ReadRcvFile(site_list)
# print(information[1][2:])
# print(year_link)

out_basedir=target_dir
if(not os.path.exists(target_dir)):
    print("TargetDir doesn't exist  !!!")
    exit()
# if(not os.path.exists(target_dir)):
#         print("Target dir doesn't exist !!!!!!")
#         exit()
if(not os.path.exists(target_dir+year)):
    os.makedirs(target_dir+year)
for i in range(int(sdoy),int(edoy)+1):
    outdir=target_dir+year
    # tempdir=year+"/"+AddZero(i)+'/'+year1+'d/'
    # filepath=year_link+'/'+"brdc"+AddZero(i)+"0."+year1+"n"
    filepath=year_link+"BRDM00DLR_S_"+year+AddZero(i)+"0000_01D_MN.rnx.gz"
        # print(Site[j])
    # print(filepath)
    os.system("wget -c -N -t 10 --auth-no-challenge "+filepath+" -P "+outdir)
    os.system("gzip -f -d "+outdir+"/*.gz")
    os.system("mv "+outdir+"/BRDM00DLR_S_"+year+AddZero(i)+"0000_01D_MN.rnx " +outdir+"/brdm"+AddZero(i)+"0."+year1+"d")
    # if(os.path.exists(outdir+'/'+"brdc"+AddZero(i)+"0."+year1+"n.gz")):
    #     os.system("gzip -f -d "+outdir+"/*.gz")
        
    # else:
    #     os.system("wget -c -N -t 10 --auth-no-challenge "+filepath+".Z"+" -P "+outdir)
    #     os.system("gzip -f -d "+outdir+"/*.Z")
os.system("rm -rf "+" ./wget-log")

    # os.system("./CRX2RNX "+outdir+"/"+Site[j]+"*")
    # os.system("gzip -d "+outdir+"/*.gz")
    # os.system("./CRX2RNX "+outdir+"/*"+year1+"d")
    # os.system("rm -rf "+outdir+"/*d")



    