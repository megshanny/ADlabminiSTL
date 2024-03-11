#include <miniSTL/stl.hpp>
#include <string>
#include <tuple>

#define DATATITLE "Flight ID,Departure date,Intl/Dome,Flight NO.,Departure airport,Arrival airport,Departure Time,Arrival Time,Airplane ID,Airplane Model,Air fares"

struct Date {
    int year;
    int month;
    int day;
};

struct Time {
    Date date;
    int hour;
    int minute;
    long long int time;
};

struct Flight {
    // Flight ID,Departure date,Intl/Dome,Flight NO.,Departure airport,Arrival airport,Departure Time,Arrival Time,Airplane ID,Airplane Model,Air fares
    int flight_id;
    Date departure_date;
    bool is_intl;
    int flight_no;
    int departure_airport;
    int arrival_airport;
    Time departure_time;
    Time arrival_time;
    int airplane_id;
    int airplane_model;
    int air_fares;
};

struct Path {
    Vector<Flight> flights;
};

struct DFSResult {
    Vector<int> airports;
    int airport_id;
    Time start_time;
};

struct BFSResult {
    Vector<int> airports;
    int airport_id;
    Time start_time;
};

struct ConnectivityResult {
    Vector<Path> paths;
    int airport_1;
    int airport_2;
    bool is_connected;
};

struct ShortestPathResult {
    Vector<Path> paths;
    int airport_1;
    int airport_2;
    Time start_time;
    Time end_time;
};

struct MinimumCostPathResult {
    Vector<Path> paths;
    int airport_1;
    int airport_2;
    Time start_time;
    Time end_time;
    int total_cost;
};

struct AllPathsResult {
    Vector<Path> paths;
    int airport_1;
    int airport_2;
    Time start_time;
    Time end_time;
};

Date parse_date(std::string date);
Time parse_time(std::string time);
Flight parse_flight(std::string line);
std::string to_string(Date date);
std::string to_string(Time time);
std::string to_string(Flight flight);

// 使用模板函数，将dfs、bfs的print函数合并为一个函数
template <typename T>
void print_airport_result(T result)
{
    if (result.airports.empty()) {
        std::cout << "No airport found." << std::endl;
        return;
    }
    for (int i = 0; i < result.airports.size(); i++) {
        std::cout << result.airports[i] << ' ';
    }
    std::cout << std::endl;
}

template <typename T>
void print_path_result(T result)
{
    if (result.paths.empty()) {
        std::cout << "No path found." << std::endl;
        return;
    }
    for (int i = 0; i < result.paths.size(); i++) {
        std::cout << std::endl;
        std::cout << "Path " << i + 1 << ":" << std::endl;
        std::cout << DATATITLE << std::endl;
        for (int j = 0; j < result.paths[i].flights.size(); j++) {
            std::cout << to_string(result.paths[i].flights[j]) << std::endl;
        }
    }
}

struct Planner {
    int airport_num = 0, flight_num = 0;
    Planner(int max_airport_num);
    Vector<Flight> flights;
    Vector<int> nextflight;
    Vector<int> head;
    void set_max_airport_num(int num);
    int load(std::string filename);
    void sort();
    void printflight(int flight_id);
    void printairport(int airport_id);
    DFSResult query_dfs(int airport_id, std::string start_time);
    BFSResult query_bfs(int airport_id, std::string start_time);
    ConnectivityResult query_connectivity(int airport_1, int airport_2);
    ShortestPathResult query_shortest_path(int airport_1, int airport_2, std::string start_time, std::string end_time);
    MinimumCostPathResult query_minimum_cost_path(int airport_1, int airport_2, std::string start_time, std::string end_time);
    AllPathsResult query_all_paths(int airport_1, int airport_2, std::string start_time, std::string end_time);
};
