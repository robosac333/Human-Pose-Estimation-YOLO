/**
 * @file Detector.cpp
 * @author Sachin Jadhav (sjd3333@umd.edu)
 * @brief Implementation of the Detector class methods for human detection
 * @version 0.1
 * @date 2024-10-30
 */

#include "Detector.hpp"

#include <fstream>
#include <iostream>

Detector::Detector(const std::string& modelPath, const std::string& configPath,
                   const std::string& classesPath)
    : loadModel(modelPath, configPath, classesPath) {
  boxes.clear();
  confidences.clear();
}

std::vector<cv::Rect> Detector::detectHumans(const cv::Mat& inputImage) {
  std::cout << "Creating blob from input image. Image dimensions: "
            << inputImage.size() << std::endl;

  /**
   * @brief Prepare input image for neural network processing
   * @details Converts image to blob format with following parameters:
   *          - Scale factor: 1.0/255.0 (normalize pixel values)
   *          - Size: 416x416 (YOLO default input size)
   *          - Mean subtraction: (0,0,0)
   *          - BGR2RGB conversion: true
   *          - Crop: false
   */
  cv::Mat imageBlob =
      cv::dnn::blobFromImage(inputImage, 1.0 / 255.0, cv::Size(416, 416),
                             cv::Scalar(0, 0, 0), true, false);

  std::cout << "Blob dimensions: " << imageBlob.size << std::endl;
  std::cout << "Blob channels count: " << imageBlob.channels() << std::endl;

  // Set network input and get layer information
  net.setInput(imageBlob);
  auto allLayerNames = net.getLayerNames();
  std::cout << "Total number of layers in the network: " << allLayerNames.size()
            << std::endl;

  /**
   * @brief Process network output
   * @details Collects output from unconnected layers and processes detection
   * results
   */
  std::vector<cv::Mat> networkOutputs;
  net.forward(networkOutputs, net.getUnconnectedOutLayersNames());

  // Process each detection in the network output
  for (const auto& outputData : networkOutputs) {
    for (int rowIdx = 0; rowIdx < outputData.rows; ++rowIdx) {
      /**
       * @brief Extract and process detection data
       * @details For each detection:
       *          1. Get confidence scores for all classes
       *          2. Find highest confidence score and corresponding class
       *          3. Check if detection is a person with high confidence
       *          4. Calculate bounding box coordinates
       */
      cv::Mat probabilityScores =
          outputData.row(rowIdx).colRange(5, outputData.cols);
      cv::Point maxScorePosition;
      double maxScoreValue;
      cv::minMaxLoc(probabilityScores, nullptr, &maxScoreValue, nullptr,
                    &maxScorePosition);

      // Process detections with high confidence (>0.5) that are classified as
      // people
      if (maxScoreValue > 0.5 && classLabels[maxScorePosition.x] == "person") {
        // Calculate bounding box coordinates
        int xCenter =
            static_cast<int>(outputData.at<float>(rowIdx, 0) * inputImage.cols);
        int yCenter =
            static_cast<int>(outputData.at<float>(rowIdx, 1) * inputImage.rows);
        int boxWidth =
            static_cast<int>(outputData.at<float>(rowIdx, 2) * inputImage.cols);
        int boxHeight =
            static_cast<int>(outputData.at<float>(rowIdx, 3) * inputImage.rows);
        int topLeftX = xCenter - boxWidth / 2;
        int topLeftY = yCenter - boxHeight / 2;

        // Store detection results
        boxes.emplace_back(cv::Rect(topLeftX, topLeftY, boxWidth, boxHeight));
        confidences.emplace_back(static_cast<float>(maxScoreValue));
      }
    }
  }

  /**
   * @brief Apply Non-Maximum Suppression
   * @details Filters overlapping detections to prevent multiple detections
   *          of the same person. Uses confidence threshold of 0.5 and
   *          NMS threshold of 0.4
   */
  std::vector<int> selectedIndices;
  cv::dnn::NMSBoxes(boxes, confidences, 0.5, 0.4, selectedIndices);

  // Collect final detections
  std::vector<cv::Rect> humanDetections;
  for (int selectedIndex : selectedIndices) {
    humanDetections.push_back(boxes[selectedIndex]);
  }

  return humanDetections;
}