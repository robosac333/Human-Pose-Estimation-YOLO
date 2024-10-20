#include "../include/Detection.hpp"

Detection::Detection(int classId, float confidence, const cv::Rect& bbox)
    : classId(classId), confidence(confidence), bbox(bbox) {}

std::string Detection::getClassName() const {
    // Stub implementation
    return "";
}
