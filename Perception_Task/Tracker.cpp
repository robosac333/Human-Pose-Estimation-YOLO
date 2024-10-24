/**
 * @file Tracker.cpp
 * @brief Implementation of the Tracker class methods
 */

#include "Tracker.hpp"
#include <iostream>
#include <fstream>

Tracker::Tracker(const std::string& modelPath,
                const std::string& configPath,
                const std::string& classesPath,
                const cv::Mat& image) : detectHuman(modelPath, configPath, classesPath) {
}

void Tracker::Track(const cv::Mat& Image) {
    // Detect humans in current frame
    std::vector<cv::Rect> detections = detectHumans(Image);
    std::cout << "Detected Humans" << std::endl;
    
    // Update tracking information
    updateTrackers(detections, Image);

    // Draw bounding boxes and location information for detected humans
    for (const auto& detection : detections) {
        cv::rectangle(Image, detection, cv::Scalar(255, 255, 0), 2);
        cv::Point3f location = getLocation(detection);
        cv::putText(Image,
                   "Human: (" + std::to_string(int(location.x)) + ", " +
                               std::to_string(int(location.y)) + ", " +
                               std::to_string(int(location.z)) + ")",
                   cv::Point(detection.x, detection.y - 10),
                   cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0), 2);
    }
}

void Tracker::updateTrackers(const std::vector<cv::Rect>& detections, const cv::Mat& Image) {
    // Update existing trackers and remove failed ones
    auto it = trackers.begin();
    while (it != trackers.end()) {
        cv::Rect trackedRect;
        if ((*it)->update(Image, trackedRect)) {
            // Draw tracking information for successful trackers
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
        // Check if detection overlaps with any existing tracker
        for (const auto& tracker : trackers) {
            cv::Rect trackedRect;
            tracker->update(Image, trackedRect);
            if ((det & trackedRect).area() > 0) {
                isNewDetection = false;
                break;
            }
        }
        // Create new tracker if detection is new
        if (isNewDetection) {
            cv::Ptr<cv::Tracker> tracker = cv::TrackerKCF::create();
            tracker->init(Image, det);
            trackers.push_back(tracker);
        }
    }
}

cv::Point3f Tracker::getLocation(const cv::Rect& rect) {
    // Calculate 3D position using camera parameters
    double focalLength = Camera_Matrix.at<double>(0, 0);
    double centerX = Camera_Matrix.at<double>(0, 2);
    double centerY = Camera_Matrix.at<double>(1, 2);
    
    // Convert pixel coordinates to 3D coordinates
    double x = (rect.x + (double)rect.width/2 - centerX) / focalLength;
    double y = (rect.y + (double)rect.height/2 - centerY) / focalLength;
    double z = focalLength / rect.width; // Assuming known average human width
    
    return cv::Point3f(x, y, z);
}