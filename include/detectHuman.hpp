// detectHuman.hpp
#ifndef DETECT_HUMAN_HPP
#define DETECT_HUMAN_HPP

#include "loadModel.hpp"
#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <vector>
#include <string>


class detectHuman : public loadModel {
public:
    // Constructor accepting modelPath, configPath, and classesPath
    detectHuman(const std::string& modelPath,
                const std::string& configPath,
                const std::string& classesPath,
                const cv::Mat& image);

    // Perform human detection on an image
    std::vector<cv::Rect> detectHumans();

    cv::Mat Image;

    std::vector<cv::Rect> boxes;

    std::vector<float> confidences;

};

#endif // DETECT_HUMAN_HPP
