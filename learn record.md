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
ros2 node list                                查看运行中的节点

## 3.1 ros2 param
ros2 param list /节点名                        查看指定节点的所有参数
ros2 param get /节点名 参数名                   查看某个参数的当前值
ros2 param set /节点名 参数名 值                修改运行中节点的参数值
ros2 param dump /节点名                        以 YAML 格式输出节点的全部参数
注意：set 只修改运行中节点的参数；若节点只在启动时读取参数，修改后不会自动改变其工作行为。

## 3.2 ros2 topic
ros2 topic list                               查看当前所有话题
ros2 topic list -t                            查看话题及其消息类型
ros2 topic type [话题名]                       查看 [话题名] 的消息类型
ros2 topic info [话题名]                       查看 [话题名] 的消息类型、发布者数量和订阅者数量
ros2 topic info [话题名] -v                    查看发布者、订阅者及其 QoS 详细信息
ros2 topic echo [话题名]                       持续打印 [话题名] 收到的消息，按 Ctrl+C 退出
ros2 topic echo [话题名] --once                只接收并打印一条消息
ros2 topic hz [话题名]                         统计 [话题名] 的实际接收频率（Hz）
ros2 topic bw [话题名]                         统计 [话题名] 的接收带宽
ros2 topic pub --once /test std_msgs/msg/String "{data: 'hello'}"  向 /test 发布一条 String 消息
ros2 topic pub -r 10 /test std_msgs/msg/String "{data: 'hello'}"  以 10 Hz 持续发布消息，按 Ctrl+C 退出
ros2 interface show sensor_msgs/msg/LaserScan 查看 LaserScan 消息有哪些字段

## 3.3 ros2 pkg
ros2 pkg executables [功能包]                  查看某个功能包下的可执行文件（执行入口）
ros2 pkg list | grep [特定名字]                查找某个关键词的相关功能包  注意：这里列出来的不是某个工作空间下存在的功能包，而是当前环境下所有的功能包
ros2 pkg prefix [功能包]                       查看功能包的安装位置
ros2 pkg create [功能包]                       创建功能包

## 3.4 colcon
colcon list                                   查看所有功能包
colcon build --packages-select [功能包]        只构建指定功能包
colcon build --packages-up-to [功能包]         构建指定功能包及其递归依赖

## other
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

## 4.5 服务通信 create_service
create_service(
    Trigger,                  #服务类型，Trigger的request为空，response包含success和message
    'reset_odometry',         #服务名，字符串格式
    self.reset_callback       #回调函数
)

## 4.6 创建定时器 create_wall_timer
create_wall_timer(
    5ms,                         #时间
    std::bind(                   #绑定回调函数
        &BridgeNode::poll,
        this
    )
)

## 4.7 绑定函数 std::bind
std::bind(
    &MyNode::timer_callback,     #回调函数地址
    this                         #绑定当前对象this指针
)
也可以用lambda表达式替换它


# 5. 相关概念
## 5.1 QoS（Quality of Service）

一种用于配置节点之间通信行为的机制

ROS 2 的 QoS 主要由以下几种关键策略（Policies）组合而成：
1. Reliability（可靠性）-----核心参数
RELIABLE（可靠传输）：类似于 TCP。底层会通过确认和重传机制确保每一条消息都送达。如果丢包，会尝试重新发送。

BEST_EFFORT（尽力而为）：类似于 UDP。发送方发完即走，不确认对方是否收到，丢包也不重传。适用于高频、可容忍少量丢包的数据。

2. Durability（持久性）-----核心参数
VOLATILE（易失）：不保存历史消息。新订阅者只能接收到它连接之后发布的新消息。常见用途：/scan、/cmd_vel

TRANSIENT_LOCAL（本地暂存）：发布者（Publisher）会把最近发布的消息在本地缓存。当新的订阅者晚加入时，发布者会把历史缓存消息发送给它（类似于 ROS 1 中的 latch=True）。常见用途：静态地图、某些配置或状态信息

3. History（历史记录）-----核心参数
KEEP_LAST（保留最后几条）：只保留最近的若干条消息，数量由 Depth（深度） 参数决定。

KEEP_ALL（保留所有）：保留队列允许范围内的所有消息（直到资源耗尽）。

4. Depth（队列深度）-----核心参数
与 KEEP_LAST 配合使用。例如设置 History = KEEP_LAST 且 Depth = 10，意味着系统最多在队列中缓存最新的 10 条消息。如果消息产生速度大于消费速度，旧的消息会被覆盖。

5. Deadline（截止时间）
规定了两条连续消息发布之间的最大允许时间间隔。如果发布者在规定时间内没有发布新消息，或者订阅者在规定时间内没有收到消息，系统会触发超时回调。

6. Lifespan（生命周期）
规定了消息的“保质期”。如果消息在队列中停留的时间超过了设定的生命周期，且未被消费，它就会自动过期并被丢弃。

7. Liveliness（活跃度）
用于检测发布者是否仍然“存活”。如果发布者崩溃或网络中断，系统可以通过心跳机制及时通知订阅者。

在 ROS 2 中，发布者和订阅者的 QoS 设置必须兼容，否则它们无法建立通信。QoS 的存在，就是让“大而密但不重要的传感器数据”走轻量通道，让“小而精且致命的控制指令”走钢铁长城。

一个常见的传感器QoS配置：
qos = QoSProfile(
    reliability=ReliabilityPolicy.BEST_EFFORT,
    durability=DurabilityPolicy.VOLATILE,
    history=HistoryPolicy.KEEP_LAST,
    depth=5
)

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

## 5.4 调度器 Executor
可以把 Executor 理解成 ROS 2 节点的回调调度器
1. Executor包含：SingleThreadedExecutor 和 MultiThreadedExecutor，不是说换成multi就可以实现多线程了，还要考虑两个任务是否在同一个Callback Group，在的话就无法实现多线程
2. ROS 2 中常见的 Callback Group 有两种：
MutuallyExclusive（互斥回调组）：同一个组内的回调不能同时执行，即使 Executor 有多个线程。
Reentrant（可重入回调组）：允许同一个组内的不同回调并发执行，也可能允许同一个回调的多个实例重叠执行，因此需要保证线程安全。

Executor 决定有多少线程可以调度回调，Callback Group 决定哪些回调允许并发执行。

## 5.5 CMakeLists.txt
作用：定义源代码如何编译、链接和安装
主要格式：
1. 查找依赖
find_package(ament_cmake REQUIRED)                   查找构建所需的依赖包
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)

2. 定义可执行文件
add_executable(bridge_node src/bridge_node.cpp)      指定哪些源文件编译成哪个可执行程序

3. 为可执行文件配置 ROS 2 依赖
ament_target_dependencies(bridge_node                为目标配置 ROS 2 依赖
  rclcpp
  std_msgs
)

4. 安装可执行文件
install(TARGETS                                      指定编译产物安装到哪里
    bridge_node
    DESTINATION lib/${PROJECT_NAME}
)

ament_package()                                      完成 ROS 2 ament 包的配置

其他函数：
target_include_directories(bridge_node               配置头文件搜索路径
    PRIVATE
    ${SERIAL_INCLUDE_DIRS}
)

target_link_libraries(bridge_node                    配置链接库
    PRIVATE
    serial_library
)

add_library(command_gate                             创建库目标
    src/command_gate.cpp
)

问题：假如说我在 功能包A 中创建了一个新的command库，我在另一个 功能包B 中想要用这个库，即跨功能包复用库，该怎么做？
首先，在功能包A的CMakeLists中：
1. 创建库：
add_library(command_gate SHARED
  src/command_gate.cpp
)
2. 配置头文件路径
target_include_directories(command_gate PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
3. 安装库
install(TARGETS command_gate                       将编译生成的库安装到工作空间
  EXPORT export_command_gate
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
)
4. 安装头文件
install(DIRECTORY include/                         安装头文件，让其他包能够使用。
  DESTINATION include
)
5. 导出库的构建信息
ament_export_targets(export_command_gate HAS_LIBRARY_TARGET)     向其他 CMake 功能包导出库目标及相关构建信息。

其次，在功能包B的package.xml中：
<depend>[功能包A]</depend>                 声明功能包A的依赖
其次，在功能包B的CMakeLists中：

1. 查找功能包A
find_package([功能包] REQUIRED)
2. 为可执行文件配置 ROS 2 依赖
ament_target_dependencies(test_node
  stm32_base_bridge
)

## 5.6 package.xml
作用：声明功能包级的依赖，CMakeLists.txt是配置具体编译目标，这两者需要配合

## 5.7 RAII机制 Resource Acquisition Is Initialization
可以理解成：资源的生命周期绑定到对象的生命周期。人话：把“申请资源”和“释放资源”绑定到一个局部对象的“出生（构造函数）”和“去世（析构函数）”上
比如文件、串口、锁、动态内存，都可以由一个对象管理。
std::unique_ptr
std::lock_guard
std::fstream
本质上都体现了RAII

# 6. Something
## 6.1 项目介绍
### 一、项目背景与整体目标

CuriousRobot 是一个基于 ROS 2 Jazzy 和 STM32 的四轮差速式移动机器人项目。项目采用上下位机分层架构：上位机运行 Ubuntu 和 ROS 2，负责运动指令交互、数据通信、状态估计以及后续的导航功能；下位机采用 STM32，负责接收运动控制指令、驱动底盘，并采集传感器数据。项目的整体目标是建立一套从上层运动控制到底盘执行、再到传感器反馈和机器人状态估计的完整软件链路，为后续接入激光雷达、SLAM 和 Nav2 自主导航提供基础。

### 二、ROS 2 软件架构与功能包职责

CuriousRobot 主要涉及以下功能包：
**1. stm32_base_bridge**
负责 ROS 2 与 STM32 之间的通信，它订阅 `/cmd_vel` 运动控制指令，将速度转换成串口协议数据帧发送给 STM32；同时接收下位机上传的遥测数据，解析后发布里程计和 IMU 消息。

**2. stm32_robot_bringup**
负责系统启动和配置管理，通过 ROS 2 Launch 文件组织相关节点，并将 YAML 配置参数传入节点。

**3. curiousrobot_description**
负责描述机器人的机械结构，包括底盘、车轮、传感器等部件的 link、joint 和几何模型，为 robot_state_publisher、TF 和 RViz 可视化提供机器人描述。

**4. stm32_robot_navigation**
用于组织机器人状态估计和导航相关配置，包括 EKF 等模块，并为后续导航功能提供集成入口。

**5. ldlidar**
用于激光雷达的数据接入，为后续环境感知、建图和导航提供传感器数据。


### 三、运动控制指令链路

项目通过 `/cmd_vel` 接收上层运动指令，消息类型为 `geometry_msgs/msg/Twist`，主要使用：
- `linear.x`：机器人前进或后退的线速度。
- `angular.z`：机器人绕竖直轴旋转的角速度。

Bridge 收到指令后，并不会立即发送到 STM32。
首先，`on_command()` 回调接收消息，调用 `CommandGate` 对指令进行合法性检查、速度限幅和状态保存。随后，定时执行的 `poll()` 根据配置的发送周期，从 CommandGate 获取当前允许输出的速度，并通过 `send()` 调用 `encode_command()` 完成协议编码，最终使用串口发送到 STM32。STM32 接收并解析指令后，按照底盘控制逻辑驱动电机。

因此，控制链路可以概括为：
**上层控制节点 → /cmd_vel → Bridge → CommandGate → 串口协议 → STM32 → 电机与底盘。**

### 四、STM32 遥测反馈链路

机器人运动过程中，下位机采集底盘状态和 IMU 数据，通过串口协议返回给上位机。Bridge 的 `poll()` 负责串口轮询和数据读取，`TelemetryParser.feed()` 对收到的数据进行帧解析，处理拆包、粘包以及异常数据等情况。
解析成功后，`publish_sample()` 根据遥测帧中的有效标志位决定发布哪些 ROS 2 消息：
- 当 `ODOM_VALID` 有效时，发布 `/odom/unfiltered`，消息类型为 `nav_msgs/msg/Odometry`。
- 当 `IMU_VALID` 有效时，发布 `/imu/data`，消息类型为 `sensor_msgs/msg/Imu`。

反馈链路可以概括为：
**编码器与 IMU → STM32 → 串口遥测 → Bridge → Odometry/IMU Topic。**

### 五、EKF 状态估计与 TF 坐标变换

项目使用 robot_localization 中的 EKF 对轮式里程计和 IMU 数据进行状态估计。EKF 主要使用轮式里程计中的前进速度、横向速度和偏航角速度，以及 IMU 中配置启用的z轴角速度。EKF 根据机器人运动模型和传感器观测估计位姿，并输出融合后的 `/odom` 消息，以及 `odom → base_footprint` 动态坐标变换。

项目中的主要坐标关系为：
**map → odom → base_footprint → base_link → 传感器及车轮坐标系。**

其中，`odom → base_footprint` 用于表示机器人相对于里程计坐标系的运动；`base_link` 到传感器的固定变换可以由 robot_state_publisher 根据 URDF 发布。

### 六、底盘通信与运动安全机制

为了避免串口异常、控制指令失效等问题导致机器人持续运动，Bridge 实现了多项安全机制。
1. 对 `/cmd_vel` 中的速度值进行有限数值检查、合法性检查和限幅。
2. Bridge 设置了指令超时机制。如果上层长时间没有发送有效控制指令，CommandGate 会输出零速度。
3. Bridge 还会检查串口连接状态、遥测数据是否超时以及电机故障标志。如果底盘状态不满足安全条件，就不应继续输出非零运动指令。
4. 在串口协议方面，项目使用帧头、消息类型、长度、序列号和 CRC 校验等机制，帮助识别有效数据帧和异常数据。

## 6.2 参数覆盖机制
比如说一个launch文件中对某些parameters进行了覆盖，或者有yaml文件，那么在实际启动这个节点的时候__init__会采用这个覆盖的值，并且替换cfg中的值。流程是：declare_parameter()（声明ros参数，即覆盖了的值） -> get_parameter()读取到覆盖值
Launch 中后传入的参数字典 > YAML 配置 > Python 默认值（py中的默认值defaults）

## 6.3 话题重映射 Remapping
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

## 6.4 name和namespace
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

## 6.5 二维运动学模型
速度：
dx/dt = vx · cos(yaw) - vy · sin(yaw)
dy/dt = vx · sin(yaw) + vy · cos(yaw)
位置：
对速度进行0-t积分

## 6.6 urdf
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

## 6.7 setup.py
这个文件只是在构建安装阶段执行，用于配置入口点，并非每次都需要执行，CMakeLists.txt同理
Python的入口配置：setup.py
C++的入口配置：CMakeLists.txt

## 6.8 串口桥接节点
Bridge 节点由 ROS 2 Launch 启动，通过已安装的 Python 入口点进入 main 函数，实例化节点并初始化参数、Publisher、Subscriber 和 Timer。节点进入 Executor 调度后，主要通过两类回调工作。第一类是 /cmd_vel 订阅回调 on_command()，负责接收、校验、限幅和缓存上层运动指令，并结合底盘通信状态进行安全判断。第二类是定时器回调 poll()，以约 5 ms 为周期维护串口通信，读取 STM32 遥测，通过 TelemetryParser 解析并发布 Odometry 和 IMU 数据。同时，它按照独立的发送周期获取 CommandGate 的安全速度输出，编码后发送给 STM32。当发生指令超时、遥测超时、电机故障或串口异常时，Bridge 会限制非零运动输出；串口断开后会尝试重连，并清理旧状态，避免恢复通信后执行过期指令。

## 6.9 ros2通信方式
### 6.9.1 Topic 通信
### 6.9.2 Service 通信
整个过程分为四步：
1. Client 创建请求，并调用 Service。
2. ROS 2 将请求传递给对应的 Server。
3. Server 的回调函数处理请求。
4. Server 返回响应，Client 获取结果。

使用 .srv 文件定义 Service 接口：
bool reset                     // request
---
bool success                   // response
string message

### 6.9.3 Action 通信
Action是为需要一定执行时间的任务设计。Action 不是完全独立于 Topic 和 Service 的通信基础设施，而是在它们之上建立的一套任务执行协议。在 ROS 2 底层，一个 Action 通常使用三个 Service 和两个 Topic 实现：
Send Goal Service	           提交目标
Get Result Service	           获取最终结果
Cancel Goal Service	           请求取消目标
Feedback Topic	               发布执行过程中的反馈
Status Topic	               发布目标状态

使用 .action 文件定义 action 接口：
float64 target_x               // Goal
float64 target_y
---
bool success                   // Result
---
float64 remaining_distance     // Feedback

## 6.10 常见问题
### 6.10.1 运行功能包后，发现话题已存在，但自己的节点始终订阅不到数据。
首先怀疑是不是两者的QoS配置有差异，即QoS兼容问题
ros2 topic info /scan --verbose
查看QoS配置

## 6.11 colcon build 构建流程
colcon build 后，它会经历以下步骤：
1. 发现阶段：遍历 src/ 下的所有文件夹，寻找含有 package.xml 的目录。
2. 分类阶段：读取每个 package.xml 里的 build_type，将它们归类为 C++ 包、Python 包或其他类型。
3. 依赖排序：根据 package.xml 中声明的 <depend> 依赖关系，自动计算出正确的编译顺序（被依赖的包先编译）。
4. 执行构建：依次对每个包调用对应的构建系统（读取 CMakeLists.txt 或 setup.py）完成构建。

## 6.12 build_depend、exec_depend 和 depend
<build_depend>	            构建依赖	                     编译时需要
<exec_depend>	            执行依赖	                     运行时需要
<depend>	                同时声明构建、导出和执行依赖	   编译、导出、运行都需要
<buildtool_depend>      	构建过程中使用的工具	          例如：ament_cmake

# 7 C++
## 7.1 UniquePtr
转移对象所有权：
auto robot1 = std::make_unique<Robot>();
auto robot2 = std::move(robot1);
robot2.reset()   释放指针
## 7.2 Mutex锁
举例子：
std::mutex serial_mutex_;
std::lock_guard<std::mutex> lock(serial_mutex_);

std::lock_guard<std::mutex>   作用：管理一个互斥锁的生命周期（锁卫士）
当代码执行到这行时，lock 对象被创建，它会立即自动调用 serial_mutex_.lock()，把锁给锁上。如果此时锁被别的线程占着，当前线程就会在这里阻塞等待，直到拿到锁。
当代码运行离开当前的作用域（比如函数执行结束，或者遇到了大括号 }），lock 对象生命周期结束并被销毁，它会自动调用 serial_mutex_.unlock() 把锁释放。
lock_guard：无论函数是正常结束，还是中途报错抛出异常，只要离开作用域，栈上的 lock 对象被销毁时一定会自动解锁。
## 7.3 explicit关键字
它的核心作用是：禁止隐式类型转换。默认情况下，如果一个类的构造函数只接受一个参数（或者多个参数但其他参数都有默认值），C++ 编译器会允许“隐式转换”。
也就是说，你可以把一个基本数据类型直接赋值给一个对象，编译器会“好心”地悄悄帮你把这个基本类型用构造函数包装成一个临时对象。
C++ 规范（如 Google C++ Style Guide）明确规定：所有单参数构造函数都必须是显式的（即加上 explicit）
## 7.4 库
#include <memory>                智能指针需要用到 share_ptr unique_ptr
#include <functional>            std::bind需要用到
#include <chrono>                定时器需要用到
搭配using namespace std::chrono_literals;使用，可以在函数中直接写1s 500ms
## 7.5 数组
std::array<int, 3> data{1, 2, 3}               固定长度，编译时已确定
std::vector<int> data{1, 2, 3, 4}              动态数组
vector.push_back(5);                           填加数据
## 7.6 lambda表达式
本质上是一种“匿名函数”——你可以在需要传函数的地方，就地写出一段简短逻辑，而不用专门去外面定义一个具名函数或写一个繁琐的仿函数类。
写法：
            [捕获列表](参数列表) mutable -> 返回类型 { 函数体 }

1. [捕获列表]（必填）：决定 Lambda 内部可以使用外部作用域里的哪些变量（按值拷贝还是按引用）。就算不用外部变量，也必须写一对空方括号 []。
2. (参数列表)（可选）：和普通函数参数一样。如果没有参数，可以写 ()，甚至在不需要 mutable 或尾置返回类型时直接省略 ()。
3. mutable（可选）：默认情况下，按值捕获的变量在 Lambda 内部是只读（const）的；加上 mutable 后，允许在函数体内修改按值捕获的局部副本（注意：改的只是副本，不影响外部原变量）。
4. -> 返回类型（可选）：通常编译器能自动推导返回类型，绝大多数情况可省略；当返回类型复杂或需要明确指定时再写。
5. { 函数体 }（必填）：要执行的具体代码。

常见写法：
            [捕获列表](参数列表) { 函数体 }

常见捕获方式：
[]	                          不捕获任何外部变量
[x]	                          按值捕获变量 x（在 Lambda 创建时拷贝一份）
[&x]	                      按引用捕获变量 x（直接读写外部的 x）
[=]	                          按值捕获当前作用域中用到的所有外部变量
[&]	                          按引用捕获当前作用域中用到的所有外部变量
[this]	                      捕获当前类的 this 指针，从而在成员函数里访问成员变量和成员函数
[=, &x] / [&, x]	          混合捕获：默认按值/按引用，但对个别变量指定相反方式
## 7.7 std::function
可以保存“可调用对象”。它可以把普通函数、Lambda 表达式、std::bind 绑定结果、仿函数（重载了 operator() 的类对象）甚至成员函数指针包装成同一种类型，方便存进变量、放进容器、作为参数传给别人，或者延迟到以后再调用
模板语法：
            std::function<返回类型(参数类型1, 参数类型2, ...)>
可以配合 std::bind 或 Lambda 绑定对象成员函数
## 7.8 CMakeLists.txt
1. 
cmake_minimum_required(VERSION 3.10)           # 指定构建该项目所需的最低 CMake 版本为 3.10（必须写在 CMakeLists.txt 的最前面）
2. 
project(d19_demo)                              # 定义当前项目的名称为 d19_demo（同时会生成如 PROJECT_NAME、d19_demo_SOURCE_DIR 等相关变量）
3. 
add_library(calculator                         # 创建一个名为 calculator 的库目标（默认生成静态库 libcalculator.a，除非显式指定 SHARED 或 MODULE）
    calculator.cpp                             # 指定编译生成 calculator 库所需的源文件列表
)
4. 
target_include_directories(calculator          # 为 calculator 目标配置头文件包含目录
    PUBLIC                                     # PUBLIC 表示：calculator 自身编译时使用该路径，且其他链接了 calculator 的目标也会自动继承该头文件搜索路径
        ${CMAKE_CURRENT_SOURCE_DIR}/include    # ${CMAKE_CURRENT_SOURCE_DIR} 表示当前处理的 CMakeLists.txt 所在目录，拼接 /include 指向头文件目录
)
5. 
add_executable(d19_app                         # 创建一个名为 d19_app 的可执行文件目标
    main.cpp                                   # 指定编译生成 d19_app 可执行程序所需的源文件（通常包含 main 函数入口）
)
6. 
target_link_libraries(d19_app                  # 将 calculator 库链接到 d19_app 可执行目标上
    PRIVATE                                    # PRIVATE 表示：仅 d19_app 自身链接使用 calculator，不会向依赖 d19_app 的其他目标传递此链接关系
        calculator                             # 被链接的目标名称，对应前面通过 add_library 定义的 calculator
)

** PUBLIC **既作用于当前目标自己，也会向依赖当前目标的其他目标传递
** PRIVATE **仅自己需要，依赖自己的其他目标不需要
** INTERFACE **自己不需要，但是依赖当前目标的其他目标需要

mkdir build
cd build
cmake ..               # 读取上一级目录的 CMakeLists.txt，然后在当前 build 目录生成构建系统（Makefile）
cmake --build .        # 让 CMake 调用底层的构建工具（如 make 或 ninja），对“当前所在目录（.）”里的已配置项目执行实际编译（即执行Makefile）。

