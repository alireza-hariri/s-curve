#include <stdbool.h>
#include <stdio.h>
#include "s_curve.h"
#include "cubic_root.h"

// testing my fucntions
bool testing_CubicRoot()
{
    bool all_test_pass = true;

    all_test_pass &= is_close(_IQ(0.02), cube_root_fast(_IQ(0.000008)));
    all_test_pass &= is_close(_IQ(0.03), cube_root_fast(_IQ(0.000027)));
    all_test_pass &= is_close(_IQ(0.1), cube_root_fast(_IQ(0.001)));
    all_test_pass &= is_close(_IQ(0.3333), cube_root_fast(_IQ(1.0 / 27)));
    all_test_pass &= is_close(_IQ(1), cube_root_fast(_IQ(1)));
    all_test_pass &= is_close(_IQ(2), cube_root_fast(_IQ(8)));
    all_test_pass &= is_close(_IQ(3), cube_root_fast(_IQ(27)));
    all_test_pass &= is_close(_IQ(5), cube_root_fast(_IQ(125)));

    printf("cube-root tests: %s\n", all_test_pass ? "pass" : "fail");
    return all_test_pass;
}

bool testing_RampTravelDistance()
{
    bool all_test_pass = true;
    RampCalculationResult R;

    R = RampTravelDistance(_IQ(1.5), _IQ(0.2), _IQ(3.2), _IQ(1.5));
    all_test_pass &= is_close(_IQ(1), R.curve_time);
    all_test_pass &= is_close(_IQ(0.75), R.speed_diff_curve);
    all_test_pass &= is_close(_IQ(5.1), R.travel);

    R = RampTravelDistance(_IQ(1.5), _IQ(3), _IQ(3.2), _IQ(1.5));
    all_test_pass &= is_close(_IQ(0.365148), R.curve_time);
    all_test_pass &= is_close(_IQ(0.100000), R.speed_diff_curve);
    all_test_pass &= is_close(_IQ(2.263919), R.travel);

    printf("half-ramp tests: %s\n", all_test_pass ? "pass" : "fail");
    return all_test_pass;
}

bool testing_calculate_speed_params()
{
    bool all_test_pass = true;
    _iq distance;

    // noting special
    distance = _IQ(9.5);
    S_CurveResult r = calculate_speed_params(
        _IQ(1.5), // jerk
        _IQ(1.5), // acc
        _IQ(1.5), // dcc
        _IQ(0.0), // v1
        _IQ(3.0), // v2
        _IQ(0.0), // v3
        distance);
    _iq middle_travel = distance - r.up_ramp.travel - r.down_ramp.travel;
    all_test_pass &= is_close(_IQ(0.5), middle_travel);

    // up-ramp and down-ramp of speed are meeting
    distance = _IQ(3.5);
    r = calculate_speed_params(
        _IQ(1.5), // jerk
        _IQ(1.5), // acc
        _IQ(1.5), // dcc
        _IQ(0.0), // v1
        _IQ(3.0), // v2
        _IQ(0.0), // v3
        distance);
    middle_travel = distance - r.up_ramp.travel - r.down_ramp.travel;
    all_test_pass &= is_close(_IQ(0.0), middle_travel);

    // speed curves are meeting
    distance = _IQ(1.5);
    r = calculate_speed_params(
        _IQ(1.5), // jerk
        _IQ(1.5), // acc
        _IQ(1.5), // dcc
        _IQ(0.0), // v1
        _IQ(3.0), // v2
        _IQ(0.0), // v3
        distance);
    middle_travel = distance - r.up_ramp.travel - r.down_ramp.travel;
    all_test_pass &= is_close(_IQ(0.0), middle_travel);
    all_test_pass &= is_close(r.max_possible_speed, r.up_ramp.speed_diff_curve*2);
    

    printf("full-ramp tests: %s\n", all_test_pass ? "pass" : "fail");
    return all_test_pass;
}

int main()
{
    bool test_res1 = testing_CubicRoot();
    bool test_res2 = testing_RampTravelDistance();
    bool test_res3 = testing_calculate_speed_params();
    return !(test_res1 && test_res2 && test_res3);
}