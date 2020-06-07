//dijkstra shortest path algorithm for map of cities and their connections in json file format
//has to be run with -ljsoncpp flag:  g++ dijkstra.cpp -ljsoncpp
//needed jsoncpp library
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <fstream>
#include <jsoncpp/json/json.h>

using namespace std;

double Dijkstra_shortest_path(string file_name, string _start, string _finish){
    const unsigned _ininity = 999999;
    ifstream ifs(file_name);
    Json::Reader reader;
    Json::Value obj;
    reader.parse(ifs, obj);
    map<string, pair<unsigned, bool>> _map;
    const Json::Value cities = obj["cities"];
    for(const auto& _city : cities)
        _map.insert(make_pair(_city["name"].asString(), make_pair(_ininity, false)));

    _map[_start].first = 0;
    puts("\nBefore finding path");
    for(auto v : _map)
        cout<<"[name]: "<<v.first<<" - [shortest path from "<<_start<<"]: "<<v.second.first<<endl;
    vector<string> _queue;
    _queue.push_back(_start);
    for(int i=0; i<(int)_queue.size(); i++){
        unsigned _distance = _map[_queue[i]].first, _it=0;
        for(_it; _it<cities.size();_it++)
            if(cities[_it]["name"].asString()==_queue[i]) break;
        const Json::Value _city = cities[_it];
        for(const auto& connected_city : _city["connections"]){
            if(connected_city["distance"].asUInt() + _distance < _map[connected_city["name"].asString()].first)
                _map[connected_city["name"].asString()].first = connected_city["distance"].asUInt() + _distance;
            if(!_map[connected_city["name"].asString()].second)_queue.push_back(connected_city["name"].asString());
        }
        _map[_queue[i]].second = true;
    }
    puts("\nAfter finding path:");
    for(auto v : _map)
        cout<<"[name]: "<<v.first<<" - [shortest path from "<<_start<<"]: "<<v.second.first<<endl;
    return _map[_finish].first;
}

int main()
{
    //here we print all map
    ifstream ifs("romania_map.json");
    Json::Reader reader;
    Json::Value obj;
    reader.parse(ifs, obj);
    cout<< "Map title: "<<obj["title"].asString()<<endl;
    cout<<"Distance units: "<<obj["distance_unit"].asString()<<endl;
    cout<<"List of cities:"<<endl;
    const Json::Value cities = obj["cities"];
    for(const auto& _city : cities){
        cout<<_city["name"].asString()<<" - connected to:\n";
        for(const auto& _connected_city : _city["connections"])
            cout<<"\tname: "<<_connected_city["name"].asString()<<", distance: "
            <<_connected_city["distance"].asUInt()<<" "
            <<obj["distance_unit"].asString ()<<endl;
        puts("");
    }
    //here we put file name, and path from which to which city we want go
    unsigned shortest_distnace = Dijkstra_shortest_path("romania_map.json", "Arad", "Bucharest");
    cout<<"\nShortest path to Bucharest from Arad: "<<shortest_distnace<<endl;
}