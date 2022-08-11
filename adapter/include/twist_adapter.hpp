#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "geometry_msgs/TwistStamped.h"
#include "std_msgs/UInt8.h"
#include "std_msgs/Float64.h"

#include <prius_msgs/Control.h>


class Twist_adapter
{
private:
    ros::NodeHandle n;
    ros::NodeHandle private_nh;

    bool forward;
    bool to_brake;
    int count, interval;
    

    std::string name, topic;
    geometry_msgs::Twist current_state;
    float max_throttle_linear_diff, max_steering_angular_diff;
    float linear_multiplier, angular_multiplier;

    int current_gear = 2;

    // ros::Publisher throttle_pub, steering_pub, brake_pub, gear_pub;
    ros::Publisher control_pub;
    ros::Subscriber cmd_vel_sub, current_twist_sub;

    void twist_callback(const geometry_msgs::Twist::ConstPtr& cmd);
    void current_twist_callback(const geometry_msgs::Twist::ConstPtr& twist);


public:
    Twist_adapter();
    void pub();

};