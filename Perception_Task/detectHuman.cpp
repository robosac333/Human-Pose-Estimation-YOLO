#include "detectHuman.hpp"
#include <iostream>
#include <fstream>

detectHuman::detectHuman(const std::string& modelPath, 
                        const std::string& configPath, 
                        const std::string& classesPath,
                        const cv::Mat& image)
    : loadModel(modelPath, configPath, classesPath),  // Initialize base class
      Image(image.clone())  // Initialize Image member
{
    boxes.clear();
    confidences.clear();
}

std::vector<cv::Rect> detectHuman::detectHumans(){

    cv::Mat blob = cv::dnn::blobFromImage(Image, 1/255.0, cv::Size(416, 416), cv::Scalar(0,0,0), true, false);
    net.setInput(blob);

    std::vector<cv::Mat> outs;
    net.forward(outs, net.getUnconnectedOutLayersNames());

    for (const auto& out : outs) {
        for (int i = 0; i < out.rows; ++i) {
            cv::Mat scores = out.row(i).colRange(5, out.cols);
            cv::Point classIdPoint;
            double confidence;
            cv::minMaxLoc(scores, 0, &confidence, 0, &classIdPoint);
            
            if (confidence > 0.5 && classes[classIdPoint.x] == "person") {
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

    std::vector<int> indices;
    cv::dnn::NMSBoxes(boxes, confidences, 0.5, 0.4, indices);

    std::vector<cv::Rect> detectedHumans;
    for (int idx : indices) {
        detectedHumans.push_back(boxes[idx]);
    }

    return detectedHumans;
}
