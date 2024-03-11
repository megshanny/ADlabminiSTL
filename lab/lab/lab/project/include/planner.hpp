#include <miniSTL/stl.hpp>
#include <string>
#include <tuple>
// #include <vector>
#include <set>
#include <readcsv.hpp>
struct DFSResult
{
    Vector<int> dfsData;
};

struct BFSResult
{
    // std::set<int> bfsData1;
    Vector<int> bfsData;
};

struct ConnectivityResult
{
    Vector< Vector<Data> > connectData;
};

struct ShortestPathResult
{
    Vector< Vector<Data> > pathData;
};

struct MinimumCostPathResult
{
    Vector< Vector<Data> > costData;
};

struct AllPathsResult
{
    Vector< Vector<Data> > allpathData;
};



struct Planner
{

    DFSResult query_dfs(int airport_id, Data* data, int stime);
    BFSResult query_bfs(int airport_id, Data* data, int stime);
    ConnectivityResult query_connectivity(int airport_1, int airport_2, Data* data);
    ShortestPathResult query_shortest_path(int airport_1, int airport_2, int start_time, int end_time, Data* data);
    MinimumCostPathResult query_minimum_cost_path(int airport_1, int airport_2, int start_time, int end_time, Data* data);
    AllPathsResult query_all_paths(int airport_1, int airport_2, int start_time, int end_time, Data* data);
};
