#ifndef IMAGEPROCESSOR_HPP
#define IMAGEPROCESSOR_HPP

#include <opencv2/core.hpp>
#include <string>

/**
 * @class ImageProcessor
 * @brief Provides various image processing functionalities.
 *
 * This class contains static methods for reading, resizing, normalizing, and 
 * converting images to a blob format, as well as an instance variable for 
 * holding the input image.
 */
class ImageProcessor {
private:
    /**
     * @brief The input image to be processed.
     */
    cv::Mat inputImage;

public:
    /**
     * @brief Constructs an ImageProcessor object with a given input image.
     *
     * @param inputImage The input image to be processed.
     */
    ImageProcessor(const cv::Mat& inputImage);

    /**
     * @brief Reads an image from the specified file path.
     *
     * @param path The file path of the image to read.
     * @return A cv::Mat object representing the read image.
     */
    static cv::Mat readImage(const std::string& path);

    /**
     * @brief Resizes the given image to a specified size.
     *
     * @param image The image to resize.
     * @return A cv::Mat object representing the resized image.
     */
    static cv::Mat resizeImage(const cv::Mat& image);

    /**
     * @brief Normalizes the given image.
     *
     * @param image The image to normalize.
     * @return A cv::Mat object representing the normalized image.
     */
    static cv::Mat normalizeImage(const cv::Mat& image);

    /**
     * @brief Converts the given image to a blob format, typically for use 
     * in deep learning models.
     *
     * @param image The image to convert to a blob.
     * @return A cv::Mat object representing the blob.
     */
    static cv::Mat convertToBlob(const cv::Mat& image);
};

#endif // IMAGEPROCESSOR_HPP
