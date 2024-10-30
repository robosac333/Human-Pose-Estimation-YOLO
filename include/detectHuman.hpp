/**
 * @file detectHuman.hpp
 * @author Sachin Jadhav (sjd3333@umd.edu)
 * @brief Header file for the detectHuman class for detecting humans in images.
 * @version 0.1
 * @date 2024-10-30
 */

#ifndef DETECT_HUMAN_HPP
#define DETECT_HUMAN_HPP

#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <string>
#include <vector>

#include "loadModel.hpp"

/**
 * @class detectHuman
 * @brief Class for detecting humans in images using a pre-trained model.
 *
 * The detectHuman class is derived from the loadModel class and is responsible
 * for performing human detection on a given image. It provides methods to load
 * a model, detect humans, and store the results as bounding boxes and
 * confidence scores.
 */
class detectHuman : public loadModel {
 public:
  /**
   * @brief Constructor for the detectHuman class.
   * @param modelPath Path to the pre-trained model file.
   * @param configPath Path to the model configuration file.
   * @param classesPath Path to the file containing class names.
   */
  detectHuman(const std::string& modelPath, const std::string& configPath,
              const std::string& classesPath);

  /**
   * @brief Detect humans in the provided image.
   * @param Image The image frame in which to detect humans.
   * @return A vector of bounding rectangles (cv::Rect) around detected humans.
   */
  std::vector<cv::Rect> detectHumans(const cv::Mat& Image);

  /**
   * @brief Vector of bounding boxes for detected humans.
   */
  std::vector<cv::Rect> boxes;

  /**
   * @brief Vector of confidence scores corresponding to each detected human.
   */
  std::vector<float> confidences;
};

#endif  // DETECT_HUMAN_HPP
