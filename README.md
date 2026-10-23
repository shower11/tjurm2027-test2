# RoboMaster 装甲板灯条识别项目

## 思路流程
图像预处理：将彩色图像转换为灰度图，并使用 cv::threshold 进行二值化，提取高亮灯条。

轮廓提取：使用 cv::findContours 获取白斑边缘，并通过 cv::minAreaRect 拟合最小外接矩形。

装甲板配对：判断灯条数量是否为 2，若是，提取 8 个角点再次求最小外接矩形，绘制装甲板外框。（然而二值化后并没有干扰信息）

视频导出：使用 cv::VideoWriter 将处理后的帧序列保存为 AVI 格式视频。

## 编译与运行指南
1. 准备测试视频
请确保将测试视频文件命名为 test.avi，并放置在 project 根目录下（与 CMakeLists.txt 同级）。

2. 编译项目
在 project 根目录下打开终端，执行以下命令：



mkdir -p build
cd build

cmake ..

make

在 build 目录下执行：

./tjurm-test2

3. 操作说明
程序运行时会弹出一个窗口显示处理结果。

按空格键：逐帧播放（方便查看每一帧的识别细节）。

按 ESC 键：退出程序。

程序结束后，处理好的视频将自动保存在 project/result/output.avi


## 进度
还没做位姿解算
