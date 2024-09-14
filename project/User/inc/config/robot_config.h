#ifndef _ROBOT_CONFIG_H
#define _ROBOT_CONFIG_H

/*  机器人宏定义列举 */
#define NIU_MO_SON 0       // 舵轮
#define CHEN_JING_YUAN 1   // 麦轮
#define NIUNIU 2           // 2024.6.16第一辆全向轮
#define QI_TIAN_DA_SHENG 3 // 2024.7.23第二辆全向轮

#define ROBOT QI_TIAN_DA_SHENG 

typedef enum CHASSIS_TYPE
{
    STEER_WHEEL,   // 舵轮
    MECANUM_WHEEL, // 麦克纳姆轮
    OMNI_WHEEL,    // 全向轮
} CHASSIS_TYPE;

// 麦轮参数
#define MECANUM_WIDTH 0.15f  // 麦轮宽
#define MECANUM_LENGTH 0.20f // 麦轮长

// yaw轴电机类型
typedef enum YAW_MOTOR_TYPE
{
    YAW_GM6020,
    YAW_DM_MOTOR
} YAW_MOTOR_TYPE;

#if ROBOT == CHEN_JING_YUAN
#define GIMBAL_FOLLOW_ZERO 2007 // 底盘跟随机械零点
#define GIMBAL_MOTOR_SIGN -1    // 云台电机方向，以逆时针为正
#elif ROBOT == NIU_MO_SON
#define GIMBAL_FOLLOW_ZERO 9899 // 底盘跟随机械零点
#define GIMBAL_MOTOR_SIGN 1     // 云台电机方向，以逆时针为正
#elif ROBOT == NIUNIU
#define GIMBAL_MOTOR_SIGN 1      // 云台电机方向，以逆时针为正
#define GIMBAL_FOLLOW_ZERO 30383 // 底盘跟随机械零点//1\2号轮中间
#elif ROBOT == QI_TIAN_DA_SHENG
#define GIMBAL_MOTOR_SIGN 1     // 云台电机方向，以逆时针为正
#define GIMBAL_FOLLOW_ZERO 3340 // 底盘跟随机械零点
#endif
void setRobotType(void);

#endif
