# 1. ros2环境安装

## 1.1 colcon构建工具安装
sudo apy install python3-colcon-common-extensions

工作空间下colcon build #编译

## 1.2 第一个程序实现

### 1.2.1 创建功能包(src/下)
ros2 pkg create hello_world_cpp --build-type ament_cmake --dependencies rclcpp --noode-name helloworld

--rclcpp：ros2 client
--noode-name：设置可执行程序

### 1.2.2 编辑源文件
#include "rclcpp/rclcpp.hpp"

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);

    auto node = rclcpp::Node::make_shared("hello_world_cpp");
    RCLCPP_INFO(node->get_logger(), "hello world!");
    rclcpp::shutdown();
    return 0;
}

### 1.2.3 编辑配置文件

### 1.2.4 编译
colcon build

### 1.2.5 执行
. install/setup.bash  #刷新环境变量
ros2 run hello_world_cpp helloworld

## 1.3 ros2体系框架
ros = 通信 + 工具 + 功能 + 社区

### 1.3.1 ros2文件系统
可以划分为3层：操作系统层  中间层  应用层

--操作系统层指ros2需要运行在windows/linux/rtos操作系统上

--中间层主要由数据分发服务DDS与ros2封装的关于机器人开发的中间件组成，ros2中间件主要由客户端库 + DDS抽象层 + 进程内通讯API 组成

--应用层指开发者构建的应用程序，在功能包中可以包含源码、数据定义、接口等内容

#### 1.3.1.1 概览
功能包是ROS2应用程序的核心，但是功能包不能直接构建，必须依赖于工作空间，一个ROS2工作空间的目录结构如下：

WorkSpace --- 自定义的工作空间。
    |--- build：存储中间文件的目录，该目录下会为每一个功能包创建一个单独子目录。
    |--- install：安装目录，该目录下会为每一个功能包创建一个单独子目录。
    |--- log：日志目录，用于存储日志文件。
    |--- src：用于存储功能包源码的目录。
        |-- C++功能包
            |-- package.xml：包信息，比如:包名、版本、作者、依赖项。
            |-- CMakeLists.txt：配置编译规则，比如源文件、依赖项、目标文件。
            |-- src：C++源文件目录。
            |-- include：头文件目录。
            |-- msg：消息接口文件目录。
            |-- srv：服务接口文件目录。
            |-- action：动作接口文件目录。
        |-- Python功能包
            |-- package.xml：包信息，比如:包名、版本、作者、依赖项。
            |-- setup.py：与C++功能包的CMakeLists.txt类似。
            |-- setup.cfg：功能包基本配置文件。
            |-- resource：资源目录。
            |-- test：存储测试相关文件。
            |-- 功能包同名目录：Python源文件目录。

另外，无论是Python功能包还是C++功能包，都可以自定义一些配置文件相关的目录。

|-- C++或Python功能包
    |-- launch：存储launch文件。
    |-- rviz：存储rviz2配置相关文件。
    |-- urdf：存储机器人建模文件。
    |-- params：存储参数文件。
    |-- world：存储仿真环境相关文件。
    |-- map：存储导航所需地图文件。
    |-- ......

#### 1.3.1.2 源文件说明
在ROS2中，推荐以继承Node的方式来创建节点对象。

C++继承Node实现示例如下：

#include "rclcpp/rclcpp.hpp"

class MyNode: public rclcpp::Node{
public:
    MyNode():Node("node_name"){
        RCLCPP_INFO(this->get_logger(),"hello world!");
    }

};

int main(int argc, char *argv[])
{
    rclcpp::init(argc,argv);
    auto node = std::make_shared<MyNode>();
    rclcpp::shutdown();
    return 0;
}
Python继承Node实现示例如下：

import rclpy
from rclpy.node import Node

class MyNode(Node):
    def __init__(self):
        super().__init__("node_name_py")
        self.get_logger().info("hello world!")
def main():

    rclpy.init()
    node = MyNode() 
    rclpy.shutdown()

之所以继承比直接实例化Node更被推荐，是因为继承方式可以在一个进程内组织多个节点，这对于提高节点间的通信效率是很有帮助的，但是直接实例化则与该功能不兼容。

#### 1.3.1.3 配置文件说明
1.package.xml
不管是何种类型的功能包，package.xml的格式都是类似的，在该文件中包含了包名、版本、作者、依赖项的信息，package.xml可以为colcon构建工具确定功能包的编译顺序。一个简单的package.xml示例如下：

<?xml version="1.0"?>
<?xml-model href="http://download.ros.org/schema/package_format3.xsd" schematypens="http://www.w3.org/2001/XMLSchema"?>
<package format="3">
  <name>pkg01_helloworld_cpp</name>
  <version>0.0.0</version>
  <description>TODO: Package description</description>
  <maintainer email="ros2@todo.todo">ros2</maintainer>
  <license>TODO: License declaration</license>

  <buildtool_depend>ament_cmake</buildtool_depend>
  <depend>rclcpp</depend>

  <test_depend>ament_lint_auto</test_depend>
  <test_depend>ament_lint_common</test_depend>

  <export>
    <build_type>ament_cmake</build_type>
  </export>
</package>
1.根标签

<package>：该标签为整个xml文件的根标签，format属性用来声明文件的格式版本。
2.元信息标签

<name>：包名；
<version>：包的版本号；
<description>：包的描述信息；
<maintainer>：维护者信息；
<license>：软件协议；
<url>：包的介绍网址；
<author>：包的作者信息。
3.依赖项

<buildtool_depend>：声明编译工具依赖；
<build_depend>：声明编译依赖；
<build_export_depend>：声明根据此包构建库所需依赖；
<exec_depend>：声明执行时依赖；
<depend>：相当于<build_depend>、<build_export_depend>、<exec_depend>三者的集成；
<test_depend>：声明测试依赖；
<doc_depend>：声明构建文档依赖。
2.CMakeLists.txt
C++功能包中需要配置CMakeLists.txt文件，该文件描述了如何构建C++功能包，一个简单的CMakeLists.txt示例如下：

声明cmake的最低版本
cmake_minimum_required(VERSION 3.8)
包名，需要与package.xml中的包名一致
project(pkg01_helloworld_cpp)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find dependencies
find_package(ament_cmake REQUIRED)
引入外部依赖包
find_package(rclcpp REQUIRED)

映射源文件与可执行文件
add_executable(helloworld src/helloworld.cpp)
设置目标依赖库
ament_target_dependencies(
  helloworld
  "rclcpp"
)
定义安装规则
install(TARGETS helloworld
  DESTINATION lib/${PROJECT_NAME})

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  // the following line skips the linter which checks for copyrights
  // comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  // the following line skips cpplint (only works in a git repo)
  // comment the line when this package is in a git repo and when
  // a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
在示例中关于文件的使用已经通过注释给出了简短说明，其实关于CMakeLists.txt的配置是比较复杂的，后续随着学习的深入，还会给出更多的补充说明。

3.setup.py
Python功能包中需要配置setup.py文件，该文件描述了如何构建Python功能包，一个简单的setup.py示例如下：

from setuptools import setup

package_name = 'pkg02_helloworld_py'

setup(
    name=package_name, # 包名
    version='0.0.0',   # 版本
    packages=[package_name], # 功能包列表
    data_files=[ #需要被安装的文件以及安装路径
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
    ],
    install_requires=['setuptools'], # 安装依赖
    zip_safe=True,
    maintainer='ros2', # 维护者
    maintainer_email='ros2@todo.todo', # 维护者 email
    description='TODO: Package description', # 包描述
    license='TODO: License declaration', # 软件协议
    tests_require=['pytest'], # 测试依赖
    entry_points={
        'console_scripts': [
            # 映射源文件与可执行文件
            'helloworld = pkg02_helloworld_py.helloworld:main'
        ],
    },
)
使用语法可参考上述示例中的注释。

#### 1.3.1.4 操作命令
1.创建
新建功能包语法如下：

ros2 pkg create 包名 --build-type 构建类型 --dependencies 依赖列表 --node-name 可执行程序名称
格式解释：

--build-type：是指功能包的构建类型，有cmake、ament_cmake、ament_python三种类型可选；
--dependencies：所依赖的功能包列表；
--node-name：可执行程序的名称，会自动生成对应的源文件并生成配置文件。
2.编译
编译功能包语法如下：

colcon build
或

colcon build --packages-select 功能包列表
前者会构建工作空间下的所有功能包，后者可以构建指定功能包。

3.查找
在ros2 pkg命令下包含了多个查询功能包相关信息的参数。

ros2 pkg executables [包名] # 输出所有功能包或指定功能包下的可执行程序。
ros2 pkg list # 列出所有功能包
ros2 pkg prefix 包名 # 列出功能包路径
ros2 pkg xml # 输出功能包的package.xml内容
4.执行
执行命令语法如下：

ros2 run 功能包 可执行程序 参数

### 1.3.2 ros2核心模块

#### 1.3.2.1 通信模块
通信模块是整个ROS2架构中的重中之重，比如你可能想要了解在ROS2中是如何控制机器人底盘运动的？雷达、摄像头、imu、GPS等这些传感器数据是如何传输到ROS2系统的？人机交互时调用者如何下发指令，机器人又是如何反馈数据的？导航、机械臂等系统性实现不同模块之间是如何交互数据的。

#### 1.3.2.2 功能包
功能包的应用主要有三种方式：

1.二进制安装
ROS官方或社区提供的功能包可以很方便的通过二进制方式安装，安装命令如下：

sudo apt install ros-ROS2版本代号-功能包名称
小提示：
可以调用apt search ros-ROS2版本代号-* | grep -i 关键字格式的命令，根据关键字查找所需的功能包。

2.源码安装
也可以直接下载官方、社区或其他第三方提供的源代码，一般我们会从github获取源码，下载命令如下：
git clone 仓库地址
源码下载后，需要自行编译。

3.自实现
开发者按照业务需求自己编写功能包实现。

#### 1.3.2.3 分布式
ROS2是一个分布式架构，不同的ROS2设备之间可以方便的实现通信，这在多机器人设备协同中是极其重要的。

#### 1.3.2.4 终端命令与rqt
rqt是一个图形化工具，它的功能与命令行工具类似，但是图形化的交互方式更为友好。

#### 1.3.2.5 launch文件
通过launch文件，可以批量的启动ROS2节点，这是在构建大型项目时启动多节点的常用方式。

#### 1.3.2.6 TF坐标变换
TF坐标变换可以实现机器人不同部件或不同机器人之间的相对位置关系的转换。

#### 1.3.2.7 可视化
ROS2内置了三维可视化工具rviz2，它可以图形化的方式显示机器人模型或显示机器人系统中的一些抽象数据。

### 1.3.3 ros2应用方向
NAV2
Nav2项目继承自ROS Navigation Stack。该项目旨在可以让移动机器人从A点安全的移动到B点。它也可以应用于涉及机器人导航的其他应用，例如跟随动态点。Nav2将用于实现路径规划、运动控制、动态避障和恢复行为等一系列功能。

OpenCV
OpenCV（Open Source Computer Vision Library）是一个开源的计算机视觉和机器学习软件库。OpenCV旨在为计算机视觉应用程序提供通用基础架构，并加速机器感知在商业产品中的使用。OpenCV允许企业轻松地使用和修改代码。

MoveIt
MoveIt是一组ROS软件包， 主要包含运动规划、碰撞检测、运动学、3D感知、操作控制等功能。它可以用于构建机械臂的高级行为。MoveIt现在可以用于市面上的大多数机械臂，并被许多大公司使用。

The Autoware Foundation
Autoware Foundation是ROS下属的非营利组织，支持实现自动驾驶的开源项目。Autoware基金会在企业发展和学术研究之间创造协同效应，为每个人提供自动驾驶技术。

F1 Tenth
F1 Tenth是将模型车改为无人车的竞速赛事，是一个由研究人员、工程师和自主系统爱好者组成的国际社区。它最初于 2016 年在宾夕法尼亚大学成立，但后来扩展到全球许多其他机构。

microROS
在基于ROS的机器人应用中，micro-ROS正在弥合性能有限的微控制器和一般处理器之间的差距。micro-ROS在各种嵌入式硬件上运行，使ROS能直接应用于机器人硬件。

Open Robotics
Open Robotics与全球ROS社区合作，为机器人创建开放的软件和硬件平台，包括 ROS1、ROS2、Gazebo模拟器和Ignition模拟器。Open Robotics使用这些平台解决一些重要问题，并通过为各种客户组织提供软件和硬件开发服务来帮助其他人做同样的事情。

PX4
PX4是一款用于无人机和其他无人驾驶车辆的开源飞行控制软件。该项目为无人机开发人员提供了一套灵活的工具，用于共享技术并为无人机应用程序创建量身定制解决方案。

ROS-Industrial
ROS-Industrial是一个开源项目，将 ROS 软件的高级功能扩展到工业相关硬件和应用程序。

# 2. ros2通信机制核心

## 2.1 通信机制简介
### 2.1.1 节点
在通信时，不论采用何种方式，通信对象的构建都依赖于节点(Node)，在ROS2中，一般情况下每个节点都对应某一单一的功能模块(例如：雷达驱动节点可能负责发布雷达消息，摄像头驱动节点可能负责发布图像消息)。一个完整的机器人系统可能由许多协同工作的节点组成，ROS2中的单个可执行文件(C++程序或Python程序)可以包含一个或多个节点。

### 2.1.2 话题
话题(Topic)是一个纽带，具有相同话题的节点可以关联在一起，而这正是通信的前提。并且ROS2是跨语言的，有的节点可能是使用C++实现，有的节点可能是使用Python实现的，但是只要二者使用了相同的话题，就可以实现数据的交互。

### 2.1.3 通信模型
不同的通信对象通过话题关联到一起之后，以何种方式实现通信呢？在ROS2中，常用的通信模型有四种：

1.话题通信（单向数据流）：是一种单向通信模型，在通信双方中，发布方发布数据，订阅方订阅数据，数据流单向的由发布方传输到订阅方。

2.服务通信（请求响应模式）：是一种基于请求响应的通信模型，在通信双方中，客户端发送请求数据到服务端，服务端响应结果给客户端。

3.动作通信（带有连续反馈）：是一种带有连续反馈的通信模型，在通信双方中，客户端发送请求数据到服务端，服务端响应结果给客户端，但是在服务端接收到请求到产生最终响应的过程中，会发送连续的反馈信息到客户端。

4.参数服务（数据共享）：是一种基于共享的通信模型，在通信双方中，服务端可以设置数据，而客户端可以连接服务端并操作服务端数据。

### 2.1.4 接口
在通信过程中，需要传输数据，就必然涉及到数据载体，也即要以特定格式传输数据。在ROS2中，数据载体称之为接口(interfaces)。通信时使用的数据载体一般需要使用接口文件定义。常用的接口文件有三种：msg文件、srv文件与action文件。每种文件都可以按照一定格式定义特定数据类型的“变量”。

1.msg文件
msg文件是用于定义话题通信中数据载体的接口文件，一个典型的.msg文件示例如下。

int64 num1
int64 num2
在文件中声明了一些被传输的类似于C++变量的数据。

2.srv文件
srv文件是用于定义服务通信中数据载体的接口文件，一个典型的.srv文件示例如下。

int64 num1
int64 num2
---
int64 sum
文件中声明的数据被---分割为两部分，上半部分用于声明请求数据，下半部分用于声明响应数据。

3.action文件
action文件使用用于定义动作通信中数据载体的接口文件，一个典型的.action文件示例如下。

int64 num
---
int64 sum
---
float64 progress
文件中声明的数据被---分割为三部分，上半部分用于声明请求数据，中间部分用于声明响应数据，下半部分用于声明连续反馈数据。

4.变量类型
不管是何种接口文件，在文件中每行声明的数据都由字段类型和字段名称组成，可以使用的字段类型有：

int8, int16, int32, int64 (或者无符号类型: uint*)

float32, float64

string

time, duration

其他msg文件

变长数组和定长数组

ROS中还有一种特殊类型：Header，标头包含时间戳和ROS2中常用的坐标帧信息。许多接口文件的第一行包含Header标头。

另外，需要说明的是：

参数通信的数据无需定义接口文件，参数通信时数据会被封装为参数对象，参数客户端和服务端操作的都是参数对象。
## 2.2 话题通信
话题通信是ROS中使用频率最高的一种通信模式，话题通信是基于发布订阅模式的，也即：一个节点发布消息，另一个节点订阅该消息。

话题通信的发布方与订阅方是一种多对多的关系，也即，同一话题下可以存在多个发布方，也可以存在多个订阅方，这意味着数据会出现交叉传输的情况，当然如果没有订阅方，数据传输也会出现丢失的情况。

关于消息接口的使用有多种方式：

在ROS2中通过std_msgs包封装了一些原生的数据类型,比如：String、Int8、Int16、Int32、Int64、Float32、Float64、Char、Bool、Empty.... 这些原生数据类型也可以作为话题通信的载体，不过这些数据一般只包含一个 data 字段，而std_msgs包中其他的接口文件也比较简单，结构的单一意味着功能上的局限性，当传输一些结构复杂的数据时，就显得力不从心了；
在ROS2中还预定义了许多标准话题消息接口，这在实际工作中有着广泛的应用，比如：sensor_msgs包中定义了许多关于传感器消息的接口（雷达、摄像头、点云......），geometry_msgs包中则定义了许多几何消息相关的接口（坐标点、坐标系、速度指令......）；
如果上述接口文件都不能满足我们的需求，那么就可以自定义接口消息；
## 2.3 服务通信

## 2.4 动作通信

## 2.5 参数服务

# 3. 常用命令
source opt/ros/jazzy/setup.bash               加载ros2系统环境
source [工作空间下]/install/setup.bash         加载工作空间环境
colcon list                                   查看所有功能包
ros2 pkg executables [功能包]                  查看某个功能包下的可执行文件（执行入口）
ros2 pkg list | grep [特定名字]                查找某个关键词的相关功能包  注意：这里列出来的不是某个工作空间下存在的功能包，而是当前环境下所有的功能包
ros2 pkg prefix [功能包]                       查看功能包的安装位置
ros2 node list                                查看运行中的节点
--symlink-install
对于非编译型文件或脚本（如 Python 脚本、Launch 文件、URDF 模型、YAML 配置文件等），colcon 不会去复制它们，而是在 install/ 目录下创建一个指向 src/ 源码文件的软链接。如果加了该参数：因为 install/ 目录下的文件只是指向 src/ 的快捷方式，直接修改 src/ 中的 Python 代码或 Launch 文件并保存，重新运行节点就能直接生效，完全不需要重新编译

# 4. 常用函数
## 4.1 订阅 create_subscription
create_subscription(
    Twist,                  # 消息类型
    '/cmd_vel',             # Topic 名称
    self.cmd_vel_callback,  # 回调函数
    10                      # QoS 深度
)
## 4.2 发布 create_publisher
create_publisher(
    Odometry,               #消息类型
    '/odom/unfiltered',     #Topic 名称
    10                      #QoS 深度
)
## 4.3 声明启动参数 DeclareLaunchArgument
DeclareLaunchArgument(      #声明一个可以在运行时配置的启动参数
    name,                   #必填，参数名（字符串格式 ' '）
    default_value,          #用户未在终端指定该参数时，系统采用的默认值
    description,            #该参数的文字说明
    choices                 #一个列表，限制该参数只能从指定的几个选项中选择
)
## 4.4 包含子launch IncludeLaunchDescription
用来包含其他launch文件，类似于include
示例：
IncludeLaunchDescription(PythonLaunchDescriptionSource(PathJoinSubstitution([
            FindPackageShare('curiousrobot_description'), 'launch', 'description.launch.py'])),
            launch_arguments={
                'use_sim_time': LaunchConfiguration('use_sim_time'),
                'publish_joints': LaunchConfiguration('preview_joints'),
                'rviz': LaunchConfiguration('rviz'),
            }.items())
其中，
PythonLaunchDescriptionSource：告诉系统被包含的目标文件是一个 Python 写的 Launch 文件

PathJoinSubstitution：把不同的路径部分（如功能包路径、文件夹名、文件名）组合成一个完整的、跨平台的绝对文件路径

FindPackageShare：通过包名查找路径

launch_arguments：子 Launch 文件通常也会用主launch中的 DeclareLaunchArgument 声明参数。通过主 Launch 中的 launch_arguments={...}.items()，可以实现自上而下的参数透传

LaunchConfiguration：引用和获取DeclareLaunchArgument声明参数的值

# 5. 相关概念
## 5.1 QoS（Quality of Service）

一种用于配置节点之间通信行为的机制

ROS 2 的 QoS 主要由以下几种关键策略（Policies）组合而成：
1. Reliability（可靠性）
RELIABLE（可靠传输）：类似于 TCP。底层会通过确认和重传机制确保每一条消息都送达。如果丢包，会尝试重新发送。

BEST_EFFORT（尽力而为）：类似于 UDP。发送方发完即走，不确认对方是否收到，丢包也不重传。适用于高频、可容忍少量丢包的数据。

2. Durability（持久性）
VOLATILE（易失）：不保存历史消息。新订阅者只能接收到它连接之后发布的新消息。

TRANSIENT_LOCAL（本地暂存）：发布者（Publisher）会把最近发布的消息在本地缓存。当新的订阅者晚加入时，发布者会把历史缓存消息发送给它（类似于 ROS 1 中的 latch=True）。

3. History（历史记录）
KEEP_LAST（保留最后几条）：只保留最近的若干条消息，数量由 Depth（深度） 参数决定。

KEEP_ALL（保留所有）：保留队列允许范围内的所有消息（直到资源耗尽）。

4. Depth（队列深度）
与 KEEP_LAST 配合使用。例如设置 History = KEEP_LAST 且 Depth = 10，意味着系统最多在队列中缓存最新的 10 条消息。如果消息产生速度大于消费速度，旧的消息会被覆盖。

5. Deadline（截止时间）
规定了两条连续消息发布之间的最大允许时间间隔。如果发布者在规定时间内没有发布新消息，或者订阅者在规定时间内没有收到消息，系统会触发超时回调。

6. Lifespan（生命周期）
规定了消息的“保质期”。如果消息在队列中停留的时间超过了设定的生命周期，且未被消费，它就会自动过期并被丢弃。

7. Liveliness（活跃度）
用于检测发布者是否仍然“存活”。如果发布者崩溃或网络中断，系统可以通过心跳机制及时通知订阅者。

在 ROS 2 中，发布者和订阅者的 QoS 设置必须兼容，否则它们无法建立通信。QoS 的存在，就是让“大而密但不重要的传感器数据”走轻量通道，让“小而精且致命的控制指令”走钢铁长城。

## 5.2 EKF

融合多传感器数据，从充满噪声的多个传感器数据（轮式编码器、IMU、GPS/视觉里程计/激光雷达）中，实时推算出系统最真实、最可靠的内部状态（如位置、速度、姿态）
作用一：数据融合
作用二：处理非线性系统
作用三：量化并压制不确定性
EKF 的数学本质是一个递归的最小均方误差估计器，它通过两个交替的步骤来工作：
预测步（Predict）：基于系统的物理运动模型（如运动学方程）和上一时刻的状态，预测当前时刻的状态，并推算误差的不确定性。
更新步（Update）：当新的传感器测量值到来时，EKF 会对比“预测值”和“测量值”，根据各自的噪声水平（协方差矩阵）进行加权平均——谁更靠谱，就给谁更高的权重，从而修正预测误差，输出最优估计。

为什么使用 EKF 融合 IMU来发布odom？
由于轮式里程计存在测量噪声、模型误差及累计漂移，单独依赖其数据难以获得长期可靠的运动状态估计。因此，可以使用 EKF 融合轮式里程计与 IMU 测量，结合运动模型及测量协方差估计机器人的位置、速度和姿态，提高局部状态估计的鲁棒性与连续性。

## 5.3 robot_localization
robot_localization 是一个非常成熟、标准的状态估计（State Estimation）功能包集。它专门用来解决机器人在三维或二维空间中的定位问题。它的核心任务是：接收来自各种传感器的原始数据，通过滤波算法融合它们，最终输出一个连续、高精度的机器人全局或局部位姿（Pose）和速度（Twist）。EKF（扩展卡尔曼滤波）和UKF（无迹卡尔曼滤波）就是它提供的两个节点

输入：Odometry、IMU、Pose
输出：融合后的Odometry、TF广播

robot_localization 的状态向量（15个）按以下顺序排列：
位置(x, y, z)、位姿(roll, pitch, yaw)、线速度(vx, vy, vz)、角速度(wx, wy, wz)、线性加速度(ax, ay, az)

## 6. Something
## 6.1 参数覆盖机制
比如说一个launch文件中对某些parameters进行了覆盖，或者有yaml文件，那么在实际启动这个节点的时候__init__会采用这个覆盖的值，并且替换cfg中的值。流程是：declare_parameter()（声明ros参数，即覆盖了的值） -> get_parameter()读取到覆盖值
Launch 中后传入的参数字典 > YAML 配置 > Python 默认值（py中的默认值defaults）
## 6.2 话题重映射 Remapping
remappings=[
    ('odometry/filtered', '/odom')
]
以上的两个话题，左边的开头没有/，右边有/

odometry/filtered      #相对名称，会结合节点命名空间解析
/odom                  #绝对名称，指向根命名空间下的 /odom

例如，一个节点位于 /robot1 命名空间中：
odometry/filtered通常会解析为/robot1/odometry/filtered
而/odom仍然是/odom
假设你有两台机器人robot1、robot2，它们都运行相同的 EKF 程序，你希望分别发布/robot1/odom、/robot2/odom，那么就需要使用：

remappings=[
    ('odometry/filtered', 'odom')
]
## 6.3 name和namespace
Node(
    package='robot_localization',
    executable='ekf_node',
    name='ekf_filter_node',
    namespace='robot1',
    remappings=[
        ('odometry/filtered', 'odom')
    ]
)
name            指定ros2节点名称
namespace       指定节点所在的命名空间
executable      指定启动哪个可执行程序

最终节点的完整名称为：/robot1/ekf_filter_node
它的重映射输出 Topic 为：/robot1/odom
## 6.4 二维运动学模型
速度：
dx/dt = vx · cos(yaw) - vy · sin(yaw)
dy/dt = vx · sin(yaw) + vy · cos(yaw)
位置：
对速度进行0-t积分

## 6.5 urdf
<link name="base_link"/>
<link name="left_wheel_link"/>

<joint name="left_wheel_joint" type="continuous">
  <parent link="base_link"/>
  <child link="left_wheel_link"/>
  <origin xyz="0 0.18 -0.08" rpy="0 0 0"/>
  <axis xyz="0 1 0"/>
</joint>
type="continuous"	     可以绕指定轴连续旋转，没有 URDF 定义的角度上下限
parent	                 父刚体是 base_link
child	                 子刚体是 left_wheel_link
origin	                 车轮关节相对于底盘的位置
axis	                 关节的旋转轴方向
