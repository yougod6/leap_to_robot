#include "ros/ros.h"
//#include "Leap.h"
//#include "lmc_listener.h"
#include "leap_motion/Human.h"
#include "leap_motion/Hand.h"
#include "leap_motion/Gesture.h"
#include "geometry_msgs/Twist.h"
#include <iostream>

geometry_msgs::Twist position;
ros::Publisher tutle_pub;
ros::Subscriber leap_sub;

// bool circle_enabled;
// bool swipe_enabled;
// bool screen_tap_enabled;
// bool key_tap_enabled;
int type;
int state;
int size;

void messageCallback(const leap_motion::Human::ConstPtr& human){ 
    leap_motion::Hand hand;
    // Leap::Controller() Controller;
    // Controller.enableGesture(TYPE_CIRCLE);
    // bool check_circle =controller.isGestureEnabled(TYPE_CIRCLE);

    //bool Leap::Controller::isGestureEnabled	(TYPE_CIRCLE) const check_circle;


    if( human -> left_hand.is_present )
    {
    std::cout << size << std::endl;
        
        
        //std::cout<<"recognize !!"<<std::endl;
        hand = human -> left_hand;
        size = hand.gesture_list.size();
        state=3;

        // Draw circle
        // check = hand.gesture_list[0].gesture_type;
        // std::cout<<check<<std::endl;

        // gesture is recognizig and load to gesture_list[]
        if(size>0){
            type = hand.gesture_list[0].gesture_type;
            state = hand.gesture_list[0].gesture_state;
            //Draw circle
            // if(type==4) //TYPE_CIRCLE=4
            // {   
            // position.linear.x=4;
            // position.linear.y=0;
            // position.angular.z =4;
            // }
            //Draw horizontal line.
            if(type==1)//TYPE_SWIPE=1
            {   
            position.linear.x=0;
            position.linear.y=-2;
            position.angular.z =0;
            }
            //Draw vertical line
            else if(type==5)//TYPE_SCREEN_TAP=5
            {   
            position.linear.x=2;
            position.linear.y=0;
            position.angular.z =0;
            }
            //Spin 
            else if(type==6)//TYPE_KEY_TAP=6
            {   
            position.linear.x=0;
            position.linear.y=0;
            position.angular.z =2;
            }
            else
            {
            position.linear.x=0;
            position.linear.y=0;
            position.angular.z =0;  
            }
        tutle_pub.publish(position);

        }
 

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
    ros::init(argc, argv, "leap_to_turtlesim_gesture");
    ros::NodeHandle nh;


    // nh.getParam("/enable_gesture_circle",circle_enabled);
    // nh.getParam("/enable_gesture_swipe",swipe_enabled);
    // nh.getParam("/enable_gesture_screen_tap",screen_tap_enabled);
    // nh.getParam("/enable_gesture_key_tap",key_tap_enabled);

  
    leap_sub = nh.subscribe<leap_motion::Human>("/leap_motion/leap_filtered", 1, messageCallback);
    tutle_pub = nh.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel", 1);
    ros::spin();
    
    return 0;
}