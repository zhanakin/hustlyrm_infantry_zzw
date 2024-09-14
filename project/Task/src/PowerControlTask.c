#include "PowerControlTask.h"
float Interval;
uint32_t timtim;
void PowerControlTask(void *pvParameters)
{
	portTickType xLastWakeTime;

	static int i = 0;

	CapControllerInit();

	float referee_power;

	while (1)
	{
		xLastWakeTime = xTaskGetTickCount();

		// TODO:首先进行异常处理，万一不能收到裁判系统数据或者裁判系统数据离线
		referee_power = LIMIT_MAX_MIN(referee_data.Game_Robot_State.chassis_power_limit, 300, 40);

		if (remote_controller.fly_state == IS_FLY)
		{
			NingCapControl(referee_data.Power_Heat_Data.buffer_energy, referee_power, 300.0f);//全向轮300W飞坡姿态良好
		}
		else if (remote_controller.super_power_state == POWER_TO_SuperPower)
		{
			NingCapControl(referee_data.Power_Heat_Data.buffer_energy, referee_power, referee_power + 50.0f);
		}
		else if(gimbal_receiver_pack1.through_hole_flag)
		{
			NingCapControl(referee_data.Power_Heat_Data.buffer_energy, referee_power, 45.0f);
		}
		else
		{
			NingCapControl(referee_data.Power_Heat_Data.buffer_energy, referee_power, referee_power);
		}

		// 测试
		// NingCapControl(referee_data.Power_Heat_Data.buffer_energy, referee_power, 100.0f);

		if (i % 4 == 0) // 250HZ
		{
			SendCapPack(&cap_send_data, cap_controller.cap_power);
			Interval = GetDeltaT(&timtim);
			CanSend(SUPER_POWER_CAN, (int8_t *)(&cap_send_data), SEND_TO_SUPER_POWER_CAN_ID, 8);
		}
		
		i++;

		vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(1));
	}
}
