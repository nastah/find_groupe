//
// Created by oplawin on 22.01.21.
//

#ifndef UNTITLED_STOPFINDER_H
#define UNTITLED_STOPFINDER_H

#include "dotOnMap.h"
#include <vector>

using namespace std;

const int timeOfStop =10*60*10 ;

class stopFinder
{
private:
    vector<dotOnMap> arrayToOut;
    vector <int> arrayOfStops;

    void softFilterDot(vector <dotOnMap> &target);
    void hardFlterDot(dotOnMap   &target);
    void countNeighbours();//
    vector<int> findStops();//(vector <dotOnMap>  &target);

    void filteroutStops(vector<int> &stopsIndex);

    void claerNonctivate(vector<dotOnMap> &target);
public:
    stopFinder( vector<dots> inputArray);
    stopFinder( vector<dotOnMap> inputArray);
    void calculate();
    string getjson();
};


#endif //UNTITLED_STOPFINDER_H
