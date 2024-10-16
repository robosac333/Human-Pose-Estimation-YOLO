#ifndef DETECTION_HPP
#define DETECTION_HPP

#include <opencv2/core.hpp>
#include <string>

/**
 * @class Detection
 * @brief Represents a detection result from the YOLO model.
 *
 * This class contains information about a detected object's class ID, confidence score,
 * and bounding box coordinates.
 */
class Detection {
public:
    /**
     * @brief The ID of the detected class.
     */
    int classId;

    /**
     * @brief The confidence score of the detection, ranging from 0.0 to 1.0.
     */
    float confidence;

    /**
     * @brief The bounding box of the detected object.
     */
    cv::Rect bbox;

    /**
     * @brief Constructs a new Detection object.
     *
     * @param classId The ID of the detected class.
     * @param confidence The confidence score of the detection.
     * @param bbox The bounding box of the detected object.
     */
    Detection(int classId, float confidence, const cv::Rect& bbox);

    /**
     * @brief Gets the class name associated with the detection.
     *
     * @return A string representing the class name.
     */
    std::string getClassName() const;
};

#endif // DETECTION_HPP
