#include "GimbalSend.h"

GimbalSendPack_1 gimbal_pack_send_1;

void GimbalSendPack()
{
  float bullet_spd_100 = referee_data.Shoot_Data.bullet_speed * 100;    //获取实时射击信息（弹速）

  gimbal_pack_send_1.is_shootable = heat_controller.shoot_flag;         //热量限制标志位
  gimbal_pack_send_1.robot_color = referee_data.Game_Robot_State.robot_id < 10 ? 1 : 0; //机器人颜色
  gimbal_pack_send_1.robot_level = referee_data.Game_Robot_State.robot_level;           //机器人等级
  //子弹速度上限
  gimbal_pack_send_1.bullet_level = 2; // referee_data.Game_Robot_State.shooter_id1_17mm_speed_limit <= 15 ? 0 : (referee_data.Game_Robot_State.shooter_id1_17mm_speed_limit <= 22 ? 1 : 2);
  //机器人获得buff状态
  gimbal_pack_send_1.buff_state = 0;   // referee_data.Buff_Musk.power_rune_buff & 0x0F;
  //弹速
  gimbal_pack_send_1.bullet_speed = (uint16_t)bullet_spd_100;
}
