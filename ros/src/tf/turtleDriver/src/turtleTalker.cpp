#include "ros/ros.h"
#include "std_msgs/String.h"
#include <sstream>
#include "../include/turtleDriver/joystick.cpp"
#include <unistd.h>
#include <iostream>

int main(int argc, char **argv){
    ros::init(argc, argv, "talker");
    ros::NodeHandle n;
    ros::Publisher chatter_pub = n.advertise<std_msgs::String>("/input/joystick",10);
    ros::Rate loop_rate(1000);

    Joystick joystick("/dev/input/js0");

    if(!joystick.isFound())
    {
        /* TODO
         * If joystick is not found, should the system check for joystick again?
         * or should system just exit
         */
        std_msgs::String msg;
        msg.data = msg.data = "Joystick not found...";
        ROS_INFO("%s", msg.data.c_str());

        chatter_pub.publish(msg);
        ros::shutdown();
        return -1;
    }

    int count = 0;
    while(ros::ok()) {
        std_msgs::String msg;
        std::stringstream ss;
        JoystickEvent event;
        bool eventFlag = false;

        if(joystick.sample(&event)){
            int eventNumber = event.number;
            int eventValue = event.value;
            if(event.isButton()){
                ss << "Button;" << eventNumber << ";" << eventValue;
                eventFlag = true;
            }
            else if(event.isAxis()){
                ss << "Axis;" << eventNumber << ";" << eventValue;
                eventFlag = true;
            }
        }

        if(eventFlag == true){
            msg.data = ss.str();
            ROS_INFO("%s", msg.data.c_str());

            chatter_pub.publish(msg);
            eventFlag = false;
        }
        ros::spinOnce();
        loop_rate.sleep();
        ++count;
    }
    return 0;
}

