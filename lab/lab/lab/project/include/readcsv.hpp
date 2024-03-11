#pragma once

#include <iostream>

struct Data
{
    // int fnumber;
    std::string FlightID;
    std::string ddate;
    std::string intlOrDome;
    std::string FlightNumber;
    int dairport;
    int aairport;
    int dtime;
    int atime;
    int planeID;
    std::string planeModel;
    int fare;
    Data(){};
    Data(std::string _ddate,int _dairport,int _aairport,int _dtime,int _atime,int _planeID, std::string _planeModel, int fare):ddate(_ddate),dairport(_dairport),dtime(_dtime),atime(_atime),planeID(_planeID),planeModel(_planeModel)
    {};
    bool operator<(Data a) const
    {
        if (this->dtime!=a.dtime)
        {
            return this->dtime<a.dtime;
        }
        else
        {
            return this->aairport<a.aairport;
        }
        
    }
};
