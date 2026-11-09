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
source opt/ros/jazzy/setup.bash                
source [工作空间下]/install/setup.bash         
colcon list                                   查看所有功能包
ros2 pkg executables [功能包]                  查看某个功能包下的可执行文件（执行入口）
ros2 pkg list | grep [特定名字]                查找某个关键词的相关功能包  注意：这里列出来的不是某个工作空间下存在的功能包，而是当前环境下所有的功能包
ros2 pkg prefix [功能包]                       查看功能包的安装位置