//
// Created by oplawin on 22.01.21.
//

#include "dotOnMap.h"

void dotOnMap::addNeighbours(dotOnMap target)     {
    neighbours++;
    if (target.getTime() > this->time)
    {
        window.endTime = target.getTime();
    }

    if (target.getTime() < this->time)
    {
        window.startTime = target.getTime();
    }
}

dotOnMap::dotOnMap(int stime, double slat, double slon) {
    time =  stime;
    lat =   slat;//cos (lon* M_PI/180 )
    lon =   slon;
    window.startTime =stime ;
    window.endTime= stime;
}

dotOnMap::dotOnMap(dots inputParam) {
    time =  inputParam.time;
    lat =   inputParam.lat;//cos (lon* M_PI/180 )
    lon =   inputParam.lon;
    window.startTime = inputParam.time ;
    window.endTime= inputParam.time;
}

int dotOnMap::getTime() {
    return time;
}

nlohmann::json dotOnMap::getJson() {
    nlohmann::json j;
    cout<<"/v/"<< this->neighbours;
    j["state"]="ARRIVAL";
    j["t1"] = this->window.startTime;
    j["t2"] = this->window.endTime;
    j["lat"]= this->lat;
    j["lon"]= this->lon;
    //j["dist"]= distance(arrayToOut[ell],arrayToOut[ell+1]);
    j["tm"]= this->getPeriod();
    j["id"]= 1;
    return j;
}

int dotOnMap::getPeriod() {
    return window.getTime();
}

void dotOnMap::deactivate() {
    active = false;
}

double dotOnMap::distance(dotOnMap d) {
    double Lat1 = this->lat;
    double Lat2 = d.lat;
    double Lon1 = this->lon;
    double Lon2 = d.lon;
    Lat1 = Lat1 * M_PI/180; Lat2 = Lat2 * M_PI/180;
    Lon1 = Lon1 * M_PI/180; Lon2 = Lon2 * M_PI/180;
    double coslat1 = cos(Lat1);
    double coslat2 = cos (Lat2);
    double sinlat1 = sin(Lat1);
    double sinlat2 = sin(Lat2);
    double delta = Lon1 - Lon2;
    double cosDelta = cos (delta);
    double sinDelta = sin(delta);
    double y = sqrt( pow( coslat2 * sinDelta, 2) + pow(coslat1 * sinlat2 - sinlat1 * coslat2 * cosDelta, 2));
    double x = sinlat1 * sinlat2 + coslat1 * coslat2 * cosDelta;
    double ad = atan2(y,x);

    return ad * 6372795;
}

bool dotOnMap::isIntersect(dotOnMap  dot)
{
    if ( this->distance(dot) < 100)
    {
        return true;
    }
    else{return false;}
}

void dotOnMap::checkDistance(dotOnMap d) {
    if (distance(d) / (double)abs(this->time - d.time) > maxVelocity){
        deactivate();
    }
}

int dotOnMap::getNeighbours(){
    return this->neighbours;
}

    int dotOnMap::getstartTime(){return window.startTime;};
    int dotOnMap::getendTime(){return window.endTime;};