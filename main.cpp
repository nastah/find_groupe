
#include <fstream>
#include "stopFinder.cpp"
#include "dotOnMap.cpp"


#include <vector>
#include <iostream>
#include <string>
#include <sstream>


int main() {
     
    string rawJson;
    string jsonFileName = "111.csv";
    ifstream  jsonFile;
    vector<dots> input;
    string  word, str2;
    dots tempdot;

    vector<string> row; 


    jsonFile.open(jsonFileName,ios::in);
    getline(jsonFile, str2);
    if (jsonFile.is_open())
    {
        while (std::getline(jsonFile, str2))
        {
            stringstream s(str2);
            while (getline(s, word, ',')) { 

                row.push_back(word); 
            
            }
            if(row[0]!= "transpid"){
                tempdot.time = stoi(row[4]);
                tempdot.lat=stod(row[5]);
                tempdot.lon=stod(row[6]);

                input.push_back(tempdot);
            }
            row.clear();
        }
        jsonFile.close();
    }

    stopFinder cs(input);
    cs.calculate();
    rawJson =cs.getjson();
    cout<<rawJson;
    return 0;

}
