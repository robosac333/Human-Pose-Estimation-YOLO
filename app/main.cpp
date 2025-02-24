// C++ system headers (alphabetical order)
#include <string>

// Third-party library headers
#include <opencv2/opencv.hpp>

// Other/local headers (alphabetical order)
#include "Tracker.hpp"
#include "loadModel.hpp"

int main() {
  cv::VideoCapture cap(0);  // Open the default camera
  if (!cap.isOpened()) {
    std::cerr << "Error opening video capture" << std::endl;
    return -1;
  }

  // Use relative paths
  std::string modelPath =
      "/home/navdeep/Project/Monocular-Human-Detection-YOLO/yolo_classes/"
      "yolov3.weights";
  std::string config_path =
      "/home/navdeep/Project/Monocular-Human-Detection-YOLO/yolo_classes/"
      "yolov3.cfg";
  std::string coco_path =
      "/home/navdeep/Project/Monocular-Human-Detection-YOLO/yolo_classes/"
      "coco.names";

  cv::Mat frame;
  Tracker tracker(modelPath, config_path, coco_path, frame);
  tracker.loadFromFile();

  // Get start time
  int64_t start_time = cv::getTickCount();
  const int DURATION_SECONDS = 500;

  while (true) {
    // Check elapsed time
    int64_t current_time = cv::getTickCount();
    double elapsed_time = (current_time - start_time) / cv::getTickFrequency();

    // Stop after DURATION_SECONDS
    if (elapsed_time >= DURATION_SECONDS) {
      std::cout << "Tracking stopped after " << DURATION_SECONDS << " seconds"
                << std::endl;
      break;
    }

    cap >> frame;
    if (frame.empty()) {
      std::cerr << "Error capturing frame" << std::endl;
      break;
    }

    // Display remaining time
    int remaining_time = DURATION_SECONDS - elapsed_time;
    cv::putText(
        frame, "Stopping tracking after" + std::to_string(remaining_time) + "s",
        cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 255, 0),
        2);

    tracker.Track(frame);
    cv::imshow("Human Detector and Tracker", frame);

    // Check for key press with a small wait
    char key = cv::waitKey(30);
    if (key == 27 || key == 'q') {  // ESC or 'q' to quit early
      std::cout << "Manual stop triggered" << std::endl;
      break;
    }
  }

  // Cleanup
  cap.release();
  cv::destroyAllWindows();

  return 0;
}
