// 遥控器选择
#ifndef _REMOTE_CONTROL_H
#define _REMOTE_CONTROL_H

#include <string.h>
#include <stdint.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "counter.h"
#include "debug.h"

enum CONTROL_TYPE
{
    DJI_REMOTE_CONTROL, // DJI遥控器
    BLUE_TOOTH,         // 蓝牙控制
    KEY_MOUSE,          // 键鼠
};
enum ROBOT_STATE
{
    OFFLINE_MODE,
    CONTROL_MODE,
};
enum CONTROL_MODE_ACTION
{
    NOT_CONTROL_MODE,
    NOT_FOLLOW_GIMBAL,   // 仅底盘运动模式
    FOLLOW_GIMBAL,       // 云台跟随模式
    CV_ROTATE,           // 恒速度旋转
    CHANGE_SPEED_FOLLOW, // 变速旋转
};

enum PowerControlState // 功率控制状态
{
    POWER_TO_BATTERY,    // 接电源
    POWER_TO_SuperPower, // 接电容
};

enum FlyControlState // 功率控制状态
{
    NOT_FLY, // 不飞
    IS_FLY,  // 飞坡
};

/* 异常状态检测以及处理 */
typedef enum AbnormalState
{
    ROBOT_NORMAL,
} AbnormalState;

/* 底盘腿运动状态 */
enum LEG_ACTION
{
    NOT_STRETCH, // 原始状态下的不伸腿
    RAW_LENGTH,  // 正常运动时的小幅度伸腿
    HIGH_LENGTH, // 伸腿
    LEAN_LEFT,   // 向左倾斜
    LEAN_RIGHT   // 向右倾斜
};

enum GIMBAL_ACTION
{
    GIMBAL_POWERDOWN, // 云台掉电模式
    GIMBAL_ACT_MODE,
    GIMBAL_AUTO_AIM_MODE,
    GIMBAL_TEST_MODE,
    GIMBAL_SI_MODE,         // 云台系统辨识模式
    GIMBAL_SMALL_BUFF_MODE, // 打符模式
    GIMBAL_BIG_BUFF_MODE,   // 大符
};

enum SHOOT_ACTION
{
    SHOOT_POWERDOWN_MODE, // 掉电模式
    SHOOT_CHECK_MODE,     // 自检模式
    SHOOT_TEST_MODE,      // 弹道测试模式
    SHOOT_FIRE_MODE,      // 射击模式
    SHOOT_AUTO_AIM_MODE,  // 自瞄模式
    SHOOT_SUPPLY_MODE     // 补给模式
};

enum CHASSIS_FORMAT
{
    X_MODE,    // X型
    CROSS_MODE // 十字型
};

enum BOMB_BAY_STATE
{
    BOMB_BAY_OFF,   // 弹舱盖关闭 或 未初始化
    BOMB_BAY_ON,    // 弹舱盖开启
};


enum BLUE_TOOTH_KEY
{
    BLUE_TOOTH_OFFLINE_MODE = 0,
    BLUE_TOOTH_CONTROL_MODE = 1,
    BLUE_TOOTH_ROTATE_MODE = 2,
    BLUE_TOOTH_UP = 10,
    BLUE_TOOTH_LEFT = 11,
    BLUE_TOOTH_STABLE = 12,
    BLUE_TOOTH_RIGHT = 13,
    BLUE_TOOTH_DOWN = 14,
    BLUE_TOOTH_NO_ACTION = -1
};

#pragma pack(1)
/*遥控器结构体*/
typedef struct
{
    unsigned short ch[4]; // 摇杆
    unsigned short s[2];  // 拨杆
    unsigned short poke;  // 拨轮（左上角）
} Remote;

#define CH_MIDDLE 1024
#define CH_RANGE 660 // 范围为CH_MIDDLE - CH_RANGE ~ CH_MIDDLE + CH_RANGE

// 拨杆位置
enum SWPos
{
    Lost = 0,
    Up = 1,
    Mid = 3,
    Down = 2
};
enum WhichSW
{
    LEFT_SW = 1,
    RIGHT_SW = 0
};
enum WhichCH
{
    RIGHT_CH_LR = 0, // 右杆左右方向
    RIGHT_CH_UD = 1, // 右杆上下方向
    LEFT_CH_LR = 2,  // 左杆左右方向
    LEFT_CH_UD = 3,  // 左杆上下方向
};
/*鼠标结构体*/
typedef struct
{
    short x;
    short y;
    short z;
    unsigned char press_l;
    unsigned char press_r;

    // 按键检测
    unsigned char last_press_l;
    unsigned char last_press_r;
    unsigned char mouseChangeOn_l;  // 检测按键值从0转1改变
    unsigned char mouseChangeOff_l; // 检测按键值从1转0转变
    unsigned char mouseChangeOn_r;  // 检测按键值从0转1改变
    unsigned char mouseChangeOff_r; // 检测按键值从1转0转变
} Mouse;

#define KEY_B 0x8000
#define KEY_V 0x4000
#define KEY_C 0x2000
#define KEY_X 0x1000
#define KEY_Z 0x0800
#define KEY_G 0x0400
#define KEY_F 0x0200
#define KEY_R 0x0100
#define KEY_E 0x0080
#define KEY_Q 0x0040
#define KEY_CTRL 0x0020
#define KEY_SHIFT 0x0010
#define KEY_D 0x0008
#define KEY_A 0x0004
#define KEY_S 0x0002
#define KEY_W 0x0001

/*遥键鼠结构体综合*/
typedef struct
{
    Remote rc;
    Mouse mouse;
    uint16_t keyValue;

    // 以下是检测按键触发状态变量
    uint16_t last_keyValue;
    uint16_t keyChangeOn;  // 检测按键值从0转1改变
    uint16_t keyChangeOff; // 检测按键值从1转0转变
} RC_Ctl_t;

#pragma pack()

typedef struct RemoteController
{
    enum CONTROL_TYPE control_type; // 控制类型(遥控器，蓝牙等)
    enum ROBOT_STATE robot_state;   // 机器人状态(掉线模式，控制模式)
    enum ROBOT_STATE last_robot_state;
    enum GIMBAL_ACTION gimbal_action; // 云台模式
    enum GIMBAL_ACTION last_gimbal_action;
    enum SHOOT_ACTION shoot_action; // 打弹模式
    enum SHOOT_ACTION last_shoot_action;
    enum CONTROL_MODE_ACTION last_control_mode_action; // 底盘模式
    enum CONTROL_MODE_ACTION control_mode_action;
    enum PowerControlState super_power_state;	//主动电容标志位
    enum FlyControlState fly_state;	//飞坡大功率标志位
    enum CHASSIS_FORMAT chassis_format;	//全向轮底盘正交与否
		enum BOMB_BAY_STATE bomb_bay;	//弹舱盖开启与否

    enum BLUE_TOOTH_KEY blue_tooth_key;
    RC_Ctl_t dji_remote;

    /* 标志位 */
    int8_t reverse_flag;      // 拨盘反拨标志位
    int8_t single_shoot_flag; // 仅单发时有效，打击标志位

} RemoteController;

void setGimbalAction(enum GIMBAL_ACTION action);
void setShootAction(enum SHOOT_ACTION action);

void setControlMode(enum CONTROL_TYPE type);
void setRobotState(enum ROBOT_STATE state);
void setControlModeAction(enum CONTROL_MODE_ACTION action);
void initRemoteControl(enum CONTROL_TYPE type);
void setSuperPower(enum PowerControlState super_power_state);
void setFlyMode(enum FlyControlState fly_state);
void setChassisFormat(enum CHASSIS_FORMAT chassis_format);
void setBombBayState(enum BOMB_BAY_STATE bay_state);

// 蓝牙
void Blue_Tooth_Deal(uint16_t *);

// 遥控器
void RemoteReceive(volatile unsigned char rx_buffer[]);

void RC_Rst(void);

extern RemoteController remote_controller;

#endif // !_REMOTE_CONTROL_H
