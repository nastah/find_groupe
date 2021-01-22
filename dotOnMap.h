//
// Created by oplawin on 22.01.21.
//

#ifndef UNTITLED_DOTONMAP_H
#define UNTITLED_DOTONMAP_H

#include <cmath>
#include <string>
#include "json.hpp"
#include <iostream>

using namespace std;

const double maxVelocity = 70;//250;


struct dots
{
    double  lat;//cos (lon* M_PI/180 )
    double  lon;
    int  time;
};

struct Window{
    int startTime;
    int endTime;

    int getTime(){return abs(startTime-endTime);};

};

class dotOnMap {
private:
    double  lat;//cos (lon* M_PI/180 )
    double  lon;
    int  time;

    int neighbours = 0;

    Window window;

    int dist;

    bool active = true;
public:
    dotOnMap(dots inputParam );
    dotOnMap(int stime, double slat, double slon);

    void checkDistance(dotOnMap d);

    double distance (dotOnMap d);
    void addNeighbours(dotOnMap target );
    int getPeriod();
    int getTime();
    void deactivate();
    nlohmann::json getJson();
    bool isIntersect(dotOnMap  dot);

    int getNeighbours();
    int getstartTime();
    int getendTime();
};


#endif //UNTITLED_DOTONMAP_H
