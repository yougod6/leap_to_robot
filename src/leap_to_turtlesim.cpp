#include "ros/ros.h"
#include "leap_motion/Human.h"
#include "leap_motion/Hand.h"
#include "geometry_msgs/Twist.h"
#include <iostream>
#define LEAP_TO_TURTLE 5
#define MOVE_START 0.2

geometry_msgs::Twist position;
ros::Publisher tutle_pub;
ros::Subscriber leap_sub;

void messageCallback(const leap_motion::Human::ConstPtr& human){ 
    leap_motion::Hand hand;

    if( human -> left_hand.is_present )
    {
        hand = human -> left_hand;
    
        // ROS_INFO("palm_center.x = %f", hand.palm_center.x);
        // ROS_INFO("palm_center.y = %f", hand.palm_center.y);
        // ROS_INFO("palm_center.z = %f", hand.palm_center.z);
        if(abs(hand.pitch)>=MOVE_START)
            position.linear.x = -hand.pitch * LEAP_TO_TURTLE;
        else
            position.linear.x = 0;

        if(abs(hand.yaw)>=MOVE_START*2)
            position.angular.z = -hand.yaw *0.5*LEAP_TO_TURTLE;
        else   
            position.angular.z = 0;
        
        if(hand.grab_strength>=0.5)
        {
            position.linear.x = 0;
            position.angular.z = 0;
        }

        tutle_pub.publish(position);
        ROS_INFO("linear.x = %f", position.linear.x);
        ROS_INFO("angular.z = %f", position.angular.z);
    }
    // else
    // {
    //     position.linear.x = 0;
    //     position.linear.y = 0;
    //     position.angular.z =0;
    //     tutle_pub.publish(position);
    // }
}



int main(int argc, char** argv) {
    ros::init(argc, argv, "leap_to_turtlesim");
    ros::NodeHandle nh;

    leap_sub = nh.subscribe<leap_motion::Human>("/leap_motion/leap_filtered", 1, messageCallback);
    tutle_pub = nh.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel", 1);

    ros::spin();
    return 0;
}