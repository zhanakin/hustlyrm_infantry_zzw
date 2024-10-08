/**
 ******************************************************************************
 * @file    ChassisSolver.c
 * @brief   底盘解算器
 ******************************************************************************
 * @attention
 ******************************************************************************
 */

#include "ChassisSolver.h"

ChassisSolver chassis_solver;

void changeFricAction()
{
    if (remote_controller.shoot_action != SHOOT_POWERDOWN_MODE)
        setShootAction(SHOOT_POWERDOWN_MODE);
    else
        setShootAction(SHOOT_FIRE_MODE);
}

void changeSupplyMode()
{
    if (remote_controller.shoot_action != SHOOT_POWERDOWN_MODE)
        setShootAction(SHOOT_POWERDOWN_MODE);
    else
        setShootAction(SHOOT_SUPPLY_MODE);
}

void DJIKeyMouseUpdate(ChassisSolver *infantry)
{
    uint8_t ctrl_flag = 0;
    if (offline_detector.remote_state == REMOTE_OFF)
    {
        setAllModeOff();
        return;
    }
    // 记得设置好接口可以跳转回遥控器模式
    if (remote_controller.dji_remote.rc.s[RIGHT_SW] != Mid)
    {
        initRemoteControl(DJI_REMOTE_CONTROL);
        // setControlMode(DJI_REMOTE_CONTROL);
    }
    else
    {
        // 模式设置
        if (remote_controller.dji_remote.rc.s[LEFT_SW] == Up) // 高自由度活动
        {
            setShootAction(SHOOT_FIRE_MODE);
        }
        else if (remote_controller.dji_remote.rc.s[LEFT_SW] == Mid)
        {
            setRobotState(CONTROL_MODE);
            setControlModeAction(FOLLOW_GIMBAL);
            setShootAction(SHOOT_POWERDOWN_MODE);
            setGimbalAction(GIMBAL_ACT_MODE);

            //					  setShootAction(SHOOT_POWERDOWN_MODE);
            //            setGimbalAction(GIMBAL_ACT_MODE);

            // //速度设置
            // infantry->chassis_speed_w = 0.00f; // rad /s
        }
        else
        {
            setRobotState(CONTROL_MODE);
            setControlModeAction(FOLLOW_GIMBAL);
            setShootAction(SHOOT_POWERDOWN_MODE);
            // setGimbalAction(GIMBAL_POWERDOWN);
            setGimbalAction(GIMBAL_ACT_MODE);  // 可环顾四周
            infantry->chassis_speed_w = 0.00f; // rad /s
        }

        // 按键检测
        volatile uint16_t keyValue = remote_controller.dji_remote.keyValue;
        remote_controller.dji_remote.keyChangeOn = (remote_controller.dji_remote.last_keyValue ^ remote_controller.dji_remote.keyValue) &
                                                   remote_controller.dji_remote.keyValue;
        remote_controller.dji_remote.keyChangeOff = (remote_controller.dji_remote.last_keyValue ^ remote_controller.dji_remote.keyValue) &
                                                    remote_controller.dji_remote.last_keyValue;
        remote_controller.dji_remote.last_keyValue = keyValue;

        /* 按键操作 */

        float speed_x = 0.0f, speed_y = 0.0f;
        for (uint16_t i = 1; i > 0; i <<= 1)
        {
            uint16_t key_and = keyValue & i;
            switch (key_and) // 按键
            {
            case KEY_B:
                break;
            case KEY_V:
                break;
            case KEY_SHIFT:
                break;
            case KEY_CTRL:
                ctrl_flag = 1;
                break;
            case KEY_Q:
                break;
            case KEY_E:
                break;
            case KEY_R:
                break;
            case KEY_F:
                break;
            case KEY_G:
                break;
            case KEY_Z:
                break;
            case KEY_X:
                break;
            case KEY_C:
                break;
            case KEY_D:
                speed_y -= SPEED_MAX;
                break;
            case KEY_A:
                speed_y += SPEED_MAX;
                break;
            case KEY_S:
                speed_x -= SPEED_MAX;
                break;
            case KEY_W:
                speed_x += SPEED_MAX;
                break;
            default:
                break;
            }
            key_and = remote_controller.dji_remote.keyChangeOn & i;
            switch (key_and) // 按键上升沿
            {
            case KEY_B:
                // changeSupplyMode();
                break;
            case KEY_V:
                break;
            case KEY_SHIFT:
                setControlModeAction(CV_ROTATE);
                chassis_solver.chassis_speed_w = 6.00f; // rad /s
                break;
            case KEY_CTRL:
                break;
            case KEY_Q:
                break;

            case KEY_E:
                break;

            case KEY_R:
                break;

            case KEY_F:
                break;

            case KEY_G:
                //                SmallLegAction();
                //                setControlModeAction(JUMP_PRE_MODE);
                break;
            case KEY_Z:
                //                if (ctrl_flag) // 小符
                //                {
                //                    setGimbalAction(GIMBAL_SMALL_BUFF_MODE);
                //                }
                //                else
                //                {
                //                    setGimbalAction(GIMBAL_ACT_MODE);
                //                }

                break;
            case KEY_X:
                //                if (ctrl_flag) // 大符
                //                {
                //                    setGimbalAction(GIMBAL_BIG_BUFF_MODE);
                //                }
                //                else
                //                {
                //                    setGimbalAction(GIMBAL_ACT_MODE);
                //                }
                break;
            case KEY_C:
                // changeChassisAction();
                break;
            case KEY_D:
                break;
            case KEY_A:
                break;
            case KEY_S:
                break;
            case KEY_W:
                break;
            default:
                break;
            }
            key_and = remote_controller.dji_remote.keyChangeOff & i;

            switch (key_and) // 按键下降沿
            {
            case KEY_B:
                setControlModeAction(FOLLOW_GIMBAL);
                break;
            case KEY_V:
                break;
            case KEY_SHIFT:
                setControlModeAction(FOLLOW_GIMBAL);
                chassis_solver.chassis_speed_w = 0.00f; // rad /s
                break;
            case KEY_CTRL:
                break;
            case KEY_Q:
                break;
            case KEY_E:
                break;
            case KEY_R:
                break;
            case KEY_F:
                break;
            case KEY_G:
                setControlModeAction(FOLLOW_GIMBAL);
                break;
            case KEY_Z:
                break;
            case KEY_X:
                break;
            case KEY_C:
                break;
            case KEY_D:
                break;
            case KEY_A:
                break;
            case KEY_S:
                break;
            case KEY_W:
                break;
            default:
                break;
            }
        }

        chassis_solver.chassis_speed_x = speed_x;
        chassis_solver.chassis_speed_y = speed_y;

        // 鼠标左键检测
        volatile unsigned char press_l = remote_controller.dji_remote.mouse.press_l;
        remote_controller.dji_remote.mouse.mouseChangeOn_l = (remote_controller.dji_remote.mouse.last_press_l ^ press_l) &
                                                             press_l;
        remote_controller.dji_remote.mouse.mouseChangeOff_l = (remote_controller.dji_remote.mouse.last_press_l ^ press_l) &
                                                              remote_controller.dji_remote.mouse.last_press_l;
        remote_controller.dji_remote.mouse.last_press_l = press_l;

        // 按键操作，连发情况
        if (remote_controller.dji_remote.mouse.mouseChangeOn_l)
        {
            remote_controller.single_shoot_flag = TRUE;
        }

        volatile unsigned char press_r = remote_controller.dji_remote.mouse.press_r;
        remote_controller.dji_remote.mouse.mouseChangeOn_r = (remote_controller.dji_remote.mouse.last_press_r ^ press_r) &
                                                             press_r;
        remote_controller.dji_remote.mouse.mouseChangeOff_r = (remote_controller.dji_remote.mouse.last_press_r ^ press_r) &
                                                              remote_controller.dji_remote.mouse.last_press_r;
        remote_controller.dji_remote.mouse.last_press_r = press_r;

        // 退出辅瞄
        if (remote_controller.dji_remote.mouse.mouseChangeOff_r && remote_controller.gimbal_action == GIMBAL_AUTO_AIM_MODE)
        {
            setGimbalAction(GIMBAL_ACT_MODE);
        }
    }
}

void blueToothStateUpdate(ChassisSolver *infantry)
{
    // switch (remote_controller.blue_tooth_key)
    // {
    // case BLUE_TOOTH_UP:
    //     infantry->target_v = 0.5f;
    //     break;
    // case BLUE_TOOTH_DOWN:
    //     infantry->target_v = -0.5f;
    //     break;
    // case BLUE_TOOTH_STABLE:
    //     infantry->target_v = 0;
    //     infantry->target_yaw_v = 0;
    //     break;
    // case BLUE_TOOTH_LEFT:
    //     infantry->target_yaw_v += 0.5f;
    //     break;
    // case BLUE_TOOTH_RIGHT:
    //     infantry->target_yaw_v -= 0.5f;
    //     break;
    // default:
    //     break;
    // }
    // remote_controller.blue_tooth_key = BLUE_TOOTH_NO_ACTION;
}

void setAllModeOff()
{
    setRobotState(OFFLINE_MODE);
    setControlModeAction(NOT_CONTROL_MODE);
    setShootAction(SHOOT_POWERDOWN_MODE);
    setGimbalAction(GIMBAL_POWERDOWN);
}

void DJIRemoteUpdate(ChassisSolver *infantry)
{
    // 判断状态
    switch (remote_controller.dji_remote.rc.s[RIGHT_SW])
    {
    case Down:
        setAllModeOff();
        break;
    case Mid:
        initRemoteControl(KEY_MOUSE);
        break;
    case Up:
        switch (remote_controller.dji_remote.rc.s[LEFT_SW])
        {
        case Down:
            setAllModeOff();
            break;
        case Mid:

            // 模式设置
            setRobotState(CONTROL_MODE);
            setControlModeAction(FOLLOW_GIMBAL);
            setShootAction(SHOOT_POWERDOWN_MODE); // 先不打弹
            setGimbalAction(GIMBAL_ACT_MODE);

            // 底盘控制
            chassis_solver.chassis_speed_x = (remote_controller.dji_remote.rc.ch[RIGHT_CH_LR] - CH_MIDDLE) * SPEED_MAX / CH_RANGE;
            chassis_solver.chassis_speed_y = (remote_controller.dji_remote.rc.ch[RIGHT_CH_UD] - CH_MIDDLE) * SPEED_MAX / CH_RANGE;
            chassis_solver.chassis_speed_w = 0;

            break;
        case Up:
            // 模式设置
            setRobotState(CONTROL_MODE);
            setControlModeAction(FOLLOW_GIMBAL);
            setShootAction(SHOOT_TEST_MODE);
            setGimbalAction(GIMBAL_POWERDOWN);
            // setSuperPower(POWER_TO_BATTERY);

            // 底盘控制
            chassis_solver.chassis_speed_x = (remote_controller.dji_remote.rc.ch[RIGHT_CH_UD] - CH_MIDDLE) * SPEED_MAX / CH_RANGE;
            chassis_solver.chassis_speed_y = 0.0f;
            chassis_solver.chassis_speed_w = 0.0f;

            break;
        default:
            setAllModeOff();
            break;
        }
        break;
    default:
        setAllModeOff();
        break;
    }
    if (offline_detector.remote_state == REMOTE_OFF)
    {
        setAllModeOff();
    }
}

/**
 * @brief 根据遥控器或者蓝牙更新控制状态
 * @param[in] infantry
 */
void get_control_info(ChassisSolver *infantry)
{
    switch (remote_controller.control_type)
    {
    case BLUE_TOOTH:
        blueToothStateUpdate(infantry);
        break;
    case DJI_REMOTE_CONTROL:
        DJIRemoteUpdate(infantry);
        break;
    case KEY_MOUSE:
        DJIKeyMouseUpdate(infantry);
    default:
        break;
    }
}
