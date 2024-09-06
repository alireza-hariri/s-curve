#include <stdbool.h>

#include "IQmathLib.h"
#include "cubic_root.h"
#include "s_curve.h"



//  calculations of trajectory params
//
// on a half ramp (speed s-curve) with no initial speed (v1) we can write:
// a = j * t
// v = (j * t^2) / 2
// d = (j * t^3) / 6
//
//  =>  v/a^2 = 1/2j  =>  2*v*j = a^2

/* metrics */ 
/// dist: m
/// speed: m/s
/// Acceleration: m/s^2
/// jerk: m/s^3



RampCalculationResult RampTravelDistance(_iq jerk, _iq speed_low, _iq speed_hi, _iq max_acc)
{
    // calculating upward or downward ramp params. (assuming that the speed_hi can be reached) 
    _iq speed_diff = speed_hi - speed_low;
    _iq s_curve_acc; // end of s-curve acc (may be lower than max_acc)
    _iq const_acc_time;
    RampCalculationResult R = {false, 0, 0, 0};

    // calculate s_curve based on assumption that upper and lower curve will not meet
    R.speed_diff_curve = _IQdiv(_IQmpy(max_acc, max_acc), 2 * jerk);
    if ((R.speed_diff_curve * 2) > speed_diff)
    {
        // the above assumption was wrong
        R.curves_are_meeting = true;
        R.speed_diff_curve = speed_diff / 2;
        s_curve_acc = _IQsqrt(_IQmpy(2 * R.speed_diff_curve, jerk));
        const_acc_time = 0;
    }
    else
    {
        s_curve_acc = max_acc;
        const_acc_time = _IQdiv(speed_diff - R.speed_diff_curve * 2, s_curve_acc);
    }
    R.curve_time = _IQdiv(s_curve_acc, jerk);

    //    // 3 parts of curve :
    //    // low curve
    //    travel = _IQmpy(speed_low,curve_time);// + curve_time*curve_time*s_curve_acc/6;
    //    // middle part
    //    travel += _IQmpy((speed_low+speed_hi)/2,curve_time);
    //    // upper curve
    //    travel += _IQmpy(speed_hi,curve_time);// - curve_time*curve_time*s_curve_acc/6;
    // simple method
    R.travel = _IQmpy((speed_low + speed_hi) / 2, const_acc_time + R.curve_time * 2);
    return R;
}

_iq one = _IQ(1);
S_CurveResult calculate_speed_params(_iq jerk, _iq max_acc, _iq max_dcc, _iq speed_start, _iq speed_max, _iq speed_end, _iq distance)
{
    // Note that real value of max_speed and max_acc may be lower based on calculations
    static RampCalculationResult R1;
    static RampCalculationResult R2;
    R1 = RampTravelDistance(jerk, speed_start, speed_max, max_acc);
    R2 = RampTravelDistance(jerk, speed_end, speed_max, max_dcc);
    if ((R1.travel + R2.travel) > distance)
    {
        // we need lower speed_max
        _iq min_tralvel;
        min_tralvel = _IQmpy(R1.curve_time * 2, speed_start + R1.speed_diff_curve);
        min_tralvel += _IQmpy(R2.curve_time * 2, speed_end + R2.speed_diff_curve);

        if ((!R1.curves_are_meeting) && (min_tralvel < distance))
        {
            // upper and lower curves are not meeting
            _iq v1 = speed_start;
            _iq v3 = speed_end;
            _iq t1 = _IQdiv(max_acc, jerk);
            _iq t2 = _IQdiv(max_dcc, jerk);
            // d = ((v1+v2)/2) * ( (v2-v1)/a1 + t1 ) + ((v3+v2)/2) * ( (v2-v3)/a2 + t1 )
            // => (v2^2 - v1^2)/a1 + (v2^2 - v3^2)/a2 + (v1+v2)*t1 + (v3+v2)*t2 - 2d = 0
            // => (1/a1 + 1/a2) v2^2  +  (t1+t2) v2  + (v1*t1 + v3*t2 - v1^2/a1 - v3^2/a2 - 2d) = 0

            speed_max = second_order_equation_ans1(
                _IQdiv(one, max_acc) + _IQdiv(one, max_dcc),
                t1 + t2,
                _IQmpy(t1, v1) + _IQmpy(t2, v3) - _IQdiv(_IQmpy(v1, v1), max_acc) - _IQdiv(_IQmpy(v3, v3), max_dcc) - 2 * distance);
            // TODO: you can make it faster insted of code-reuse
            R1 = RampTravelDistance(jerk, speed_start, speed_max, max_acc);
            R2 = RampTravelDistance(jerk, speed_end, speed_max, max_dcc);
        }
        else
        {
            // curves should meet
            // this this scenario we assume the following conditions:
            // 1. dcc == acc
            // 2. speed_end == speed_start (v1==v3)
            // 3. v1 << v2
            _iq d = distance / 2;
            speed_max = cube_root_fast(_IQmpy(_IQmpy(d, d), jerk));
            speed_max += speed_start * 2 / 3;
            max_dcc = max_acc;
            speed_end = speed_start;
            R1 = RampTravelDistance(jerk, speed_start, speed_max, max_acc);
            R2 = R1;
        }
    }
    // _iq middle_travel = distance - R1.travel - R2.travel;
    S_CurveResult result = {speed_max,R1,R2};
    return result;
}

_iq second_order_equation_ans1(_iq a, _iq b, _iq c)
{
    // first answer of second-order-equation
    // ax^2 + bx + c = 0
    // x = (-b + sqrt(b^2 - 4ac)) / 2a
    return _IQdiv(-b + _IQsqrt(_IQmpy(b, b) - _IQmpy(4 * a, c)), 2 * a);
}

bool is_close(_iq24 a, _iq24 b){
    // are a and b close (0.0001 precision)
    return abs(a-b) < (1<<11);
}
