#include <gtest/gtest.h>
#include<gmock/gmock.h>

#include "../include/loadModel.hpp"
#include "../include/detectHuman.hpp"
#include"../include/Tracker.hpp"
#include <opencv2/opencv.hpp>
#include <filesystem>
#include<fstream>


std::vector<int> _resolution = {1280, 720};
const float pixel_size = 0.0028;
const float _height = 0.762;
const float _focal_length = 1.898;
const float _vfov = 56.34; 

class LoadModelTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup test paths
        modelPath =  "/home/navdeep/Project/Monocular-Human-Detection-YOLO/yolo_classes/yolov3.weights";
        configPath =  "/home/navdeep/Project/Monocular-Human-Detection-YOLO/yolo_classes/yolov3.cfg";
        classesPath =  "/home/navdeep/Project/Monocular-Human-Detection-YOLO/yolo_classes/coco.names";
    }

    std::string modelPath;
    std::string configPath;
    std::string classesPath;
};

// Test constructor
TEST_F(LoadModelTest, ConstructorTest) {
    EXPECT_NO_THROW({
        loadModel model(modelPath, configPath, classesPath);
    });
}

// Test model loading with valid paths
TEST_F(LoadModelTest, ValidLoadTest) {
    loadModel model(modelPath, configPath, classesPath);
    bool result = false;
    EXPECT_NO_THROW({
        result = model.loadFromFile();
    });
    EXPECT_TRUE(result);
}

// Test class labels loading
TEST_F(LoadModelTest, ClassLabelsTest) {
    loadModel model(modelPath, configPath, classesPath);
    model.loadFromFile();
    EXPECT_FALSE(model.classLabels.empty());
}

// Test network configuration
TEST_F(LoadModelTest, NetworkConfigTest) {
    loadModel model(modelPath, configPath, classesPath);
    model.loadFromFile();
    EXPECT_FALSE(model.net.empty());
}

// Test file existence
TEST_F(LoadModelTest, FileExistenceTest) {
    EXPECT_TRUE(std::filesystem::exists(modelPath)) 
        << "Model file does not exist: " << modelPath;
    EXPECT_TRUE(std::filesystem::exists(configPath)) 
        << "Config file does not exist: " << configPath;
    EXPECT_TRUE(std::filesystem::exists(classesPath)) 
        << "Classes file does not exist: " << classesPath;
}

// Test class labels content
TEST_F(LoadModelTest, ClassLabelsContentTest) {
    loadModel model(modelPath, configPath, classesPath);
    model.loadFromFile();
    
    // Test specific class labels if you know they should exist
    std::vector<std::string> expectedLabels = {"person", "car", "dog"};  // example labels
    for(const auto& label : expectedLabels) {
        EXPECT_NE(std::find(model.classLabels.begin(), 
                           model.classLabels.end(), 
                           label), 
                 model.classLabels.end())
            << "Expected label '" << label << "' not found";
    }
}

class detectHumanTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup test paths
        modelPath =  "/home/navdeep/Project/Monocular-Human-Detection-YOLO/yolo_classes/yolov3.weights";
        configPath =  "/home/navdeep/Project/Monocular-Human-Detection-YOLO/yolo_classes/yolov3.cfg";
        classesPath =  "/home/navdeep/Project/Monocular-Human-Detection-YOLO/yolo_classes/coco.names";
        image_path = "/home/navdeep/Project/Monocular-Human-Detection-YOLO/yolo_classes/bus.jpg";
        
        // Load test image in SetUp
        image = cv::imread(image_path);
        ASSERT_FALSE(image.empty()) << "Failed to load test image";
    }

    void TearDown() override {
        // Clear vectors after each test
        boxes.clear();
        confidences.clear();
    }

    std::string modelPath;
    std::string configPath;
    std::string classesPath;
    std::string image_path;
    cv::Mat image;
    std::vector<cv::Rect> boxes;
    std::vector<float> confidences;
};

// Existing constructor test
TEST_F(detectHumanTest, ConstructorTest) {
    EXPECT_NO_THROW({
        detectHuman detector(modelPath, configPath, classesPath);
    });
}

// Test model loading with valid paths
TEST_F(detectHumanTest, HumanDetections) {
    detectHuman detector(modelPath, configPath, classesPath);
    std::vector<cv::Rect> detectedHumans;

    // Test loading
    EXPECT_NO_THROW({
        detector.loadFromFile();
    });

    // Test detection
    EXPECT_NO_THROW({
        detectedHumans = detector.detectHumans(image);
    });
    
    // Check if vector is not empty
    EXPECT_FALSE(detectedHumans.empty()) << "No humans were detected in the image";
    
    // Optionally, you can add more specific checks:
    for(const auto& rect : detectedHumans) {
        // Check if rectangles have valid dimensions
        EXPECT_GT(rect.width, 0) << "Detection width should be positive";
        EXPECT_GT(rect.height, 0) << "Detection height should be positive";
    };
}

// Testing the tracker
class TrackerTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup test paths
        modelPath = "/home/navdeep/Project/Monocular-Human-Detection-YOLO/yolo_classes/yolov3.weights";
        configPath = "/home/navdeep/Project/Monocular-Human-Detection-YOLO/yolo_classes/yolov3.cfg";
        classesPath = "/home/navdeep/Project/Monocular-Human-Detection-YOLO/yolo_classes/coco.names";
        image_path = "/home/navdeep/Project/Monocular-Human-Detection-YOLO/yolo_classes/bus.jpg";
        
        // Load test image
        image = cv::imread(image_path);
        ASSERT_FALSE(image.empty()) << "Failed to load test image";
    }

    void TearDown() override {
        // Cleanup if needed
    }

    std::string modelPath;
    std::string configPath;
    std::string classesPath;
    std::string image_path;
    cv::Mat image;
};


// Test Tracker constructor
TEST_F(TrackerTest, ConstructorTest) {
    EXPECT_NO_THROW({
        Tracker tracker(modelPath, configPath, classesPath, image);
    });
}


// Test angle conversion functions
TEST_F(TrackerTest, AngleConversionTest) {
    Tracker tracker(modelPath, configPath, classesPath, image);
    
    // Test degrees to radians
    float degrees = 180.0f;
    float radians = tracker.degrees_to_radians(degrees);
    EXPECT_FLOAT_EQ(radians, M_PI) << "Incorrect degrees to radians conversion";
    
    // Test radians to degrees
    float converted_degrees = tracker.radians_to_degrees(radians);
    EXPECT_FLOAT_EQ(converted_degrees, degrees) << "Incorrect radians to degrees conversion";
}


// Test location calculation
TEST_F(TrackerTest, LocationCalculationTest) {
    Tracker tracker(modelPath, configPath, classesPath, image);
    
    // Create a test rectangle in the center of the image
    cv::Rect centerRect(image.cols/2 - 50, image.rows/2 - 50, 100, 100);
    
    cv::Point3f location = tracker.getLocation(centerRect);
    
    // Test that location is reasonable
    EXPECT_GE(location.x, -10.0f) << "X coordinate too small";
    EXPECT_LE(location.x, 10.0f) << "X coordinate too large";
    EXPECT_GT(location.y, 0.0f) << "Y coordinate should be positive";
    EXPECT_GT(location.z, 0.0f) << "Z coordinate should be positive";
}

// Test updateTrackers function
TEST_F(TrackerTest, UpdateTrackersTest) {
    Tracker tracker(modelPath, configPath, classesPath, image);
    
    // Create some test detections
    std::vector<cv::Rect> detections = {
        cv::Rect(100, 100, 50, 100),
        cv::Rect(300, 200, 50, 100)
    };
    
    EXPECT_NO_THROW({
        tracker.updateTrackers(detections, image);
    });
}