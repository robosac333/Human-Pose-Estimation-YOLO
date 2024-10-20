#include "../include/YoloDetector.hpp"
#include <opencv2/opencv.hpp>
#include <string>

int main() {
    // cv::VideoCapture cap(0);  // Open the default camera
    // if (!cap.isOpened()) {
    //     std::cerr << "Error opening video capture" << std::endl;
    //     return -1;
    // }

    // Use relative paths
    std::string modelPath = "../yolo_classes/yolov11x.pt";
    std::string onnx_path = "/home/sjd1111/Monocular-Human-Detection-YOLO/yolo_classes/yolo11x.onnx";
    std::string coco_path = "../yolo_files/coco.names";

    YoloDetector detector(
        modelPath,
        onnx_path,
        640,  // inputWidth
        640,  // inputHeight
        0.5f, // confidenceThreshold
        0.5f, // nmsThreshold
        {"person", "car", "dog"}  // classNames
    );

    detector.loadModel();
    // cv::Mat frame;

    // while (true) {
    //     cap >> frame;
    //     if (frame.empty()) {
    //         std::cerr << "Error capturing frame" << std::endl;
    //         break;
    //     }

    //     detector.detectAndTrack(frame);

    //     cv::imshow("Human Detector and Tracker", frame);

    //     if (cv::waitKey(30) >= 0) break;
    // }

    return 0;
}