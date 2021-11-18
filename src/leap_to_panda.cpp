#include "ros/ros.h"
#include "leap_motion/Human.h"
#include "leap_motion/Hand.h"
#include "leap_motion/Gesture.h"
#include "geometry_msgs/Twist.h"
#include "leap_to_robot/leap_to_panda.h" //service message to panda
#include <iostream>

ros::Subscriber leap_sub;

int type;
int size;

void messageCallback(const leap_motion::Human::ConstPtr& human){ 
    leap_motion::Hand hand;
    if( human -> left_hand.is_present )
    {
    std::cout << size << std::endl;
        
        hand = human -> left_hand;
        size = hand.gesture_list.size();

        // gesture is recognizig and load to gesture_list[]
        if(size>0)
            type = hand.gesture_list[0].gesture_type;
            //state = hand.gesture_list[0].gesture_state;
    }
}
 



int main(int argc, char** argv) {
    ros::init(argc, argv, "leap_to_panda");
    ros::NodeHandle nh;
    leap_sub = nh.subscribe<leap_motion::Human>("/leap_motion/leap_filtered", 1, messageCallback);

    ros::ServiceClient client = nh.serviceClient<leap_to_robot::leap_to_panda>("moveit_leap_server");
    leap_to_robot::leap_to_panda service_msg;
    
    while(ros::ok()){
    std::cout<<"Type : ";
    service_msg.request.gesture_type =type;
    ROS_INFO("TRY TO REQUEST...");

    if(client.call(service_msg))
        ROS_INFO("%s",service_msg.response.feedback.c_str());
    else
    	ROS_ERROR("SERVER ERROR");
  
    ros::spin();
    
    return 0;
}
}