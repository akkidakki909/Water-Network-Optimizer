#include "demand_forecast.hpp"
#include <cassert>
#include <cmath>
#include <iostream>
#include <stdexcept>
int main() {
    using namespace water;
    auto result = forecast_demand({{0,10},{1,12},{2,14},{3,16}}, 4);
    assert(std::abs(result.predicted_demand - 18.0) < 1e-9);
    assert(result.recommended_demand_limit == 18);
    auto flat = forecast_demand({{1,5},{1,7}}, 2);
    assert(std::abs(flat.predicted_demand - 6.0) < 1e-9);
    bool threw = false;
    try { (void)forecast_demand({}, 4); } catch (const std::invalid_argument&) { threw = true; }
    assert(threw);
    auto low = forecast_demand({{0,2},{1,1}}, 10);
    assert(low.recommended_demand_limit == 0);
    std::cout << "All 4 demand forecasting tests passed.\n";
}
