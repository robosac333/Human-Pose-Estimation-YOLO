#include "../include/loadModel.hpp"
#include <opencv2/opencv.hpp>
#include <string>

int main() {
    cv::VideoCapture cap(0);  // Open the default camera
    if (!cap.isOpened()) {
        std::cerr << "Error opening video capture" << std::endl;
        return -1;
    }

    // Use relative paths
    std::string modelPath = "/home/navdeep/Project/Monocular-Human-Detection-YOLO/yolo_classes/yolov3.weights";
    std::string config_path = "/home/navdeep/Project/Monocular-Human-Detection-YOLO/yolo_classes/yolov3.cfg";
    std::string coco_path = "/home/navdeep/Project/Monocular-Human-Detection-YOLO/yolo_classes/coco.names";

    loadModel detector(
        modelPath,
        config_path,
        coco_path);

    detector.loadFromFile();
    cv::Mat frame;

    while (true) {
        cap >> frame;
        if (frame.empty()) {
            std::cerr << "Error capturing frame" << std::endl;
            break;
        }

    //     detector.detectAndTrack(frame);

        cv::imshow("Human Detector and Tracker", frame);

        if (cv::waitKey(30) >= 0) break;
    }

    return 0;
}