#ifndef _tsp_config_hpp
#define _tsp_config_hpp

#ifndef TSP_MAX_POINT_NUM
#define TSP_MAX_POINT_NUM 2000
#endif

namespace TSP {

// the +1 space will be used when calculating the path without returning to the starting point
constexpr int MAXN = TSP_MAX_POINT_NUM + 1;

}  // namespace TSP

#endif  // _tsp_config_hpp