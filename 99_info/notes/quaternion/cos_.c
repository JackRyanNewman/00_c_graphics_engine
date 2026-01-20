#include <math.h>
#include <float.h>
//--Defines-----------------------------------------------------
  #define PI_OVER_4 0.7853981633974483  // π/4
  #define PI_OVER_2 1.5707963267948966  // π/2
    const double C1 =  4.16666666666666019037e-02; // 1/24
    const double C2 = -1.38888888888741095749e-03; // -1/720
    const double C3 =  2.48015872894767294178e-05;
    const double S1 = -1.66666666666666324348e-01; // -1/6
    const double S2 =  8.33333333332248946124e-03; // 1/120
    const double S3 = -1.98412698298579493134e-04;
//-------------------------------------------------------------

// Type	        Mantissa bits	Kernel degree	Number of constants (beyond r²)
// float	      23 bits	     ~6–8	          2–3
// double	      53 bits	     ~12	          6
// long double	64–113 bits	 14–20	        7–10+

//How cosine works
//0. Check != nan, most likely fast math turns this off? 
//1. Check if in range of [-π/4, π/4], bc simple math allows this range to represent anywhere within circle. 
//2. If within range Then call kernel, which uses the value, and multplies it by constants that are precomputed based off hardware arhiecture. Where it does taylor series approximinations based off the data type and how much constants it needs. 
//3. IF not within range, calcualte quadrant, and reduce angle, then a switch case. then a sepcific function. 

//Step 1. Check Range. 5 arith
//Step 2. Kernel 6 multi, 5 arith
  //Step 3 k: 1 multi, 4 instursctions(round and int) 
  //Step 4 x: 1 multi, 1 arith. 
  //Step swi: 1 arith, 2=arith for each switch. 

//Best case: 6 multi, 11 arith 
//wort case on first branch: 9 multi, 18 airth
//worst case on last brnach: 9 multi, 24 airth


double my_cos(double x){
    if (isnan(x) || isinf(x)) return NAN;           // Step 0: special cases
    if (fabs(x) <= PI_OVER_4) return kernel_cos(x); // Step 1: fast path for small x # //1 bit(artith), <=(4 arthi), 
    


    // Step 2: argument reduction
    int k = (int)round(x / PI_OVER_2);  // quadrant
    double r = x - k * PI_OVER_2;       // reduced angle [-π/4, π/4]

    // Step 3: quadrant logic
    switch (k & 3) {
        case 0: return  kernel_cos(r);
        case 1: return -kernel_sin(r);
        case 2: return -kernel_cos(r);
        case 3: return  kernel_sin(r);
    }

    // Fallback (should never happen)
    return NAN;
}


double kernel_cos(double r){
    double z = r * r; // Kernel cosine assumes |r| <= π/4           //1multi. 
    return 1.0 - 0.5 * z + z * z * (C1 + z * (C2 + z * (C3 + z)));  //5 arith, 5 multiply....
}

double kernel_sin(double r){
    double z = r * r; // Kernel sine assumes |r| <= π/4
    return r + r * z * (S1 + z * (S2 + z * (S3 + z)));
}
