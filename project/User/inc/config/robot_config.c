#include "ChasisController.h"

void setRobotType()
{
#if ROBOT == CHEN_JING_YUAN
    infantry.chassis_type = MECANUM_WHEEL;
    infantry.yaw_motor_type = YAW_GM6020;
    infantry.chassis_follow_type = TWO_SIDES_FOLLOW;
    infantry.power_limit_method = SPEED_ERROR_METHOD;
#elif ROBOT == NIUNIU
    infantry.chassis_type = OMNI_WHEEL;
    infantry.yaw_motor_type = YAW_DM_MOTOR;
    infantry.chassis_follow_type = TWO_SIDES_LEFT_RIGHT;
    infantry.power_limit_method = SPEED_ERROR_METHOD;
#elif ROBOT == NIU_MO_SON
    infantry.chassis_type = STEER_WHEEL;
    infantry.yaw_motor_type = YAW_DM_MOTOR;
    infantry.chassis_follow_type = TWO_SIDES_FOLLOW;
    infantry.power_limit_method = TORQUE_REDUCE_METHOD;
#elif ROBOT == QI_TIAN_DA_SHENG
    infantry.chassis_type = OMNI_WHEEL;
    infantry.yaw_motor_type = YAW_DM_MOTOR;
    infantry.chassis_follow_type = FOUR_SIDES_FOLLOW;
    infantry.power_limit_method = SPEED_ERROR_METHOD;
#endif

    if (infantry.chassis_type == MECANUM_WHEEL)
    {
        mecanum_pid_init();
    }
    else if (infantry.chassis_type == OMNI_WHEEL)
    {
        omni_pid_init();
    }
    else if (infantry.chassis_type == STEER_WHEEL)
    {
        steer_pid_init();
    }
}
