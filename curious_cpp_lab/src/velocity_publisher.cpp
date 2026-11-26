#include <chrono>
#include <memory>
#include <functional>

#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"

using namespace std::chrono_literals;

class VelocityPublisher : public rclcpp::Node
{
public:
    VelocityPublisher() : Node("velocity_publisher")
    {
        declare_parameter<double>("linear_speed", 1.0);
        get_parameter("linear_speed", linear_speed_);

        declare_parameter<double>("angular_speed", 1.0);
        get_parameter("angular_speed", angular_speed_);

        publisher_ = create_publisher<geometry_msgs::msg::Twist>("/cmd_vel", 10);
        timer_ = create_wall_timer(500ms, std::bind(&VelocityPublisher::cmd_callback, this));
    }

private:
    void cmd_callback()
    {
        geometry_msgs::msg::Twist msg;
        msg.linear.x = linear_speed_;
        msg.angular.z = angular_speed_;
        publisher_->publish(msg);
    }

    double linear_speed_;
    double angular_speed_;

    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_;
};

int main(int argc, char ** argv)
{
	rclcpp::init(argc, argv);
	auto node = std::make_shared<VelocityPublisher>();

	rclcpp::spin(node);
	rclcpp::shutdown();

	return 0;
}
