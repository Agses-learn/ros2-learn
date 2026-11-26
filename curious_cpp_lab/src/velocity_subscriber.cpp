#include <chrono>
#include <memory>
#include <functional>

#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"

using namespace std::chrono_literals;

class VelocitySubscriber : public rclcpp::Node
{
public:
    VelocitySubscriber() : Node("velocity_subscriber")
    {
        subscriber_ = create_subscription<geometry_msgs::msg::Twist>("/cmd_vel", 10, std::bind(&VelocitySubscriber::sub_callback, this, std::placeholders::_1));
    }

private:
    void sub_callback(const geometry_msgs::msg::Twist::SharedPtr msg)
    {
	RCLCPP_INFO(get_logger(), "linear speed: %.2f, angular speed: %.2f", msg->linear.x, msg->angular.z);
    }

    rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr subscriber_;
};

int main(int argc, char ** argv)
{
	rclcpp::init(argc, argv);

	auto node = std::make_shared<VelocitySubscriber>();

	rclcpp::spin(node);
	rclcpp::shutdown();

	return 0;
}
