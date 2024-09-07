

## intorduction

<div style="font-size: 16px; text-indent:10px;">
S-curve planning is a common practice in robotics and mechanical systems for calculating speed or position trajectories. In my case, I am running my code on a limited CPU (TMS320F280) without floating point unit and I want to avoid floating-point calculations by using Texas Instruments' fixed-point library, `IQmathLib`. 
<p> `IQmath` library has a lot of basic operations, such as sqrt, but does not include a third root, so i had to implement a `cube_root_fast` function on 32-bit numbers, assuming 24 bit of fixed-point digits and 8 bit of signed integer</p>

</div>

### debugging in vscode
First of all, I am not an expert in C debuggers, and they seem somewhat magical to me. Anyway, this repository has a normal Makefile and can be debugged using VSCode's `Makefile Tools` extension.  

Keep in mind that all numbers have 24 bits of fixed precision, and to see their real values in debugger you should divide them by `16777216.0`. 

## other repos
- [s-curve implementation in rust](https://github.com/marcbone/s_curve)
- [s-curve implementation in python](https://github.com/nameofuser1/py-scurve)