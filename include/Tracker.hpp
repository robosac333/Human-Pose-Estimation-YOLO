#ifndef TRACKER_HPP
#define TRACKER_HPP

#include "detectHuman.hpp"
#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <vector>
#include <string>

class Tracker : public detectHuman {    // Constructor that calls parent constructor
public: 

    Tracker(const std::string& modelPath,
           const std::string& configPath,
           const std::string& classesPath,
           const cv::Mat& image);

    void Track();

    void updateTrackers(const std::vector<cv::Rect>& detections);

    cv::Point3f getLocation(const cv::Rect& rect);

private:

    std::vector<cv::Ptr<cv::Tracker>> trackers;

};