//
// Created by oplawin on 22.01.21.
//

#include "stopFinder.h"


void stopFinder::calculate()
{
    countNeighbours();
    arrayOfStops =findStops();
    filteroutStops(arrayOfStops);

}
string stopFinder::getjson() 
{
    nlohmann::json jstoout;
    for (int i = 0; i< arrayOfStops.size(); i++)
    {
        jstoout.push_back(arrayToOut[arrayOfStops[i]].getJson());
    }
    return jstoout.dump();

    
}

stopFinder::stopFinder(vector<dots> inputArray) {
    for (auto &inputDot :inputArray)
    {
        arrayToOut.push_back(dotOnMap(inputDot));
    }
}

stopFinder::stopFinder(vector<dotOnMap> inputArray) {
    arrayToOut = inputArray;
}

void stopFinder::softFilterDot(vector<dotOnMap> &target) {
    if (target.size()<12){
        throw "too short";
    }
    target[0].checkDistance(target[1]);
    for (int i =1; i< 10;i++)
    {
        target[i].checkDistance(target[i-1]);
        target[i].checkDistance(target[i+1]);
    }
    claerNonctivate(target);
    for (int i =target.size() - 1 ; i > 0  ;i--)
    {
        target[i].checkDistance(target[i+1]);
    }
    claerNonctivate(target);
}


void stopFinder::countNeighbours() {
    int j;
    for (int i = 2; i < arrayToOut.size() -2 ;i++)
    {

        j = 1;

        while (i -j > 1 && j + i + 1 < arrayToOut.size() && ( arrayToOut[i].isIntersect(arrayToOut[i-j]) || arrayToOut[i].isIntersect(arrayToOut[i+j]) || arrayToOut[i].isIntersect(arrayToOut[i- j -1]) || arrayToOut[i].isIntersect(arrayToOut[i+j+1])))
        {

            if(arrayToOut[i].isIntersect(arrayToOut[i-j]))
            {
                arrayToOut[i].addNeighbours(arrayToOut[i-j]);
            }
            if(arrayToOut[i].isIntersect(arrayToOut[i+j]))
            {
                arrayToOut[i].addNeighbours(arrayToOut[i+j]);

            }
            j++;

        }
    }
}

vector<int> stopFinder::findStops(){
    vector <int> stopsIndex;

    for (int i =0; i < arrayToOut.size()  ;i++)
    {
        if (arrayToOut[i].getPeriod()> timeOfStop)
        {
            //cout<<arrayToOut[i].getJson();
            stopsIndex.push_back(i);
        }
    }
    
    return stopsIndex;

}

void stopFinder::filteroutStops(vector<int> &stopsIndex){
    int quantDot= 0;

    while (quantDot < stopsIndex.size() -1 )
    {

        if (arrayToOut[quantDot].getendTime() >= arrayToOut[quantDot+1].getstartTime())
        {

            if (arrayToOut[stopsIndex[quantDot]].getNeighbours() < arrayToOut[stopsIndex[quantDot+1]].getNeighbours())
            {
                stopsIndex.erase(stopsIndex.begin()+quantDot);
            }
            else { stopsIndex.erase(stopsIndex.begin()+quantDot+1 );}

        }
        else{quantDot++;

        }
    }

    quantDot= 0;


}


void stopFinder::claerNonctivate(vector<dotOnMap> &target) {
    int maxSize= target.size();
    while (maxSize > 0)
    {
        if (target[maxSize].getTime() < 0){

            target.erase(target.begin()+maxSize);
        }
        maxSize--;
    }
}
