#include <planner.hpp>
// #include <readcsv.hpp>
// #include <vector>
#include <iostream>
#include <queue>
int datasize = 2346;
/*
注意看：
*/

Vector<int> state(100);
Vector<int> temp;
DFSResult dfsResult;

void dfs(int airport_id, Data* data, int stime)
{
    state[airport_id] = 1;
    dfsResult.dfsData.push_back(airport_id);

    for (int i = 0; i < datasize; i++) 
    {
        if (state[data[i].aairport]==0 && data[i].dairport == airport_id && data[i].dtime > stime)
        {
            dfs(data[i].aairport,data,data[i].atime);
        }
    }
}

DFSResult Planner::query_dfs(int airport_id, Data* data, int stime)
{   
    dfsResult.dfsData.clear();
    state.assign(state.size(), 0);
    dfs(airport_id,data,stime);
    return dfsResult;
}


BFSResult Planner::query_bfs(int airport_id, Data* data, int stime)
{
    BFSResult bfsRes;
    state.assign(state.size(), 0);
    std::queue<Data> qu;
    bfsRes.bfsData.push_back(airport_id);
    for (int i = 0; i < datasize; i++)
    {
        if (data[i].dairport == airport_id && data[i].dtime > stime)
        {
            qu.push(data[i]);
            state[airport_id]=1;
            bfsRes.bfsData.push_back(data[i].aairport);
            state[data[i].aairport]=1;
            break;
        }
    }
    while (!qu.empty())
    {
        Data temp = qu.front();
        qu.pop();
        for (int i = 0; i < datasize; i++)
        {
            if (data[i].dairport == temp.aairport && state[data[i].aairport]==0 && data[i].dtime > temp.atime)
            {
                qu.push(data[i]);
                bfsRes.bfsData.push_back(data[i].aairport);
                state[data[i].aairport]=1;
                break;
            }
        }
    }
    
    return bfsRes;
    
}

ConnectivityResult Planner::query_connectivity(int airport_1, int airport_2, Data* data)
{
    ConnectivityResult connectRes;
    for (int i = 0; i < datasize; i++)
    {
        if (data[i].dairport == airport_1)
        {
            if (data[i].aairport == airport_2)
            {
                connectRes.connectData.push_back({data[i]});
            }
            else
            {
                int mid_airport = data[i].aairport;
                Vector<int> temp;
                for (int j = 0; j < datasize; j++)
                {
                    if (data[j].dairport == mid_airport && data[j].aairport == airport_2 && data[i].atime < data[j].dtime)
                    { 
                        connectRes.connectData.push_back({data[i],data[j]});
                    }
                }
            }
        }
    }
    return connectRes;
}

ShortestPathResult pathRes;
Vector<Data> pathTemp;
int min_time;

void time_dfs(int airport_id, int airport_2, Data* data, int start_time, int sum_time)
{
    state[airport_id] = 1;
    
    if (airport_id == airport_2)
    {
        pathRes.pathData.push_back(pathTemp);
        min_time = sum_time;
        return; 
    }
    
    for (int i = 0; i < datasize; i++)
    {
        int timeadd = data[i].atime-data[i].dairport + sum_time;
        if (state[data[i].aairport]==0 && data[i].dairport == airport_id && data[i].dtime > start_time && timeadd < min_time)
        {
            pathTemp.push_back(data[i]);
            time_dfs(data[i].aairport,airport_2,data,data[i].atime,timeadd);
            pathTemp.pop_back();
        }
    }
}

ShortestPathResult Planner::query_shortest_path(int airport_1, int airport_2, int start_time, int end_time, Data* data)
{
    pathRes.pathData.clear();
    min_time = end_time - start_time;
    pathTemp.clear();
    state.assign(state.size(), 0);
    time_dfs(airport_1,airport_2,data,start_time,0);
    return pathRes;
}


MinimumCostPathResult costRes;
int min_cost = INT_MAX;

void cost_dfs(int airport_id, int airport_2, Data* data, int start_time, int end_time, int sum_cost)
{
    state[airport_id] = 1;
    
    if (airport_id == airport_2)
    {
        costRes.costData.push_back(pathTemp);
        min_cost = sum_cost;
        return; 
    }
    
    for (int i = 0; i < datasize; i++)
    {
        int costAdd = sum_cost+data[i].fare;
        if (state[data[i].aairport]==0 && data[i].dairport == airport_id && data[i].dtime > start_time && costAdd < min_cost && data[i].atime < end_time) 
        {
            pathTemp.push_back(data[i]);
            cost_dfs(data[i].aairport,airport_2,data,data[i].atime,end_time,costAdd);
            pathTemp.pop_back();
        }
    }
}
MinimumCostPathResult Planner::query_minimum_cost_path(int airport_1, int airport_2, int start_time, int end_time, Data* data)
{
    pathTemp.clear();
    state.assign(state.size(), 0);
    costRes.costData.clear();
    cost_dfs(airport_1,airport_2,data,start_time,end_time,0);
    return costRes;
}


AllPathsResult Planner::query_all_paths(int airport_1, int airport_2, int start_time, int end_time, Data* data)
{
    AllPathsResult allpathRes;
    for (int i = 0; i < datasize; i++)
    {
        if (data[i].dairport == airport_1 && data[i].dtime > start_time )
        {
            if (data[i].aairport == airport_2 && data[i].atime < end_time)
            {
                allpathRes.allpathData.push_back({data[i]});
            }
            else
            {
                int mid_airport = data[i].aairport;
                Vector<int> temp;
                for (int j = 0; j < datasize; j++)
                {
                    if (data[j].dairport == mid_airport && data[j].aairport == airport_2 && data[i].atime < data[j].dtime && data[j].atime <= end_time)
                    { 
                        allpathRes.allpathData.push_back({data[i],data[j]});
                    }
                }
            }
        }
    }
    return allpathRes;
}