
#include "IQmathLib.h"
#include <stdbool.h>


typedef struct
{
    bool curves_are_meeting; // can upper and lower curve meet?
    _iq const_acc_time;
    _iq travel;
    _iq curve_time;       // acceleration time ( time-to-reach-maximum-acc )
    _iq speed_diff_curve; // = a^2/2j  ( v in the formula )
} RampCalculationResult;

typedef struct
{
    _iq max_possible_speed;
    RampCalculationResult up_ramp;
    RampCalculationResult down_ramp;
} S_CurveResult;


RampCalculationResult RampTravelDistance(_iq jerk, _iq speed_low, _iq speed_hi, _iq max_acc);

S_CurveResult calculate_speed_params(_iq jerk, _iq max_acc, _iq max_dcc, _iq speed_start, _iq speed_max, _iq speed_end, _iq distance);

_iq second_order_equation_ans1(_iq a, _iq b, _iq c);
bool is_close(_iq a, _iq b);