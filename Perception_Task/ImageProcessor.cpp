#include "../include/ImageProcessor.hpp"

ImageProcessor::ImageProcessor(const cv::Mat& inputImage)
    : inputImage(inputImage) {}

cv::Mat ImageProcessor::readImage(const std::string& path) {
    // Stub implementation
    return cv::Mat();
}

cv::Mat ImageProcessor::resizeImage(const cv::Mat& image) {
    // Stub implementation
    return cv::Mat();
}

cv::Mat ImageProcessor::normalizeImage(const cv::Mat& image) {
    // Stub implementation
    return cv::Mat();
}

cv::Mat ImageProcessor::convertToBlob(const cv::Mat& image) {
    // Stub implementation
    return cv::Mat();
}
