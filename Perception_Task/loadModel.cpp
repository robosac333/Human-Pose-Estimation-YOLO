#include "loadModel.hpp"
#include <iostream>
#include <fstream>


loadModel::loadModel(const std::string& modelPath, const std::string& configPath, const std::string& classesPath)
    : modelPath(modelPath), 
      configPath(configPath),
      classesPath(classesPath)
{}

bool loadModel::loadFromFile() {
    net = cv::dnn::readNetFromDarknet(configPath, modelPath);
    if (net.empty()) {
        throw std::runtime_error("Failed to load the model from: " + modelPath);
        return false;
    }
    net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
    net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);

    // Load class names
    std::ifstream classNamesFile(classesPath);
    if (classNamesFile.is_open()) {
        std::string className;
        while (std::getline(classNamesFile, className)) {
            classes.push_back(className);
        }
    }else {
    throw std::runtime_error("Failed to load classes");
    return false;
}



    // Initialize camera matrix and distortion coefficients
    cameraMatrix = (cv::Mat_<double>(3,3) << 
        1000, 0, 320,
        0, 1000, 240,
        0, 0, 1);
    distCoeffs = cv::Mat::zeros(5, 1, CV_64F);
    return true;
}


