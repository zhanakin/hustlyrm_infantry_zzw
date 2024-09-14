#include "PowerLimit.h"
#include "ina260.h"
#include "SuperPower.h"
#include "ChasisController.h"

void PowerLimitInit(PowerLimiter *limitter, int motor_num, MOTOR_TYPE motor_type, PowerLimitMethod method)
{
    limitter->motor_num = LIMIT_MAX_MIN(motor_num, 4, 1);
    limitter->motor_type = motor_type;
    limitter->power_limit_method = method;
}

     void PowerLimit(PowerLimiter *limitter, float set_power)
{
    float MOTOR_R, MOTOR_K, MOTOR_B, MOTOR_P0;

    if (limitter->motor_type == MF9025)
    {
        MOTOR_R = MF9025_R;
        MOTOR_K = MF9025_K;
        MOTOR_B = MF9025_B;
        MOTOR_P0 = MF9025_P0;
    }
    else if (limitter->motor_type == M3508)
    {
        MOTOR_R = M3508_R;
        MOTOR_K = M3508_K;
        MOTOR_B = M3508_B;
        MOTOR_P0 = M3508_P0;
    }

    float i_2, w_i, w_2, sum_i_2 = 0.0f, sum_w_i = 0.0f, sum_w_2 = 0.0f;
    limitter->predict_send_power = 0.0f;

    for (int i = 0; i < limitter->motor_num; i++)
    {
        // 底盘3508电机掉线处理，如果3508掉了，不把该3508计入功率预测
        // 若3508电机运动过程中can突然掉线，电调反馈的W和I保持掉线前一刻的值不再更新。如果该掉线电机前一刻W或I很大,即前一刻功率预测很大，掉线后该3508功率预测一直保持该值。最后底盘4个电机转矩电流削减很大，速度龟爬
        if (offline_detector.wheel_3508_state[i] == WHEEL_3508_ON || offline_detector.wheel_3508_state[i] == NOT_INIT) // 7_14训练赛复活后超功率问题：刚复活没初始化状态要计入功率预测
        {
            i_2 = limitter->motor_I[i] * limitter->motor_I[i];
            w_i = limitter->motor_w[i] * limitter->motor_I[i];
            w_2 = limitter->motor_w[i] * limitter->motor_w[i];

            sum_i_2 += i_2;
            sum_w_i += w_i;
            sum_w_2 += w_2;

            limitter->motor_a[i] = i_2 * MOTOR_R;
            limitter->motor_b[i] = w_i * MOTOR_K;
            limitter->motor_c[i] = w_2 * MOTOR_B;
            limitter->motor_P[i] = limitter->motor_a[i] + limitter->motor_b[i] + limitter->motor_c[i] + MOTOR_P0;

            // 计算总功率
            limitter->predict_send_power += limitter->motor_P[i];
        }
    }

    limitter->set_power = set_power;
    limitter->sum_i_2 = sum_i_2;
    limitter->sum_w_i = sum_w_i;
    limitter->sum_w_2 = sum_w_2;

    // 重置分配策略
    for (int i = 0; i < limitter->motor_num; i++)
    {
        limitter->power_arrange_state[i] = NOT_ARRANGE;
    }

    if (limitter->predict_send_power > set_power)
    {
        // 首先找到小于0的功率值，以及小于最小给定电机的功率值
        limitter->can_arrange_power = set_power;
        limitter->need_arrange_power = 0;
        limitter->need_arrange_w = 0;

        if (limitter->power_limit_method == TORQUE_REDUCE_METHOD)
        {
            // 首先找到小于0的功率值，以及小于最小给定电机的功率值
            for (int i = 0; i < limitter->motor_num; i++)
            {
                if (limitter->motor_b[i] <= 1e-4f) // 反向输出（速度方向与电流方向相反）情况，不作功率限制(或者电流很小的情况)
                {
                    limitter->can_arrange_power -= limitter->motor_P[i];
                    limitter->power_arrange_state[i] = NEG_ARRANGE;
                }
                else
                {
                    limitter->need_arrange_power += limitter->motor_P[i];
                    limitter->power_arrange_state[i] = NEED_ARRANGE;
                }
            }

            // 二次分配
            if (limitter->need_arrange_power >= limitter->can_arrange_power)
            {
                for (int i = 0; i < limitter->motor_num; i++)
                {
                    if (limitter->power_arrange_state[i] == NEED_ARRANGE)
                    {
                        limitter->motor_P[i] = limitter->motor_P[i] * limitter->can_arrange_power / limitter->need_arrange_power;
                    }
                }
            }
        }
        else if (limitter->power_limit_method == SPEED_ERROR_METHOD)
        {
            for (int i = 0; i < limitter->motor_num; i++)
            {
                if (limitter->motor_b[i] <= 1e-4f) // 反向输出（速度方向与电流方向相反）情况，不作功率限制(或者电流很小的情况)
                {
                    limitter->can_arrange_power -= limitter->motor_P[i];
                    limitter->power_arrange_state[i] = NEG_ARRANGE;
                }
                else
                {
                    limitter->need_arrange_power += limitter->motor_P[i];
                    limitter->need_arrange_w += limitter->motor_w_error[i];
                    limitter->power_arrange_state[i] = NEED_ARRANGE;
                }
            }

            // 二次分配，功率分配策略1：利用每个电机最大功率进行分配
            if (limitter->need_arrange_power >= limitter->can_arrange_power)
            {
                for (int i = 0; i < limitter->motor_num; i++)
                {
                    if (limitter->power_arrange_state[i] == NEED_ARRANGE)
                    {
                        limitter->motor_P[i] = limitter->motor_w_error[i] * limitter->can_arrange_power / (limitter->need_arrange_w + 1e-6f);
                    }
                }
            }
        }
    }
    else
    {
        // 不进行削减
        for (int i = 0; i < limitter->motor_num; i++)
        {
            limitter->power_arrange_state[i] = NORMAL_ARRANGE;
        }
    }

    // 计算削减系数
    float a, b, c;
    for (int i = 0; i < limitter->motor_num; i++)
    {
        if (limitter->power_arrange_state[i] == NEG_ARRANGE || limitter->power_arrange_state[i] == NORMAL_ARRANGE)
        {
            limitter->send_torque_lower_scale[i] = 1.0f;
        }
        else if (limitter->power_arrange_state[i] == NEED_ARRANGE)
        {
            a = limitter->motor_a[i];
            b = limitter->motor_b[i];
            c = limitter->motor_c[i] + MOTOR_P0 - limitter->motor_P[i];

            if (c > 0) // 无正解，转速过高，仅可以选0作为解
            {
                limitter->send_torque_lower_scale[i] = 0.0f;
                limitter->power_arrange_state[i] = ARRANGE_ERROR;
            }
            else if (fabs(a) < 1e-6f) // a为0保护
            {
                limitter->send_torque_lower_scale[i] = 0.0f;
                limitter->power_arrange_state[i] = ARRANGE_ERROR;
            }
            else
            {
                limitter->send_torque_lower_scale[i] = (-b + Sqrt(b * b - 4 * c * a)) / 2 / a;
                limitter->send_torque_lower_scale[i] = LIMIT_MAX_MIN(limitter->send_torque_lower_scale[i], 1.0f, 0.0f);
            }
        }
    }

    // 外部自行计算削减后的力矩
}

void setINAPower(PowerLimiter *limitter, float ina_power)
{
    limitter->actual_ina260_power = ina_power;
}
