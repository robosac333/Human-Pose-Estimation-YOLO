/**
 * @file detectHuman.cpp
 * @author Sachin Jadhav (sjd3333@umd.edu)
 * @brief Implementation of the detectHuman class methods for detecting humans in images.
 * @version 0.1
 * @date 2024-10-30
 */

#include "detectHuman.hpp"

#include <fstream>
#include <iostream>

/**
 * @brief Constructor for detectHuman class.
 * Initializes the base loadModel class and clears detection containers.
 * @param modelPath Path to the pre-trained model file.
 * @param configPath Path to the model configuration file.
 * @param classesPath Path to the file containing class names.
 */
detectHuman::detectHuman(const std::string& modelPath,
                         const std::string& configPath,
                         const std::string& classesPath)
    : loadModel(modelPath, configPath, classesPath) {
  boxes.clear();
  confidences.clear();
}

/**
 * @brief Detects humans in the provided image.
 *
 * Converts the image into a blob, forwards it through the neural network,
 * and retrieves bounding boxes for detected humans. 
 * Utilizes non-maxima suppression to refine detections.
 *
 * @param Image The image frame in which to detect humans.
 * @return A vector of cv::Rect objects, each representing a detected human.
 */
std::vector<cv::Rect> detectHuman::detectHumans(const cv::Mat& Image) {
  std::cout << "Creating blob from image of size: " << Image.size << std::endl;

  // Convert image to blob for DNN input
  cv::Mat blob = cv::dnn::blobFromImage(Image, 1 / 255.0, cv::Size(416, 416),
                                        cv::Scalar(0, 0, 0), true, false);

  // Debugging information for blob shape
  std::cout << "Blob shape: " << blob.size << std::endl;
  std::cout << "Blob channels: " << blob.channels() << std::endl;

  net.setInput(blob);

  // Print network layer information
  std::vector<std::string> layerNames = net.getLayerNames();
  std::cout << "Number of layers: " << layerNames.size() << std::endl;

  std::vector<cv::Mat> outs;
  net.forward(outs, net.getUnconnectedOutLayersNames());

  // Process each output to find human detections
  for (const auto& out : outs) {
    for (int i = 0; i < out.rows; ++i) {
      cv::Mat scores = out.row(i).colRange(5, out.cols);
      cv::Point classIdPoint;
      double confidence;
      cv::minMaxLoc(scores, 0, &confidence, 0, &classIdPoint);

      // If confidence is above threshold and detected class is "person"
      if (confidence > 0.5 && classLabels[classIdPoint.x] == "person") {
        int centerX = static_cast<int>(out.at<float>(i, 0) * Image.cols);
        int centerY = static_cast<int>(out.at<float>(i, 1) * Image.rows);
        int width = static_cast<int>(out.at<float>(i, 2) * Image.cols);
        int height = static_cast<int>(out.at<float>(i, 3) * Image.rows);
        int left = centerX - width / 2;
        int top = centerY - height / 2;

        boxes.push_back(cv::Rect(left, top, width, height));
        confidences.push_back(static_cast<float>(confidence));
      }
    }
  }

  // Perform Non-Maximum Suppression to filter overlapping boxes
  std::vector<int> indices;
  cv::dnn::NMSBoxes(boxes, confidences, 0.5, 0.4, indices);

  // Gather final detections after suppression
  std::vector<cv::Rect> detectedHumans;
  for (int idx : indices) {
    detectedHumans.push_back(boxes[idx]);
  }

  return detectedHumans;
}
