#ifndef UTILS_HPP
#define UTILS_HPP

#include <vector>
#include <string>
#include "Detection.hpp"

/**
 * @class Utils
 * @brief A utility class providing methods for loading class names and performing 
 * non-maximum suppression on detections.
 *
 * This class contains static utility functions to support various operations such as 
 * reading class names from a file and applying non-maximum suppression to a list 
 * of detections.
 */
class Utils {
public:
    /**
     * @brief Loads class names from a specified file.
     *
     * Reads a list of class names from the given file path. Each line in the file 
     * should contain a single class name.
     *
     * @param filePath The path to the file containing class names.
     * @return A vector of class names as strings.
     */
    static std::vector<std::string> loadClassNames(const std::string& filePath);

    /**
     * @brief Applies non-maximum suppression to a list of detections.
     *
     * This function filters out overlapping bounding boxes based on their 
     * confidence scores. It retains the detection with the highest score while 
     * removing others that have an IoU (Intersection over Union) greater than the 
     * specified threshold.
     *
     * @param detections A vector of Detection objects representing detected objects.
     * @param nmsThreshold The threshold for non-maximum suppression, used to 
     * determine whether detections are considered overlapping.
     * @return A vector of Detection objects after applying non-maximum suppression.
     */
    static std::vector<Detection> nonMaximumSuppression(const std::vector<Detection>& detections, float nmsThreshold);
};

#endif // UTILS_HPP
