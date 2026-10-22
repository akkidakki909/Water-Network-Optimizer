#pragma once
#include <vector>
namespace water {
struct DemandObservation { double time_step; double demand; };
struct ForecastResult { double predicted_demand; long long recommended_demand_limit; double slope; double intercept; };
ForecastResult forecast_demand(const std::vector<DemandObservation>& history, double future_time_step);
}
