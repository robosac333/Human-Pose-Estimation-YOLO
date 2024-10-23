#include "Tracker.hpp"
#include <iostream>
#include <fstream>

Tracker::Tracker(const std::string& modelPath,
           const std::string& configPath,
           const std::string& classesPath,
           const cv::Mat& image) : detectHuman(modelPath, configPath, classesPath, image)
           {


           }

void Tracker::Track() {
    std::vector<cv::Rect> detections = detectHumans();
    updateTrackers(detections);

    // Draw bounding boxes for detected humans
    for (const auto& detection : detections) {
        cv::rectangle(Image, detection, cv::Scalar(0, 255, 0), 2);
        cv::Point3f location = getLocation(detection);
        cv::putText(Image, 
                    "Human: (" + std::to_string(int(location.x)) + ", " + 
                    std::to_string(int(location.y)) + ", " + 
                    std::to_string(int(location.z)) + ")",
                    cv::Point(detection.x, detection.y - 10),
                    cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0), 2);
    }
}
void Tracker::updateTrackers(const std::vector<cv::Rect>& detections){
    // Update existing trackers
    auto it = trackers.begin();
    while (it != trackers.end()) {
        cv::Rect trackedRect;
        if ((*it)->update(Image, trackedRect)) {
            cv::rectangle(Image, trackedRect, cv::Scalar(255, 0, 0), 2);
            cv::Point3f location = getLocation(trackedRect);
            cv::putText(Image, 
                        "Tracked: (" + std::to_string(int(location.x)) + ", " + 
                        std::to_string(int(location.y)) + ", " + 
                        std::to_string(int(location.z)) + ")",
                        cv::Point(trackedRect.x, trackedRect.y - 10),
                        cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 0, 0), 2);
            ++it;
        } else {
            it = trackers.erase(it);
        }
    }

    // Initialize new trackers for newly detected humans
    for (const auto& det : detections) {
        bool isNewDetection = true;
        for (const auto& tracker : trackers) {
            cv::Rect trackedRect;
            tracker->update(Image, trackedRect);
            if ((det & trackedRect).area() > 0) {
                isNewDetection = false;
                break;
            }
        }
        
        if (isNewDetection) {
            cv::Ptr<cv::Tracker> tracker = cv::TrackerKCF::create();
            tracker->init(Image, det);
            trackers.push_back(tracker);
        }
    }
}

cv::Point3f Tracker::getLocation(const cv::Rect& rect) {
    // Estimate 3D position (x, y, z) in robot's reference frame
    // This is a simplified calculation and would need to be adjusted based on your specific setup

    double focalLength = cameraMatrix.at<double>(0, 0);
    double centerX = cameraMatrix.at<double>(0, 2);
    double centerY = cameraMatrix.at<double>(1, 2);
    
    double x = (rect.x + rect.width/2 - centerX) / focalLength;
    double y = (rect.y + rect.height/2 - centerY) / focalLength;
    double z = focalLength / rect.width;  // Assuming known average human width

    return cv::Point3f(x, y, z);
}