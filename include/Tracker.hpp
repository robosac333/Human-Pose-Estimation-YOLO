/**
 * @file Tracker.hpp
 * @brief Header file for the Tracker class that handles human detection and tracking
 * @details This class extends detectHuman to provide tracking capabilities for detected humans
 * in video streams or image sequences using OpenCV tracking algorithms.
 */

#ifndef TRACKER_HPP
#define TRACKER_HPP

#include "detectHuman.hpp"
#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <vector>
#include <string>

/**
 * @class Tracker
 * @brief A class for detecting and tracking humans in images/video frames
 * @details Inherits from detectHuman class and adds tracking functionality using OpenCV trackers.
 *         Maintains multiple trackers for different detected humans and provides 3D position estimation.
 */
class Tracker : public detectHuman {
public:
    /**
     * @brief Constructor for the Tracker class
     * @param modelPath Path to the neural network model file
     * @param configPath Path to the model configuration file
     * @param classesPath Path to the file containing class names
     * @param image Initial image frame to process
     */
    Tracker(const std::string& modelPath,
            const std::string& configPath,
            const std::string& classesPath,
            const cv::Mat& image);

    /**
     * @brief Process current frame to detect and track humans
     * @param image Current frame to process
     * @details Detects humans in the current frame and updates existing trackers.
     *          Draws bounding boxes and position information on the image.
     */
    void Track(const cv::Mat& image);

    /**
     * @brief Update tracking status for all tracked humans
     * @param detections Vector of detected human bounding boxes
     * @param Image Current frame being processed
     * @details Updates existing trackers, removes failed ones, and initializes
     *          new trackers for newly detected humans.
     */
    void updateTrackers(const std::vector<cv::Rect>& detections, const cv::Mat& Image);

    /**
     * @brief Calculate 3D position of a detected human
     * @param rect Bounding box of the detected human
     * @return cv::Point3f Estimated 3D position (x, y, z) in robot's reference frame
     * @details Uses camera parameters to estimate 3D position from 2D bounding box
     */
    cv::Point3f getLocation(const cv::Rect& rect);

private:
    std::vector<cv::Ptr<cv::Tracker>> trackers; ///< Vector of OpenCV trackers for multiple humans
};

#endif // TRACKER_HPP