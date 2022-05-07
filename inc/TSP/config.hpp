#ifndef _tsp_config_hpp
#define _tsp_config_hpp

#ifndef TSP_MAX_POINT_NUM
#define TSP_MAX_POINT_NUM 200
#endif

#ifndef TSP_TERM_COND
#define TSP_TERM_COND 1e-2
#endif

namespace TSP {

constexpr int MAXN = TSP_MAX_POINT_NUM;
constexpr float term_cond = TSP_TERM_COND;

}  // namespace TSP

#endif  // _tsp_config_hpp