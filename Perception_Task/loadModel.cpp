#include "loadModel.hpp"
#include <iostream>
#include <fstream>

/**
 * @brief Constructor for the loadModel class.
 * 
 * @param modelPath Path to the model file.
 * @param configPath Path to the configuration file.
 * @param classesPath Path to the file containing class labels.
 */
loadModel::loadModel(const std::string& modelPath, const std::string& configPath, const std::string& classesPath)
    : model_file_path(modelPath), 
      config_file_path(configPath),
      classes_file_path(classesPath) {}

/**
 * @brief Load the neural network model and class labels from the specified files.
 * 
 * @return true if the model and labels were loaded successfully, false otherwise.
 * 
 * @throws std::runtime_error if the model or class labels cannot be loaded.
 */
bool loadModel::loadFromFile() {
    // Load the model from the Darknet configuration and model files
    net = cv::dnn::readNetFromDarknet(config_file_path, model_file_path);
    if (net.empty()) {
        std::ostringstream errorMsg;
        errorMsg << "Failed to load the neural network model from the path: " << model_file_path;
        throw std::runtime_error(errorMsg.str());
    }
    std::cout << "Model has been successfully loaded from: " << model_file_path << std::endl;

    // Configure the network to use OpenCV as the backend and run on the CPU
    net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
    net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);

    // Read class names from the specified file if possible
    std::ifstream inputFile(classes_file_path);
    if (inputFile) {
        std::string lineContent;
        while (std::getline(inputFile, lineContent)) {
            classLabels.emplace_back(lineContent);
        }
    } else {
        std::ostringstream errorMsg;
        errorMsg << "Unable to open class names file: " << classes_file_path;
        throw std::runtime_error(errorMsg.str());
    }

    // Set up camera matrix with intrinsic parameters and initialize distortion coefficients to zero
    Camera_Matrix = (cv::Mat_<double>(3, 3) <<
        1000.0, 0.0, 320.0,
        0.0, 1000.0, 240.0,
        0.0, 0.0, 1.0);
    
    Dist_Coeffs = cv::Mat::zeros(cv::Size(5, 1), CV_64F);

    // Return true indicating successful initialization
    return true;
}
