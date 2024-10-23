// loadModel.hpp
#ifndef LOAD_MODEL_HPP
#define LOAD_MODEL_HPP

#include <vector>
#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>

class loadModel {
public:
    // Constructor
    loadModel(const std::string& modelPath, const std::string& configPath, const std::string& classesPath);


    cv::dnn::Net net;

    std::vector<std::string> classes;

    // Load the model from a file
    bool loadFromFile();

    cv::Mat cameraMatrix;

    cv::Mat distCoeffs;

private:
    // Path to the loaded model file
    const std::string modelPath;

    // Flag indicating if the model is loaded
    const std:: string configPath;

    // Pointer to the model data
    const std:: string classesPath;
};

#endif // LOAD_MODEL_HPP
