#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

int main() {
    //设置视频路径及调用
    std::string video_path = "../test.avi"; 
    cv::VideoCapture cap(video_path);

    //检测是否调用成功
    if (!cap.isOpened()) {
        std::cerr << "无法打开视频文件！" << std::endl;
        return -1;
    }

    //记录原视频宽度高度帧率
    int frame_width = (int)cap.get(cv::CAP_PROP_FRAME_WIDTH);
    int frame_height = (int)cap.get(cv::CAP_PROP_FRAME_HEIGHT);
    double fps = cap.get(cv::CAP_PROP_FPS);
    if (fps == 0) fps = 30.0;

    //准备视频写入器
    int fourcc = cv::VideoWriter::fourcc('M', 'J', 'P', 'G');
    cv::VideoWriter writer("../result/output.avi", fourcc, fps, cv::Size(frame_width, frame_height));
    if (!writer.isOpened()) {
        std::cerr << "警告：无法初始化视频写入器！" << std::endl;
    }

    //准备窗口
    cv::namedWindow("Result", cv::WINDOW_NORMAL);

    //准备frame（储存每帧照片），gray（储存灰度照片），binary（储存二值化照片）
    cv::Mat frame, gray, binary;
    
    //设置二值化阈值
    int threshold_value = 95;

    //每帧操作
    while (cap.read(frame)) {
        if (frame.empty()) break; //读取完视频后跳出循环
        
        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY); //将每帧图片转化为灰度照片
        cv::threshold(gray, binary, threshold_value, 255, cv::THRESH_BINARY); //将灰度照片二值化
        
        std::vector<std::vector<cv::Point>> contours; //储存 储存轮廓点的数组 的数组
        cv::findContours(binary, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE); //将每个白色轮廓的边的拐角点记录
        
        std::vector<cv::RotatedRect> light_bars; //储存矩形的数组

        //遍历contours中的每个数组
        for (size_t i = 0; i < contours.size(); i++) {
            cv::RotatedRect rect = cv::minAreaRect(contours[i]); //取一个最小矩形，将每个数组中的点框住，将该矩形储存在rect中
            light_bars.push_back(rect); //将rect中的矩形储存在light_bars数组中
        }

        // 如果正好有两个灯条
        if (light_bars.size() == 2) {
            std::vector<cv::Point2f> all_points; //储存所有点的数组

            // 拿第一个灯条的4个角
            cv::Point2f pts[4]; //pts是储存矩形四个点的数组
            light_bars[0].points(pts);  //将light_bars[0]中的矩形的四个点储存在pts中
            for (int k = 0; k < 4; k++) {
                all_points.push_back(pts[k]);  //将pts中的四个点储存在all_points中
            } 

            // 拿第二个灯条的4个角
            light_bars[1].points(pts);  //将light_bars[1]中的矩形的四个点储存在pts中
            for (int k = 0; k < 4; k++) {
                all_points.push_back(pts[k]);  //将pts中的四个点储存在all_points中
            }

            // 对8个点求最小外接矩形（装甲板）
            cv::RotatedRect armor_rect = cv::minAreaRect(all_points);
            

            cv::Point2f armor_pts[4];  //armor_pts数组储存四个点
            armor_rect.points(armor_pts);  //将上述最小矩形的四个点储存在armor_pts数组中
            for (int j = 0; j < 4; j++) {
                cv::line(frame, armor_pts[j], armor_pts[(j + 1) % 4], cv::Scalar(0, 255, 0), 2);  //将每个点与相邻点连接为一个矩形
            }
        }

        if (writer.isOpened()) {
            writer.write(frame);  //将每一帧录入视频中
        }

        // 显示每一帧
        cv::imshow("Result", frame); 
        


        int key = cv::waitKey(0);  //等待按下ESC或空格
        if (key == 27) {
            // 如果按了 ESC，直接退出循环
            break;
        } 
        else if (key == 32) {
            // 如果按了空格，就什么都不做，直接进入下一次循环（也就是看下一帧）
            continue; 
        }
        else {
            // 按了其他键，让它继续停在当前帧等待，直到按空格或ESC
            while (key != 32 && key != 27) {
                key = cv::waitKey(0);
            }
            if (key == 27) break;
        }
    }

    if (writer.isOpened()) {
        writer.release();  //关闭录像
        std::cout << "视频已成功导出！" << std::endl;
    }

    cap.release();  //关闭视频读取
    cv::destroyAllWindows(); //关闭窗口
    std::cout << "完毕！" << std::endl;

    return 0;
}