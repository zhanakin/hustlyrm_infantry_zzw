#ifndef _TOOLS_H
#define _TOOLS_H

#ifndef TRUE
#define TRUE 1
#endif // !
#ifndef FALSE
#define FALSE 0
#endif // !
#define LIMIT_MAX_MIN(x, max, min) (((x) <= (min)) ? (min) : (((x) >= (max)) ? (max) : (x)))
#ifndef PI
#define PI 3.14159265358979f
#endif
#ifndef PI_2
#define PI_2 6.2831853072f
#endif

#define SQRT_2 1.41421356f
#define _DIVIDE_SQRT_2 0.70710678f
#define MIN(a, b) ((a) < (b)) ? (a) : (b)

/* 普通延时 */
void delay_ms(unsigned long t);
void delay_us(unsigned long t);
void delay_us_f(float us);

#endif // !_TOOLS_H
