#include <vector>
#include <string>
#include "include/json.hpp"
#include <fstream>

#include <cmath>

#include <iostream>
#include <sstream>

// 1 km  =0.009009009

//250 0.00063063063

using namespace std;
// lat = y  
//lon = x 

struct dots
{
        double  lat;//cos (lon* M_PI/180 )
        double  lon;
        int  time;
};

class dotOnMap
{
    public:
        double  lat;//cos (lon* M_PI/180 )
        double  lon;
        int  time;

        int neighbours = 0;

        int startTime;
        int endTime;
        int dist;

        void addNeighbours(dotOnMap &target )
        {
            neighbours++;
            if (target.time > time)
            {
                endTime = target.time;
            }
            
            if (target.time < time)
            {
                startTime = target.time;
            }
        }
        dotOnMap(int stime, double slat, double slon)
        {
            time =  stime;
            lat =   slat;//cos (lon* M_PI/180 )
            lon =   slon;
            startTime =stime ;
            endTime= stime;
        }
        dotOnMap(dots inputParam )
        {
            time =  inputParam.time;
            lat =   inputParam.lat;//cos (lon* M_PI/180 )
            lon =   inputParam.lon;
            startTime =inputParam.time ;
            endTime= inputParam.time;
        }

        int getTime(){return abs(endTime - startTime);}
    private:
};




int distance (dotOnMap   ,dotOnMap );
void softFilterDot(vector <dotOnMap> &);
void hardFlterDot(dotOnMap   );
vector<int> findStops(vector <dotOnMap> );



void softFilterDot(vector <dotOnMap>  &target) 
{   

    if (target.size()<12){throw "too short";}
    int maxVelocity = 1;//250;
    vector<double> testdist;
    for (int i =0; i< 10;i++)
    {   
        testdist.push_back(distance (target[i], target[i + 1]) );
        //if (distance (target[i], target[i + 1]) > 1);
        //target.erase(target.begin()+i);
    }
    if (testdist[0] * (target[1].time - target[0].time) > maxVelocity )
    {
        target[0].time = -1;
    }
    for (int i =1; i< 10;i++)
    {   
        if (testdist[i] * (target[i+1].time - target[i].time )> maxVelocity && testdist[i-1] *( target[i].time - target[i-1].time) > maxVelocity)
        {
            target[i].time = -1;
        }
    }
    for (int i =1; i< 10;i++)
    {   
        if (target[i].time < 0){target.erase(target.begin()+i);}
    }

    for (int i =0; i < target.size() - 1  ;i++)
    {
        if (distance (target[i], target[i + 1]) * (target[i+1].time - target[i].time) > maxVelocity )
        {
            target[i+1].time = -1;
        }
    }
    int maxSize= target.size();

    while (maxSize > 0)
    {
        if (target[maxSize].time < 0){
            target.erase(target.begin()+maxSize);

            }
        maxSize--;
    }
    
}

bool isIntersect(dotOnMap aim,dotOnMap  dot)
{   
    double disTreshold=0.0008f;//0.0018;
    double csln =cos(aim.lon * (M_PI/180));
    double ltMax = aim.lat + (disTreshold * csln);

    double ltMin = aim.lat - (disTreshold * csln);
    double lnMax = aim.lon + disTreshold;
    double lnMin =  aim.lon - disTreshold;

    
    if (ltMin < dot.lat && lnMin < dot.lon && ltMax > dot.lat && lnMax > dot.lon )
    {

        return true;
        
    }
    else{return false;}
}

vector<int> findStops(vector <dotOnMap> target )
{
    int j;
    for (int i = 2; i < target.size() -2 ;i++)
    {

        j = 1; 
     

        while (i -j > 1 && j + i + 1 < target.size()  && ((isIntersect(target[i], target[i-j] ) || isIntersect(target[i], target[i+j]) || isIntersect(target[i], target[i- j -1])) || isIntersect(target[i], target[i+j +1]) ))
        {   


            if(isIntersect(target[i], target[i-j]))
            {
                target[i].addNeighbours(target[i-j]);
            }
            if(isIntersect(target[i], target[i+j]))
            {
                target[i].addNeighbours(target[i+j]);
            }
            j++;

        }
        

    }
    vector <int> stops;
    int timeOfStop =10*60 ;
    for (int i =0; i < target.size()  ;i++)
    {
        if (target[i].getTime() < timeOfStop)
        {
            stops.push_back(i);
        }
    }
    int quantDot= 0;
    while (quantDot < stops.size() -1 )
    {
        if (target[quantDot].endTime > target[quantDot+1].startTime) 
        {
            
            if (target[stops[quantDot]].neighbours < target[stops[quantDot+1]].neighbours) 
            {
                stops.erase(stops.begin()+quantDot);
            }
            else { stops.erase(stops.begin()+quantDot +1);}
        }
        else{quantDot++;}
        
    }
    return stops;
}

/*double distance (dotOnMap first  ,dotOnMap second)
{
    double ndist = abs(first.lon - second.lon);
    double cos_f = cos(first.lon * M_PI/180);
    double cos_s = cos( second.lon * M_PI/180);
    double tdist = abs((first.lat*cos_f) - (second.lon *cos_s));
    double dist = sqrt(ndist*ndist + tdist*tdist );
    return dist;
}

*/
int distance (dotOnMap first  ,dotOnMap second)
{
    double Lat1 = first.lat;
    double Lat2 = second.lat;
    double Lon1 = first.lon;
    double Lon2 =second.lon;
    Lat1 - Lat1 * M_PI/180; Lat2 = Lat2 * M_PI/180;
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
    double distres = ad * 6372795;
    return distres;
}


class stopFinder
{
private:
    vector<dotOnMap> arrayToOut;
public:
    stopFinder( vector<dots>);
    stopFinder( vector<dotOnMap> i);
    string getjson();
};
stopFinder::stopFinder( vector<dots> inputArray)
{
    for (auto &inputDot :inputArray)
    {
        arrayToOut.push_back(dotOnMap(inputDot));
    }
}
stopFinder::stopFinder( vector<dotOnMap> inputArray)
{
        arrayToOut= inputArray;

}

string stopFinder::getjson()
{ 
    vector <int> arrayOfStops;
    softFilterDot(arrayToOut);
    arrayOfStops = findStops(arrayToOut);

    nlohmann::json jstoout;
    nlohmann::json j;

    int ell;

    for (int i = 0; i< arrayOfStops.size(); i++)
    {
        ell =arrayOfStops[i];
        j["state"]="ARRIVAL";
        j["t1"]=arrayToOut[ell].startTime;
        j["t2"]=arrayToOut[ell].endTime;
        j["lat"]=arrayToOut[ell].lat;
        j["lon"]=arrayToOut[ell].lon;
        j["dist"]=distance(arrayToOut[ell],arrayToOut[ell+1]);
        j["tm"]=arrayToOut[ell].getTime();
        j["id"]=1;
        jstoout.push_back(j);
        j.clear();
    }
    return jstoout.dump();
    

}




int main()
{

    string rawJson;

    cout<<"adsds";

    string jsonFileName = "111.csv";
    ifstream  jsonFile;


    jsonFile.open(jsonFileName,ios::in);
    vector<dots> input;
    
    string line, word, str2;
    stringstream str;
    dots tempdot;
    getline(jsonFile, line);
    vector<string> row; 
    int iters= 0;

    if (jsonFile.is_open())
    {
        while (std::getline(jsonFile, str2))
        {
            iters++;
            stringstream s(str2);
            while (getline(s, word, ',')) { 
  
                // add all the column data 
                // of a row to a vector 

                row.push_back(word); 
            
            }
            if(row[0]!= "transpid"){
                tempdot.time = stoi(row[4]);
                tempdot.lat=stod(row[5]);
                tempdot.lon=stod(row[6]);
                //cout<<"//"<<to_string(tempdot.lon  )<<"//";
                input.push_back(tempdot);
            }
            row.clear();
        }
        jsonFile.close();
        // convert string to integer for comparision 

    }
    
    cout<<"sada";
    cout<<"/a/"<<(input.size())<<"/a/";
    stopFinder cs(input);
    rawJson =cs.getjson();
    cout<<rawJson;
    return 0;
}