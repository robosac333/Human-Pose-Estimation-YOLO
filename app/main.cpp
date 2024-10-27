// #include "loadModel.hpp"
// #include "Tracker.hpp"
// #include <opencv2/opencv.hpp>
// #include <string>

// int main() {
//     cv::VideoCapture cap(0);  // Open the default camera
//     if (!cap.isOpened()) {
//         std::cerr << "Error opening video capture" << std::endl;
//         return -1;
//     }

//     // Use relative paths
//     std::string modelPath = "/home/navdeep/Project/Monocular-Human-Detection-YOLO/yolo_classes/yolov3.weights";
//     std::string config_path = "/home/navdeep/Project/Monocular-Human-Detection-YOLO/yolo_classes/yolov3.cfg";
//     std::string coco_path = "/home/navdeep/Project/Monocular-Human-Detection-YOLO/yolo_classes/coco.names";

//     cv::Mat frame;

//     Tracker tracker(
//         modelPath,
//         config_path,
//         coco_path,
//         frame);

//     tracker.loadFromFile();

//     while (true) {
//         cap >> frame;
//         if (frame.empty()) {
//             std::cerr << "Error capturing frame" << std::endl;
//             break;
//         }



//     tracker.Track(frame);

//     cv::imshow("Human Detector and Tracker", frame);

//     if (cv::waitKey(30) >= 0) break;
//     }

//     return 0;
// }

#include "loadModel.hpp"
#include "Tracker.hpp"
#include <opencv2/opencv.hpp>
#include <string>

int main() {
    // Define paths
    std::string modelPath = "/home/navdeep/Project/Monocular-Human-Detection-YOLO/yolo_classes/yolov3.weights";
    std::string config_path = "/home/navdeep/Project/Monocular-Human-Detection-YOLO/yolo_classes/yolov3.cfg";
    std::string coco_path = "/home/navdeep/Project/Monocular-Human-Detection-YOLO/yolo_classes/coco.names";
    std::string image_path = "/home/navdeep/Project/Monocular-Human-Detection-YOLO/yolo_classes/bus.jpg";  // Add your image path

    // Load the image
    cv::Mat frame = cv::imread(image_path);
    if (frame.empty()) {
        std::cerr << "Error loading image from: " << image_path << std::endl;
        return -1;
    }

    // Print image information
    std::cout << "Image loaded successfully. Size: " << frame.size() 
              << " Channels: " << frame.channels() << std::endl;

    try {
        // Initialize tracker with the loaded image
        Tracker tracker(modelPath, config_path, coco_path, frame);
        
        // Load the model
        if (!tracker.loadFromFile()) {
            std::cerr << "Failed to load model" << std::endl;
            return -1;
        }

        // Process the image
        tracker.Track(frame);

        // Display the result
        cv::imshow("Human Detection Result", frame);
        
        // Wait for key press before closing
        cv::waitKey(0);

        // Optionally save the result
        std::string output_path = "/home/navdeep/Project/Monocular-Human-Detection-YOLO/yolo_classes/detection_result.jpg";
        cv::imwrite(output_path, frame);
        std::cout << "Result saved to: " << output_path << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error occurred: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}