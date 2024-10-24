/**
 * @file Detector.hpp
 * @brief Header file for the Detector class that handles human detection in images
 * @details This class extends loadModel to provide human detection capabilities using
 *          a pre-trained neural network model (YOLO) through OpenCV's DNN module.
 */

#ifndef DETECTOR_HPP
#define DETECTOR_HPP

#include "loadModel.hpp"
#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <vector>
#include <string>

/**
 * @class Detector
 * @brief A class for detecting humans in images using deep learning
 * @details Inherits from loadModel class and implements human detection using
 *          YOLO (You Only Look Once) neural network architecture.
 */
class Detector : public loadModel {
public:
    /**
     * @brief Constructor for the Detector class
     * @param model_file_path Path to the pre-trained neural network model file
     * @param config_file_path Path to the model configuration file
     * @param classes_file_path Path to the file containing class names/labels
     */
    Detector(const std::string& model_file_path,
            const std::string& config_file_path,
            const std::string& classes_file_path);

    /**
     * @brief Detect humans in the input image
     * @param inputImage Input image (cv::Mat) to process
     * @return std::vector<cv::Rect> Vector of bounding boxes for detected humans
     */
    std::vector<cv::Rect> detectHumans(const cv::Mat& inputImage);

    std::vector<float> confidences; ///< Confidence scores for detected objects
    std::vector<cv::Rect> boxes;    ///< Bounding boxes for detected objects
};

#endif // DETECTOR_HPP