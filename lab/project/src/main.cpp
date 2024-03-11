#include <iostream>
#include <planner.hpp>
#include <string>

#define DEFAULT_START_TIME "5/5/2017 0:30"
#define DEFAULT_END_TIME "5/9/2017 9:40"
#define DEFAULT_MAX_AIRPORT 100
#define DEFAULT_DATA_PATH "./data/flight-data.csv"

int main()
{
    Planner planner(DEFAULT_MAX_AIRPORT);
    while (1) {
        std::cout << "Please input data path:(default path:" << DEFAULT_DATA_PATH << ")";
        std::string line;
        getline(std::cin, line);
        if (line == "") {
            line = DEFAULT_DATA_PATH;
        }
        if (!planner.load(line))
            break;
    }
    planner.sort();
    while (true) {
        std::cout << std::endl;
        std::cout << "Please select query type (input number):" << std::endl;
        std::cout << "0. Exit" << std::endl;
        std::cout << "1. Search by flight id" << std::endl;
        std::cout << "2. Search by airport id" << std::endl;
        std::cout << "3. DFS search" << std::endl;
        std::cout << "4. BFS search" << std::endl;
        std::cout << "5. Connectivity check (no more than 1 flight connection)" << std::endl;
        std::cout << "6. Get fastest path" << std::endl;
        std::cout << "7. Get minimum cost path" << std::endl;
        std::cout << "8. Get all paths (no more than 1 flight connection)" << std::endl;
        std::string line;
        getline(std::cin, line);
        int type = -1;
        try {
            type = std::stoi(line);
        } catch (std::exception& e) {
            std::cout << "Please input number" << std::endl;
        }
        if (type == 0)
            break;
        else if (type == 1) {
            std::cout << "Please input flight id (0 for exit)" << std::endl;
            while (true) {
                getline(std::cin, line);
                int flight_id = -1;
                try {
                    flight_id = std::stoi(line);
                } catch (std::exception& e) {
                    std::cout << "Please input number" << std::endl;
                }
                if (flight_id == 0)
                    break;
                else if (flight_id > 0)
                    planner.printflight(flight_id);
            }
        } else if (type == 2) {
            std::cout << "Please input airport id (0 for exit)" << std::endl;
            while (true) {
                getline(std::cin, line);
                int airport_id = -1;
                try {
                    airport_id = std::stoi(line);
                } catch (std::exception& e) {
                    std::cout << "Please input number" << std::endl;
                }
                if (airport_id == 0)
                    break;
                else if (airport_id > 0) {
                    std::cout << std::endl;
                    planner.printairport(airport_id);
                }
            }
        } else if (type == 3) {
            while (true) {
                std::cout << "Please input airport id (0 for exit):";
                getline(std::cin, line);
                int airport_id = -1;
                try {
                    airport_id = std::stoi(line);
                } catch (std::exception& e) {
                    std::cout << "Please input number" << std::endl;
                }
                if (airport_id == 0)
                    break;
                else if (airport_id > 0) {
                    std::cout << "Please input start time (Default:" << DEFAULT_START_TIME << "):";
                    std::string start_time;
                    getline(std::cin, start_time);
                    if (start_time == "")
                        start_time = DEFAULT_START_TIME;
                    try {
                        auto res = planner.query_dfs(airport_id, start_time);
                        std::cout << std::endl;
                        std::cout << "DFS from airport " << res.airport_id << " at " << to_string(res.start_time) << ":" << std::endl;
                        print_airport_result(res);
                        std::cout << std::endl;
                    } catch (std::exception& e) {
                        std::cout << "Please input correct time format" << std::endl;
                    }
                }
            }
        } else if (type == 4) {
            while (true) {
                std::cout << "Please input airport id (0 for exit):";
                getline(std::cin, line);
                int airport_id = -1;
                try {
                    airport_id = std::stoi(line);
                } catch (std::exception& e) {
                    std::cout << "Please input number" << std::endl;
                }
                if (airport_id == 0)
                    break;
                else if (airport_id > 0) {
                    std::cout << "Please input start time (Default:" << DEFAULT_START_TIME << "):";
                    std::string start_time;
                    getline(std::cin, start_time);
                    if (start_time == "")
                        start_time = DEFAULT_START_TIME;
                    try {
                        auto res = planner.query_bfs(airport_id, start_time);
                        std::cout << std::endl;
                        std::cout << "BFS from airport " << res.airport_id << " at " << to_string(res.start_time) << ":" << std::endl;
                        print_airport_result(res);
                        std::cout << std::endl;
                    } catch (std::exception& e) {
                        std::cout << "Please input correct time format" << std::endl;
                    }
                }
            }
        } else if (type == 5) {
            while (true) {
                std::cout << "Please input departure airport id (0 for exit):";
                getline(std::cin, line);
                int airport_1 = -1;
                try {
                    airport_1 = std::stoi(line);
                } catch (std::exception& e) {
                    std::cout << "Please input number" << std::endl;
                }
                if (airport_1 == 0)
                    break;
                if (airport_1 > 0) {
                    std::cout << "Please input arrival airport id:";
                    getline(std::cin, line);
                    int airport_2 = -1;
                    try {
                        airport_2 = std::stoi(line);
                    } catch (std::exception& e) {
                        std::cout << "Please input number" << std::endl;
                    }
                    if (airport_2 > 0) {
                        auto res = planner.query_connectivity(airport_1, airport_2);
                        std::cout << std::endl;
                        std::cout << "Connectivity check from airport " << airport_1 << " to airport " << airport_2 << ":" << std::endl;
                        if (res.is_connected)
                            print_path_result(res);
                        else
                            std::cout << "No pathes no more than 1 flight connection" << std::endl;
                        std::cout << std::endl;
                    }
                }
            }
        } else if (type == 6) {
            while (true) {
                std::cout << "Please input departure airport id (0 for exit):";
                getline(std::cin, line);
                int airport_1 = -1;
                try {
                    airport_1 = std::stoi(line);
                } catch (std::exception& e) {
                    std::cout << "Please input number" << std::endl;
                }
                if (airport_1 == 0)
                    break;
                if (airport_1 > 0) {
                    std::cout << "Please input arrival airport id:";
                    getline(std::cin, line);
                    int airport_2 = -1;
                    try {
                        airport_2 = std::stoi(line);
                    } catch (std::exception& e) {
                        std::cout << "Please input number" << std::endl;
                    }
                    if (airport_2 > 0) {
                        std::cout << "Please input start time (Default:" << DEFAULT_START_TIME << "):";
                        std::string start_time;
                        getline(std::cin, start_time);
                        if (start_time == "")
                            start_time = DEFAULT_START_TIME;
                        std::cout << "Please input end time (Default:" << DEFAULT_END_TIME << "):";
                        std::string end_time;
                        getline(std::cin, end_time);
                        if (end_time == "")
                            end_time = DEFAULT_END_TIME;
                        try {
                            auto res = planner.query_shortest_path(airport_1, airport_2, start_time, end_time);
                            std::cout << std::endl;
                            std::cout << "Shortest path from airport " << airport_1 << " to airport " << airport_2 << " from " << to_string(res.start_time) << " to " << to_string(res.end_time) << ":" << std::endl;
                            print_path_result(res);
                            std::cout << std::endl;
                        } catch (std::exception& e) {
                            std::cout << "Please input correct time format" << std::endl;
                        }
                    }
                }
            }
        } else if (type == 7) {
            while (true) {
                std::cout << "Please input departure airport id (0 for exit):";
                getline(std::cin, line);
                int airport_1 = -1;
                try {
                    airport_1 = std::stoi(line);
                } catch (std::exception& e) {
                    std::cout << "Please input number" << std::endl;
                }
                if (airport_1 == 0)
                    break;
                if (airport_1 > 0) {
                    std::cout << "Please input arrival airport id:";
                    getline(std::cin, line);
                    int airport_2 = -1;
                    try {
                        airport_2 = std::stoi(line);
                    } catch (std::exception& e) {
                        std::cout << "Please input number" << std::endl;
                    }
                    if (airport_2 > 0) {
                        std::cout << "Please input start time (Default:" << DEFAULT_START_TIME << "):";
                        std::string start_time;
                        getline(std::cin, start_time);
                        if (start_time == "")
                            start_time = DEFAULT_START_TIME;
                        std::cout << "Please input end time (Default:" << DEFAULT_END_TIME << "):";
                        std::string end_time;
                        getline(std::cin, end_time);
                        if (end_time == "")
                            end_time = DEFAULT_END_TIME;
                        try {
                            auto res = planner.query_minimum_cost_path(airport_1, airport_2, start_time, end_time);
                            std::cout << std::endl;
                            std::cout << "Minimum cost path from airport " << airport_1 << " to airport " << airport_2 << " from " << to_string(res.start_time) << " to " << to_string(res.end_time) << ":" << std::endl;
                            print_path_result(res);
                            if (!res.paths.empty())
                                std::cout << "Total Cost:" << res.total_cost << std::endl;
                            std::cout << std::endl;
                        } catch (std::exception& e) {
                            std::cout << "Please input correct time format" << std::endl;
                        }
                    }
                }
            }
        } else if (type == 8) {
            while (true) {
                std::cout << "Please input departure airport id (0 for exit):";
                getline(std::cin, line);
                int airport_1 = -1;
                try {
                    airport_1 = std::stoi(line);
                } catch (std::exception& e) {
                    std::cout << "Please input number" << std::endl;
                }
                if (airport_1 == 0)
                    break;
                if (airport_1 > 0) {
                    std::cout << "Please input arrival airport id:";
                    getline(std::cin, line);
                    int airport_2 = -1;
                    try {
                        airport_2 = std::stoi(line);
                    } catch (std::exception& e) {
                        std::cout << "Please input number" << std::endl;
                    }
                    if (airport_2 > 0) {
                        std::cout << "Please input start time (Default:" << DEFAULT_START_TIME << "):";
                        std::string start_time;
                        getline(std::cin, start_time);
                        if (start_time == "")
                            start_time = DEFAULT_START_TIME;
                        std::cout << "Please input end time (Default:" << DEFAULT_END_TIME << "):";
                        std::string end_time;
                        getline(std::cin, end_time);
                        if (end_time == "")
                            end_time = DEFAULT_END_TIME;
                        try {
                            auto res = planner.query_all_paths(airport_1, airport_2, start_time, end_time);
                            std::cout << std::endl;
                            std::cout << "All pathes from airport " << airport_1 << " to airport " << airport_2 << " from " << to_string(res.start_time) << " to " << to_string(res.end_time) << ":" << std::endl;
                            print_path_result(res);
                            std::cout << std::endl;
                        } catch (std::exception& e) {
                            std::cout << "Please input correct time format" << std::endl;
                        }
                    }
                }
            }
        }
    }
    return 0;
}
