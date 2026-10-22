#include "demand_forecast.hpp"
#include <algorithm>
#include <cmath>
#include <stdexcept>
namespace water {
ForecastResult forecast_demand(const std::vector<DemandObservation>& history, double future_time_step) {
    if (history.empty()) throw std::invalid_argument("Demand history cannot be empty.");
    double mean_x = 0.0, mean_y = 0.0;
    for (const auto& item : history) {
        if (!std::isfinite(item.time_step) || !std::isfinite(item.demand) || item.demand < 0.0)
            throw std::invalid_argument("Observations must be finite and demand cannot be negative.");
        mean_x += item.time_step; mean_y += item.demand;
    }
    mean_x /= static_cast<double>(history.size()); mean_y /= static_cast<double>(history.size());
    double numerator = 0.0, denominator = 0.0;
    for (const auto& item : history) {
        const double dx = item.time_step - mean_x;
        numerator += dx * (item.demand - mean_y); denominator += dx * dx;
    }
    const double slope = denominator == 0.0 ? 0.0 : numerator / denominator;
    const double intercept = mean_y - slope * mean_x;
    const double prediction = std::max(0.0, intercept + slope * future_time_step);
    return {prediction, static_cast<long long>(std::llround(prediction)), slope, intercept};
}
}
