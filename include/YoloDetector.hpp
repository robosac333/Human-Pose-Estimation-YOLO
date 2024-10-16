#ifndef YOLODETECTOR_HPP
#define YOLODETECTOR_HPP

#include <string>
#include <vector>
#include "Detection.hpp"
#include "ImageProcessor.hpp"
#include "Utils.hpp"

/**
 * @class YoloDetector
 * @brief A class for performing object detection using YOLO (You Only Look Once) models.
 *
 * This class encapsulates methods for loading a YOLO model, preprocessing images, 
 * performing inference, and drawing bounding boxes on detected objects.
 */
class YoloDetector {
private:
    std::string modelPath;            /**< Path to the YOLO model file. */
    std::string onnxModelPath;        /**< Path to the ONNX model file. */
    int inputWidth;                   /**< Width of the input image. */
    int inputHeight;                  /**< Height of the input image. */
    float confidenceThreshold;        /**< Confidence threshold for detections. */
    float nmsThreshold;               /**< Non-maximum suppression (NMS) threshold. */
    std::vector<std::string> classNames; /**< List of class names for detection. */

public:
    /**
     * @brief Constructs a YoloDetector object.
     *
     * @param modelPath The file path to the YOLO model.
     * @param onnxModelPath The file path to the ONNX model.
     * @param inputWidth The width to which the input image is resized.
     * @param inputHeight The height to which the input image is resized.
     * @param confidenceThreshold The confidence threshold for filtering weak detections.
     * @param nmsThreshold The non-maximum suppression threshold.
     * @param classNames The list of class names used for detection.
     */
    YoloDetector(const std::string& modelPath, const std::string& onnxModelPath, int inputWidth, int inputHeight,
                 float confidenceThreshold, float nmsThreshold, const std::vector<std::string>& classNames);

    /**
     * @brief Loads the YOLO model.
     * 
     * This function loads the YOLO model using the provided model paths.
     *
     * @return True if the model is successfully loaded, otherwise false.
     */
    bool loadModel();

    /**
     * @brief Tracks a human in the input image.
     *
     * This function performs tracking of a human in the provided image.
     */
    void trackHuman();

    /**
     * @brief Performs inference on the input image.
     *
     * This function runs the YOLO model on the given image and returns 
     * a list of detections.
     *
     * @param image The input image for inference.
     * @return A vector of Detection objects containing detected bounding boxes and other information.
     */
    std::vector<Detection> infer(const cv::Mat& image);

    /**
     * @brief Preprocesses the input image for YOLO model inference.
     *
     * This function resizes, normalizes, and converts the input image to a blob format suitable for YOLO model inference.
     *
     * @param image The input image to be preprocessed.
     * @return The preprocessed image in blob format.
     */
    cv::Mat preprocess(const cv::Mat& image);

    /**
     * @brief Draws bounding boxes on the image based on detections.
     *
     * This function draws rectangles around detected objects and labels them using the provided detection information.
     *
     * @param image The image on which to draw the bounding boxes.
     * @param detections The list of detected objects to draw.
     */
    void drawBoundingBoxes(cv::Mat& image, const std::vector<Detection>& detections);
};

#endif // YOLODETECTOR_HPP
