#include <ros/ros.h>
#include <geometry_msgs/Point.h>

namespace PositionData
{
    geometry_msgs::Point right_value;
    geometry_msgs::Point left_value;
}

class Positions
{
public:
    explicit Positions(ros::NodeHandle node_handle)
    {
        right_position_sub = node_handle.subscribe("/right_position", 10, &Positions::right_position_callback, this);
        left_position_sub = node_handle.subscribe("/left_position", 10, &Positions::left_position_callback, this);
    }

    void right_position_callback(const geometry_msgs::Point::ConstPtr& right_position_msg)
    {
        PositionData::right_value.x = right_position_msg->x;
        PositionData::right_value.y = right_position_msg->y;
    }

    void left_position_callback(const geometry_msgs::Point::ConstPtr& left_position_msg)
    {
        PositionData::left_value.x = left_position_msg->x;
        PositionData::left_value.y = left_position_msg->y;
    }
private:
    ros::Subscriber right_position_sub;
    ros::Subscriber left_position_sub;
};

class Average
{
public:
    Average();

    void spin();

private:
    ros::NodeHandle nh_;

    ros::Publisher average_value_pub_;
};

Average::Average() : nh_()
{
    average_value_pub_ = nh_.advertise<geometry_msgs::Point>("average_value", 10);
}

void Average::spin()
{
    Positions positions(nh_);
    geometry_msgs::Point average_value;

    while (ros::ok())
    {
        ros::spinOnce();
        average_value.x = (PositionData::right_value.x + PositionData::left_value.x);
        average_value.y = (PositionData::right_value.y + PositionData::left_value.y);
        average_value.z = 0;

        average_value_pub_.publish(average_value); 
    }
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "average_value_publisher");
    Average average;
    average.spin();
    return 0;
}