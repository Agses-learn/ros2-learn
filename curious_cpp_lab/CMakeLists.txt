cmake_minimum_required(VERSION 3.8)

project(curious_cpp_lab)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(rclcpp REQUIRED)
find_package(geometry_msgs REQUIRED)

add_executable(
    velocity_publisher
    src/velocity_publisher.cpp
)

add_executable(
    velocity_subscriber
    src/velocity_subscriber.cpp
)
ament_target_dependencies(
    velocity_publisher
    rclcpp
    geometry_msgs
)

ament_target_dependencies(
    velocity_subscriber
    rclcpp
    geometry_msgs
)

install(
    TARGETS velocity_publisher
            velocity_subscriber
    DESTINATION lib/${PROJECT_NAME}
)

ament_package()
