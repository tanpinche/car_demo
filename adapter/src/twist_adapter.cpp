#include "twist_adapter.hpp"
#include <cmath>

Twist_adapter::Twist_adapter(){

  if(!private_nh.getParam("max_throttle_linear_diff", max_throttle_linear_diff)){
    max_throttle_linear_diff = 2;
  }
  if(!private_nh.getParam("max_steering_angular_diff", max_steering_angular_diff)){
    max_steering_angular_diff = 1;
  }

  if(!private_nh.getParam("linear_multiplier", linear_multiplier)){
    linear_multiplier = 1;
  }
  if(!private_nh.getParam("angular_multiplier", angular_multiplier)){
    angular_multiplier = 1;
  }


  current_gear = 0;



  // throttle_pub=n.advertise<std_msgs::Float64>("/throttle_cmd", 2);
  // steering_pub=n.advertise<std_msgs::Float64>("/steering_cmd", 2);
  // brake_pub = n.advertise<std_msgs::Float64>("/brake_cmd", 2);
  // gear_pub =n.advertise<std_msgs::UInt8>("/gear_cmd", 2);
  control_pub = n.advertise<prius_msgs::Control>("/prius",2);
  cmd_vel_sub=n.subscribe("/cmd_vel",1,&Twist_adapter::twist_callback,this);
  current_twist_sub=n.subscribe("/prius/current_twist",1,&Twist_adapter::current_twist_callback,this);
  
}

void Twist_adapter::current_twist_callback(const geometry_msgs::Twist::ConstPtr& twist){
  
  if (abs(twist->linear.x)< 0.05)
    current_state.linear.x = 0;
    else
      current_state.linear.x = twist->linear.x ;
  if(abs(twist->angular.z)<0.05)
    current_state.angular.z= 0;
    else 
      current_state.angular.z= twist->angular.z;


}

void Twist_adapter::twist_callback(const geometry_msgs::Twist::ConstPtr& cmd){

  auto cmdx = 5.0*cmd->linear.x;
  //printf("\ncmd:%f cmdx:%f",cmd->linear.x, cmdx); 
  auto currentx = current_state.linear.x;
  int gear;
  prius_msgs::Control control;


  if(cmdx<0){
    gear = 3;
  }
  else if (cmdx>0)
  gear = 2;
  else if (cmdx == 0)
  gear = 1;

  if(gear != current_gear ){
    control.shift_gears = gear;
    current_gear = gear;

  }
  else
  control.shift_gears = 0;


 
  auto linear_diff = abs(cmdx) - abs(currentx);
  // printf("\ndiff:%f",linear_diff);

  
  if(linear_diff >max_throttle_linear_diff){
    control.throttle =1.0;
    control.brake =0.0;
  
  }else if(linear_diff>0){
    control.throttle = linear_diff/max_throttle_linear_diff;
    control.brake = 0.0;
  }
  else if (linear_diff<max_throttle_linear_diff*(-1.0)){
    control.throttle = 0.0;
    control.brake = 1.0;
  } 
  else {
    control.throttle = 0.0;
    control.brake = abs(linear_diff/max_throttle_linear_diff) ;
  }
  if (cmdx == 0){
    control.brake = 1.0;
  }
  auto angular_diff = cmd->angular.z- current_state.angular.z;
  control.steer = angular_diff/max_steering_angular_diff * 2;



  control_pub.publish(control);

  // gear_pub.publish(gear);
  // steering_pub.publish(steering);
  // brake_pub.publish(brake);
  // throttle_pub.publish(throttle);
  

};

int main(int argc, char **argv)
{
  ros::init(argc, argv, "twist adapter");
  Twist_adapter adapter;

  ros::spin();
  return 0;
}