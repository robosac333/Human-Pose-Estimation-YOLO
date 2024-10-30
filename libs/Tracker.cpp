/**
 * @file Tracker.cpp
 * @author Sachin Jadhav (sjd3333@umd.edu)
 * @brief Implementation of the Tracker class methods for tracking humans in images.
 * @version 0.1
 * @date 2024-10-30
 */

#include "Tracker.hpp"

#include <fstream>
#include <iostream>

/**
 * @brief Constructor for the Tracker class.
 * @param modelPath Path to the model file used for detection.
 * @param configPath Path to the configuration file for the model.
 * @param classesPath Path to the file listing class names.
 * @param image Initial image frame for setting up trackers.
 */
Tracker::Tracker(const std::string& modelPath, const std::string& configPath,
                 const std::string& classesPath, const cv::Mat& image)
    : detectHuman(modelPath, configPath, classesPath) {}

/**
 * @brief Tracks humans in the given image frame.
 * @param Image The current image frame for detecting and updating human trackers.
 */
void Tracker::Track(const cv::Mat& Image) {
  // Detect humans in current frame
  std::vector<cv::Rect> detections = detectHumans(Image);
  std::cout << "Detected Humans" << std::endl;

  // Update tracking information
  updateTrackers(detections, Image);
}

/**
 * @brief Updates trackers with new detections and removes any that have failed.
 * @param detections Vector of bounding boxes around detected humans.
 * @param Image The current image frame for updating trackers.
 */
void Tracker::updateTrackers(const std::vector<cv::Rect>& detections,
                             const cv::Mat& Image) {
  // Update existing trackers and remove failed ones
  auto it = trackers.begin();
  while (it != trackers.end()) {
    cv::Rect trackedRect;
    if ((*it)->update(Image, trackedRect)) {
      // Draw tracking information for successful trackers
      cv::rectangle(Image, trackedRect, cv::Scalar(255, 0, 0), 2);
      cv::Point3f location = getLocation(trackedRect);
      cv::putText(Image,
                  "Tracked: (" + std::to_string(location.x) + ", " +
                      std::to_string(location.y) + ", " +
                      std::to_string(location.z) + ")",
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

/**
 * @brief Converts degrees to radians.
 * @param deg Angle in degrees.
 * @return Angle in radians.
 */
float Tracker::degrees_to_radians(float deg) { return deg * M_PI / 180.0; }

/**
 * @brief Converts radians to degrees.
 * @param radians Angle in radians.
 * @return Angle in degrees.
 */
float Tracker::radians_to_degrees(float radians) {
  return radians * (180.0 / M_PI);
}

/**
 * @brief Calculates the 3D location of a detected object based on its bounding box.
 * @param detection Bounding box of the detected object.
 * @return 3D coordinates (x, y, z) of the object in the scene.
 */
cv::Point3f Tracker::getLocation(const cv::Rect& detection) {
  cv::Point2f current_pixel;
  current_pixel.x = detection.x + static_cast<double>(detection.width) / 2;
  current_pixel.y = detection.y + static_cast<double>(detection.height) / 2;

  cv::Point3f coordinates;

  std::vector<int> _resolution = {1280, 720};
  const float pixel_size = 0.0028;
  const float _height = 0.762;
  const float _focal_length = 1.898;
  const float _vfov = 56.34;

  float offset_from_center =
      (current_pixel.y - (static_cast<float>(_resolution[1]) / 2)) * pixel_size;

  float dip_angle = (_vfov / 2) - radians_to_degrees(std::atan2(
                                      (offset_from_center), (_focal_length)));

  float z_min_plane =
      _height / tan(degrees_to_radians((_vfov / 2) - dip_angle));

  float x_from_center = (current_pixel.x - 640) * 2.8;

  float x_coord = x_from_center * z_min_plane / (_focal_length * 1000);

  // Assign calculated coordinates
  coordinates.x = x_coord;      // X coordinate
  coordinates.y = _height;      // Y coordinate (height)
  coordinates.z = z_min_plane;  // Z coordinate (depth)

  return coordinates;
}
