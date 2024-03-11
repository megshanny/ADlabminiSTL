#include <algorithm>
#include <exception>
#include <fstream>
#include <iostream>
#include <planner.hpp>
#include <queue>
#include <time.h>

Date parse_date(std::string date)
{
    Date d;
    std::string::size_type pos = 0, prev = 0;
    pos = date.find('/', prev);
    d.month = std::stoi(date.substr(prev, pos - prev));
    prev = pos + 1;
    pos = date.find('/', prev);
    d.day = std::stoi(date.substr(prev, pos - prev));
    prev = pos + 1;
    d.year = std::stoi(date.substr(prev));
    return d;
}

Time parse_time(std::string time)
{
    Time t;
    std::string::size_type pos = 0, prev = 0;
    pos = time.find(' ', prev);
    t.date = parse_date(time.substr(prev, pos - prev));
    prev = pos + 1;
    pos = time.find(':', prev);
    t.hour = std::stoi(time.substr(prev, pos - prev));
    prev = pos + 1;
    t.minute = std::stoi(time.substr(prev));
    tm stm;
    stm.tm_year = t.date.year - 1900;
    stm.tm_mon = t.date.month - 1;
    stm.tm_mday = t.date.day;
    stm.tm_hour = t.hour;
    stm.tm_min = t.minute;
    stm.tm_sec = 0;
    t.time = mktime(&stm);
    return t;
}

Flight parse_flight(std::string line)
{
    Flight flight;
    std::string::size_type pos = 0, prev = 0;
    pos = line.find(',', prev);
    flight.flight_id = std::stoi(line.substr(prev, pos - prev));
    prev = pos + 1;
    pos = line.find(',', prev);
    flight.departure_date = parse_date(line.substr(prev, pos - prev));
    prev = pos + 1;
    pos = line.find(',', prev);
    flight.is_intl = line.substr(prev, pos - prev) == "Intl";
    prev = pos + 1;
    pos = line.find(',', prev);
    flight.flight_no = std::stoi(line.substr(prev, pos - prev));
    prev = pos + 1;
    pos = line.find(',', prev);
    flight.departure_airport = std::stoi(line.substr(prev, pos - prev));
    prev = pos + 1;
    pos = line.find(',', prev);
    flight.arrival_airport = std::stoi(line.substr(prev, pos - prev));
    prev = pos + 1;
    pos = line.find(',', prev);
    flight.departure_time = parse_time(line.substr(prev, pos - prev));
    prev = pos + 1;
    pos = line.find(',', prev);
    flight.arrival_time = parse_time(line.substr(prev, pos - prev));
    prev = pos + 1;
    pos = line.find(',', prev);
    flight.airplane_id = std::stoi(line.substr(prev, pos - prev));
    prev = pos + 1;
    pos = line.find(',', prev);
    flight.airplane_model = std::stoi(line.substr(prev, pos - prev));
    prev = pos + 1;
    flight.air_fares = std::stoi(line.substr(prev));
    return flight;
}

std::string to_string(Date date)
{
    return std::to_string(date.month) + "/" + std::to_string(date.day) + "/" + std::to_string(date.year);
}

std::string to_string(Time time)
{
    std::string strm = "";
    if (time.minute < 10)
        strm += '0';
    strm += std::to_string(time.minute);
    return to_string(time.date) + " " + std::to_string(time.hour) + ":" + strm;
}

std::string to_string(Flight flight)
{
    return std::to_string(flight.flight_id) + "," + to_string(flight.departure_date) + "," + (flight.is_intl ? "Intl" : "Dome") + "," + std::to_string(flight.flight_no) + "," + std::to_string(flight.departure_airport) + "," + std::to_string(flight.arrival_airport) + "," + to_string(flight.departure_time) + "," + to_string(flight.arrival_time) + "," + std::to_string(flight.airplane_id) + "," + std::to_string(flight.airplane_model) + "," + std::to_string(flight.air_fares);
}

Planner::Planner(int max_airport_num)
{
    head.resize(max_airport_num + 1);
    Flight flight;
    flights.push_back(flight);
    nextflight.push_back(0);
    for (int i = 0; i < head.size(); i++) {
        head[i] = 0;
    }
}
void Planner::set_max_airport_num(int num)
{
    airport_num = num;
    head.resize(num + 1);
}

int Planner::load(std::string filename)
{
    int max_airport_id = 0;
    std::cout << "Loading data from " << filename << std::endl;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Error: could not open file " << filename << std::endl;
        return 1;
    }
    std::string line;
    std::getline(file, line);
    try {
        while (std::getline(file, line)) {
            Flight flight = parse_flight(line);
            if (flight.departure_airport > airport_num) {
                set_max_airport_num(flight.departure_airport * 2);
            }
            flights.push_back(flight);
            nextflight.push_back(head[flight.departure_airport]);
            flight_num++;
            head[flight.departure_airport] = flight_num;
            if (flight.departure_airport > max_airport_id)
                max_airport_id = flight.departure_airport;
            if (flight.arrival_airport > max_airport_id)
                max_airport_id = flight.arrival_airport;
        }
    } catch (std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
        std::cout << "Analysis error, data loading failed." << std::endl;
        return 1;
    }
    airport_num = max_airport_id;
    set_max_airport_num(max_airport_id);
    std::cout << "Data loaded." << std::endl;
    return 0;
}

void Planner::sort()
{
    std::cout << "Sorting data..." << std::endl;
    for (int i = 1; i <= airport_num; i++) {
        Vector<int> flight_list;
        for (int j = head[i]; j; j = nextflight[j]) {
            flight_list.push_back(j);
        }
        if (flight_list.empty())
            continue;
        // 先按出发时间，再按目标机场排序
        std::sort(flight_list.begin(), flight_list.end(), [&](int a, int b) {
            return flights[a].departure_time.time < flights[b].departure_time.time
                || (flights[a].departure_time.time == flights[b].departure_time.time
                    && flights[a].arrival_airport < flights[b].arrival_airport);
        });
        head[i] = flight_list[0];
        for (int j = 1; j < flight_list.size(); j++) {
            nextflight[flight_list[j - 1]] = flight_list[j];
        }
        nextflight[flight_list.back()] = 0;
    }
    std::cout << "Data sorted." << std::endl;
}

void Planner::printflight(int flight_id)
{
    if (flight_num < flight_id) {
        std::cout << "Error: flight " << flight_id << " does not exist." << std::endl;
        return;
    }
    std::cout << to_string(flights[flight_id]) << std::endl;
}

void Planner::printairport(int airport_id)
{
    if (airport_num < airport_id) {
        std::cout << "Error: airport " << airport_id << " does not exist." << std::endl;
        return;
    }
    if (head[airport_id] == 0) {
        std::cout << "No flight from airport " << airport_id;
        return;
    }
    std::cout << DATATITLE << std::endl;
    for (int i = head[airport_id]; i; i = nextflight[i]) {
        printflight(i);
    }
}

DFSResult Planner::query_dfs(int airport_id, std::string start_time)
{
    DFSResult result;
    result.airport_id = airport_id;
    result.start_time = parse_time(start_time);
    if (airport_num < airport_id) {
        std::cout << "Error: airport " << airport_id << " does not exist." << std::endl;
        return result;
    }
    Vector<int> path;
    Vector<bool> visited(flight_num + 1, false);
    Vector<long long int> arrival_time(flight_num + 1);
    path.push_back(airport_id);
    visited[airport_id] = true;
    arrival_time[airport_id] = result.start_time.time;
    result.airports.push_back(airport_id);
    // 在搜索的过程中，可能会出现某个机场第一次被访问时时间并非最早的情况，导致后续机场无法被访问
    // 因为限制每个机场只访问一次，所以在这种情况下不做处理，放弃对后续机场的访问即可
    while (!path.empty()) {
        bool flag = false;
        for (int i = head[path.back()]; i; i = nextflight[i]) {
            if (!visited[flights[i].arrival_airport] && flights[i].departure_time.time >= arrival_time[path.back()]) {
                path.push_back(flights[i].arrival_airport);
                visited[flights[i].arrival_airport] = true;
                arrival_time[flights[i].arrival_airport] = flights[i].arrival_time.time;
                result.airports.push_back(flights[i].arrival_airport);
                flag = true;
                break;
            }
        }
        if (!flag) {
            path.pop_back();
        }
    }
    return result;
}

BFSResult Planner::query_bfs(int airport_id, std::string start_time)
{
    BFSResult result;
    result.airport_id = airport_id;
    result.start_time = parse_time(start_time);
    if (airport_num < airport_id) {
        std::cout << "Error: airport " << airport_id << " does not exist." << std::endl;
        return result;
    }
    Vector<int> list;
    int list_begin = 0;
    Vector<bool> visited(flight_num + 1, false);
    Vector<long long int> arrival_time(flight_num + 1);
    list.push_back(airport_id);
    visited[airport_id] = true;
    arrival_time[airport_id] = result.start_time.time;
    result.airports.push_back(airport_id);
    while (list_begin < list.size()) {
        for (int i = head[list[list_begin]]; i; i = nextflight[i]) {
            if (!visited[flights[i].arrival_airport] && flights[i].departure_time.time >= arrival_time[list[list_begin]]) {
                list.push_back(flights[i].arrival_airport);
                visited[flights[i].arrival_airport] = true;
                arrival_time[flights[i].arrival_airport] = flights[i].arrival_time.time;
                result.airports.push_back(flights[i].arrival_airport);
            }
        }
        list_begin++;
    }
    return result;
}

ConnectivityResult Planner::query_connectivity(int airport_1, int airport_2)
{
    ConnectivityResult result;
    result.airport_1 = airport_1;
    result.airport_2 = airport_2;
    if (airport_1 == airport_2) {
        std::cout << "Error: same source and destination." << std::endl;
        return result;
    }
    if (airport_num < airport_1) {
        std::cout << "Error: airport " << airport_1 << " does not exist." << std::endl;
        return result;
    }
    if (airport_num < airport_2) {
        std::cout << "Error: airport " << airport_2 << " does not exist." << std::endl;
        return result;
    }
    result.is_connected = false;
    for (int i = head[airport_1]; i; i = nextflight[i]) {
        if (flights[i].arrival_airport == airport_2) {
            Path path;
            path.flights.push_back(flights[i]);
            result.paths.push_back(path);
            result.is_connected = true;
            return result;
        }
    }
    for (int i = head[airport_1]; i; i = nextflight[i]) {
        for (int j = head[flights[i].arrival_airport]; j; j = nextflight[j]) {
            if (flights[j].arrival_airport == airport_2 && flights[i].arrival_time.time <= flights[j].departure_time.time) {
                Path path;
                path.flights.push_back(flights[i]);
                path.flights.push_back(flights[j]);
                result.paths.push_back(path);
                result.is_connected = true;
                return result;
            }
        }
    }
    return result;
}

struct cmp_by_time {
    bool operator()(Flight a, Flight b)
    {
        return a.arrival_time.time > b.arrival_time.time;
    }
};

ShortestPathResult Planner::query_shortest_path(int airport_1, int airport_2, std::string start_time, std::string end_time)
{
    ShortestPathResult result;
    result.airport_1 = airport_1;
    result.airport_2 = airport_2;
    result.start_time = parse_time(start_time);
    result.end_time = parse_time(end_time);
    if (airport_1 == airport_2) {
        std::cout << "Error: same source and destination." << std::endl;
        return result;
    }
    if (airport_num < airport_1) {
        std::cout << "Error: airport " << airport_1 << " does not exist." << std::endl;
        return result;
    }
    if (airport_num < airport_2) {
        std::cout << "Error: airport " << airport_2 << " does not exist." << std::endl;
        return result;
    }
    if (start_time > end_time) {
        std::cout << "Error: start time " << start_time << " is later than end time " << end_time << std::endl;
        return result;
    }
    std::priority_queue<Flight, Vector<Flight>, cmp_by_time> q;
    Vector<long long int> arrival_time(flight_num + 1, 0x7fffffffffffffff);
    Vector<int> from(flight_num + 1, 0);
    Vector<bool> visited(flight_num + 1, false);
    visited[airport_1] = true;
    arrival_time[airport_1] = result.start_time.time;
    for (int i = head[airport_1]; i; i = nextflight[i]) {
        if (flights[i].departure_time.time >= arrival_time[airport_1]
            && flights[i].arrival_time.time <= result.end_time.time) {
            q.push(flights[i]);
        }
    }
    while (!q.empty()) {
        Flight flight = q.top();
        q.pop();
        if (visited[flight.arrival_airport])
            continue;
        visited[flight.arrival_airport] = true;
        arrival_time[flight.arrival_airport] = flight.arrival_time.time;
        from[flight.arrival_airport] = flight.flight_id;
        if (flight.arrival_airport == airport_2) {
            Path path;
            int airport = airport_2;
            while (airport != airport_1) {
                path.flights.push_back(flights[from[airport]]);
                airport = flights[from[airport]].departure_airport;
            }
            std::reverse(path.flights.begin(), path.flights.end());
            result.paths.push_back(path);
            return result;
        }
        for (int i = head[flight.arrival_airport]; i; i = nextflight[i]) {
            if (flights[i].departure_time.time >= arrival_time[flight.arrival_airport]
                && flights[i].arrival_time.time <= result.end_time.time) {
                q.push(flights[i]);
            }
        }
    }
    return result;
}

struct cmp_by_cost {
    bool operator()(std::pair<Flight, int> a, std::pair<Flight, int> b)
    {
        return a.second > b.second
            || (a.second == b.second
                && a.first.arrival_time.time > b.first.arrival_time.time);
    }
};

MinimumCostPathResult Planner::query_minimum_cost_path(int airport_1, int airport_2, std::string start_time, std::string end_time)
{
    MinimumCostPathResult result;
    result.airport_1 = airport_1;
    result.airport_2 = airport_2;
    result.start_time = parse_time(start_time);
    result.end_time = parse_time(end_time);
    if (airport_1 == airport_2) {
        std::cout << "Error: same source and destination." << std::endl;
        return result;
    }
    if (airport_num < airport_1) {
        std::cout << "Error: airport " << airport_1 << " does not exist." << std::endl;
        return result;
    }
    if (airport_num < airport_2) {
        std::cout << "Error: airport " << airport_2 << " does not exist." << std::endl;
        return result;
    }
    if (start_time > end_time) {
        std::cout << "Error: start time " << start_time << " is later than end time " << end_time << std::endl;
        return result;
    }
    std::priority_queue<std::pair<Flight, int>, Vector<std::pair<Flight, int>>, cmp_by_cost> q;
    Vector<long long int> arrival_time(flight_num + 1, 0x7fffffffffffffff);
    Vector<Vector<int>> actived_flight(flight_num + 1);
    arrival_time[airport_1] = result.start_time.time;
    for (int i = head[airport_1]; i; i = nextflight[i]) {
        if (flights[i].departure_time.time >= arrival_time[airport_1]
            && flights[i].arrival_time.time <= result.end_time.time) {
            q.push({ flights[i], flights[i].air_fares });
        }
    }
    while (!q.empty()) {
        auto data = q.top();
        q.pop();
        Flight flight = data.first;
        int cost = data.second;
        if (flight.arrival_time.time >= arrival_time[flight.arrival_airport])
            continue;
        long long int last_arrival_time = arrival_time[flight.arrival_airport];
        arrival_time[flight.arrival_airport] = flight.arrival_time.time;
        actived_flight[flight.arrival_airport].push_back(flight.flight_id);
        if (flight.arrival_airport == airport_2) {
            result.total_cost = cost;
            Path path;
            int airport = airport_2;
            long long int arrival_time = flight.arrival_time.time;
            while (airport != airport_1) {
                for (int i = 0; i < actived_flight[airport].size(); i++) {
                    if (flights[actived_flight[airport][i]].arrival_time.time <= arrival_time) {
                        Flight flight = flights[actived_flight[airport][i]];
                        path.flights.push_back(flight);
                        airport = flight.departure_airport;
                        arrival_time = flight.departure_time.time;
                        break;
                    }
                }
            }
            std::reverse(path.flights.begin(), path.flights.end());
            result.paths.push_back(path);
            return result;
        }
        for (int i = head[flight.arrival_airport]; i; i = nextflight[i]) {
            if (flights[i].departure_time.time >= last_arrival_time)
                break;
            if (flights[i].departure_time.time >= arrival_time[flight.arrival_airport]
                && flights[i].arrival_time.time <= result.end_time.time) {
                q.push({ flights[i], cost + flights[i].air_fares });
            }
        }
    }
    return result;
}

AllPathsResult Planner::query_all_paths(int airport_1, int airport_2, std::string start_time, std::string end_time)
{
    AllPathsResult result;
    result.airport_1 = airport_1;
    result.airport_2 = airport_2;
    result.start_time = parse_time(start_time);
    result.end_time = parse_time(end_time);
    if (airport_1 == airport_2) {
        std::cout << "Error: same source and destination." << std::endl;
        return result;
    }
    if (airport_num < airport_1) {
        std::cout << "Error: airport " << airport_1 << " does not exist." << std::endl;
        return result;
    }
    if (airport_num < airport_2) {
        std::cout << "Error: airport " << airport_2 << " does not exist." << std::endl;
        return result;
    }
    if (start_time > end_time) {
        std::cout << "Error: start time " << start_time << " is later than end time " << end_time << std::endl;
        return result;
    }
    for (int i = head[airport_1]; i; i = nextflight[i]) {
        if (flights[i].arrival_airport == airport_2
            && flights[i].departure_time.time >= result.start_time.time
            && flights[i].arrival_time.time <= result.end_time.time) {
            Path path;
            path.flights.push_back(flights[i]);
            result.paths.push_back(path);
        }
    }
    for (int i = head[airport_1]; i; i = nextflight[i]) {
        if (flights[i].departure_time.time < result.start_time.time
            || flights[i].arrival_time.time > result.end_time.time)
            continue;
        for (int j = head[flights[i].arrival_airport]; j; j = nextflight[j]) {
            if (flights[j].arrival_airport == airport_2
                && flights[j].departure_time.time >= flights[i].arrival_time.time
                && flights[j].arrival_time.time <= result.end_time.time) {
                Path path;
                path.flights.push_back(flights[i]);
                path.flights.push_back(flights[j]);
                result.paths.push_back(path);
            }
        }
    }
    return result;
}