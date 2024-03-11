#include <iostream>
#include <string>
#include <planner.hpp>
#include <readcsv.hpp>
#include <fstream>
#include <sstream>

int calculate_time(std::string value) // 5/5/2017 12:20  -> 61220
{
    int time=0;
    time+=(value[2]-'0')*10000;
    if (value[10]==':')
    {
        time+=(value[9]-'0')*100+(value[11]-'0')*10+(value[12]-'0')*1;
    }
    else
    {
        time+=(value[9]-'0')*1000+(value[10]-'0')*100+(value[11]-'0')*10+(value[12]-'0')*1;
    }
    return time;
}

void print_int(Vector<int>& ans)
{
    if (ans.empty())
    {
        std::cout<<"No Answer!";
    }
    else
    { 
        for (auto &i : ans)
        {
            std::cout<<i<<" -> ";
        }
        std::cout<<"end";
    }
    std::cout<<std::endl;
}

void print_data(Vector< Vector<Data> >& ans)
{
    if (ans.empty())
    {
        std::cout<<"No Answer!"<<std::endl;
    }
    else 
        for(auto& i:ans)
        {
            std::cout<<"one plan : "<<std::endl;
            for (auto j:i)
            {
                std::cout<<"FlightID = "<<j.FlightID<<" "<<"depart airport = "<<j.dairport<<" "<<"arrival airport = "<<j.aairport<<" "<<"dtime = "<<j.dtime<<" "<<"atime = "<<j.atime<<std::endl;
            }
            // std::cout<<"-----"<<std::endl;
        }
}

int main()
{
    Planner planner;
    Data data[2346];

    std::string csvFilePath = "../project/data/flight-data.csv";
    std::ifstream file(csvFilePath);

    if (!file.is_open())
    {
        std::cerr << "can not open file: "<<csvFilePath<<std::endl;
        return 1;
    }
    std::string line;

    // 逐行读取CSV文件内容
    int index=0;
    std::getline(file, line);
    while (std::getline(file,line))
    {
        std::istringstream iss(line);
        std::string value;

        // 以逗号为分隔符解析CSV行中的值
        int i = 0;
        while (std::getline(iss, value, ',')) 
        {
            switch (i)
            {
            case 0:
                data[index].FlightID = value;
                break;
            case 1:
                data[index].ddate = value;
                break;
            case 2:
                data[index].intlOrDome = value;
                break;
            case 3:
                data[index].FlightNumber = value;
            case 4:
                data[index].dairport = std::stoi(value);
                break;
            case 5:
                data[index].aairport = std::stoi(value);
                break;
            case 6://departure time
                data[index].dtime = calculate_time(value);
                break;
            case 7: //arrival time
                data[index].atime = calculate_time(value);
                break;
            case 8: 
                data[index].planeID = std::stoi(value);
                break;
            case 9:
                data[index].planeModel = value;
                break;
            case 10: 
                data[index].fare = std::stoi(value);
                break;
            default:
                break;
            }
            i++;
        }
        index++;
    }
    std::sort(data,data+2346);
    while (true)
    {
        // std::string query;
        // std::getline(std::cin, query);
        std::cout<<"please enter your query type"<<std::endl;
        std::cout<<"-----------------------------------------\n温馨提示："<<std::endl;
        std::cout<<"0: 给定出发机场，出发时间，执行深度优先遍历,对应函数：query_dfs.\n1: 给定出发机场，出发时间，执行广度优先遍历，对应函数：query_bfs.\n2: 给定机场A和B, 仅限直飞或 1 次中转, 求出从A到B的航线或者输出没有航线。对应函数：query_connectivity.\n3: 给定机场A和B以及起始结束时间, 求出从A到B的最小时间开销航线或者输出no answer。对应函数：query_shortest_path.\n4: 给定机场A和B以及起始结束时间, 求出从A到B的最小价格开销航线或者输出no answer。对应函数：query_minimum_cost_path.\n5: 给定机场A和B以及起始结束时间, 仅限直飞或 1 次中转, 求出从A到B的所有航线。对应函数：query_all_paths.\n6:退出"<<std::endl;
        std::cout<<"-----------------------------------------"<<std::endl;
        int type;
        std::cin>>type;
        if (type==0||type==1) 
        {
            std::cout<<"please enter start airport & start time"<<std::endl;
            int airport,start_time;
            std::cin>>airport>>start_time;
            if (type == 0)
            {
                auto ans = planner.query_dfs(airport,data,start_time);
                
                print_int(ans.dfsData);
            }
            else
            {
                auto ans = planner.query_bfs(airport,data,start_time);
                print_int(ans.bfsData);
            }
        }
        else if (type == 2)
        {
            std::cout<<"please enter start airport & end airport"<<std::endl;
            int airport_1,airport_2;
            std::cin>>airport_1>>airport_2;
            auto ans = planner.query_connectivity(airport_1,airport_2,data);
            print_data(ans.connectData);
        }
        else if(type == 6)
        {
            std::cout<<"----------quit successfully!-------------"<<std::endl;
            break;
        }
        else if(type==3||type==4||type==5)
        {
            std::cout<<"please enter start_airport, arrival_airport, start_time & end_time"<<std::endl;
            int airport_1,airport_2,start_time,end_time;
            std::cin>>airport_1>>airport_2>>start_time>>end_time;
            if (type==3)
            {
                auto ans = planner.query_shortest_path(airport_1,airport_2,start_time,end_time,data);
                print_data(ans.pathData);
            }
            else if (type == 4)
            {
                auto ans = planner.query_minimum_cost_path(airport_1,airport_2,start_time,end_time,data);
                print_data(ans.costData);
            }
            else if(type == 5)
            {
                auto ans = planner.query_all_paths(airport_1,airport_2,start_time,end_time,data);
                print_data(ans.allpathData);
            }
            
        }
        else
        {
            std::cout<<"err0r TYPE!"<<std::endl;
        }
        std::cout<<"按enter键继续"<<std::endl;
        getchar();
        getchar();
        /*
        int query_kind
        0: airport, start_time, dfs
        1: airport, start_time, bfs
        2: airportA, airportB, time, query_connectivity
        3: airportA, airportB, time, query_shortest_path
        4: airportA, airportB, time, query_minimum_cost_path
        5: airportA, airportB, time, query_all_paths
        */

    }
    return 0;
}
