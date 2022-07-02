#include <ros/ros.h>
#include <std_msgs/String.h>
#include <std_srvs/SetBool.h>

/*https://roboticsbackend.com/oop-with-ros-in-cpp/*/

class Sorter {

    private:
    bool flag;
    ros::Publisher pub;
    ros::Subscriber string_subscriber;
    ros::ServiceServer enabler;

    public:
    //Constructor
    Sorter(ros::NodeHandle *nh) {
        ROS_INFO("Sorter Node started");
        flag = true;
        pub = nh->advertise<std_msgs::String>("/output", 1);    
        string_subscriber = nh->subscribe("/input", 1, &Sorter::strCB, this);
        enabler = nh->advertiseService("/switch", &Sorter::srvCB, this);
    }

    //Input string callback
    void strCB(const std_msgs::String::ConstPtr& msg) {
        std::string str = msg->data.c_str();
        std_msgs::String rect_string;
        if (flag){
            //String sorting
            /*//https://www.geeksforgeeks.org/sort-string-characters/*/
            std::sort(str.begin(),str.end());
            rect_string.data = str;
            pub.publish(rect_string);
        }
        else{
            ROS_INFO("Sorter Disabled");
        }
    }

    //Service Callback
    bool srvCB(std_srvs::SetBool::Request &req, std_srvs::SetBool::Response &res)
    {
        if (req.data) {
            flag = true;
            res.success = true;
            res.message = "Sorter Enabled";
        }
        else {
            flag = false;
            res.success = false;
            res.message = "Sorter disabled";
        }

        return true;
    }
};

int main (int argc, char **argv)
{
    ros::init(argc, argv, "string_sorter");
    ros::NodeHandle nh;
    Sorter nc = Sorter(&nh);
    ros::spin();
}