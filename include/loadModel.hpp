#ifndef LOAD_MODEL_HPP
#define LOAD_MODEL_HPP

#include <vector>
#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>

/**
 * @class loadModel
 * @brief A class to load a deep learning model for object detection.
 */
class loadModel {
public:
    /**
     * @brief Constructor for the loadModel class.
     * 
     * @param model_file_path Path to the model file.
     * @param config_file_path Path to the configuration file.
     * @param classes_file_path Path to the file containing class labels.
     */
    loadModel(const std::string& model_file_path , const std::string& config_file_path, const std::string& classes_file_path);

    cv::dnn::Net net; ///< The neural network for object detection.
    
    std::vector<std::string> classLabels; ///< Vector storing the class labels.

    /**
     * @brief Load the model from the specified files.
     * 
     * @return true if the model was loaded successfully, false otherwise.
     */
    bool loadFromFile();

    cv::Mat Camera_Matrix; ///< Camera intrinsic parameters matrix.
    
    cv::Mat Dist_Coeffs; ///< Distortion coefficients matrix.

private:
    const std::string model_file_path; ///< Path to the loaded model file.
    
    const std::string config_file_path; ///< Path to the configuration file.
    
    const std::string classes_file_path; ///< Path to the class labels file.
};

#endif // LOAD_MODEL_HPP
