#include "GimbalTask.h"

int8_t send_to_gimbal_data[8];

/*
 * @brief 打包发送给云台的数据（热量限制标志位，机器人颜色、等级、弹速上限、buff状态，弹速）
 * @param[in] void
 */
void SendToGimbalPack()
{
    GimbalSendPack();
    memcpy(send_to_gimbal_data, &gimbal_pack_send_1, 8);
}

/*
 * @brief 云台控制任务
 * @param[in] void
 */
void GimbalTask(void *pvParameters)
{
    //声明一个变量用于记录上一次任务唤醒的时间
    portTickType xLastWakeTime;

    //使任务在开始时延迟 1000 毫秒（1 秒）。这是为了确保任务在初次运行时不会立即执行后续代码
    vTaskDelay(pdMS_TO_TICKS(1000));

    while (1)
    {
        //获取当前系统的时间戳，用于后续的延时计算
        xLastWakeTime = xTaskGetTickCount();

        HeatUpdate();           //热量更新
        SendToGimbalPack();     //打包更新的数据

        //参数：CAN 总线标识、数据内容、CAN 消息 ID 和数据长度（8 字节）
        CanSend(GIMBAL_CAN_COMM_CANx, send_to_gimbal_data, SEND_TO_GIMBAL_CAN_ID_1, 8); 

        //设置事件组的标志位。这通常用于通知其他任务或系统某个事件发生。xCreatedEventGroup 是事件组的句柄，GIMBAL_TASK_BIT 是事件标志位
        xEventGroupSetBits(xCreatedEventGroup, GIMBAL_TASK_BIT); // 标志位置一

        /*  延时  */
        //延迟任务的下一个周期
        //vTaskDelayUntil 是一个用于周期性延迟的函数，它会使任务延迟指定的时间（这里是 5 毫秒），并且任务将以相对固定的周期运行
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(5));
    }
}
