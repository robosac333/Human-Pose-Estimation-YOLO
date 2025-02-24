/**
 * @file test.cpp
 * @author Navdeep Singh (nsingh19@umd.edu)
 * @brief Unit tests for model loading, human detection, and tracking
 * functionalities.
 * @version 0.1
 * @date 2024-10-24
 *
 * @copyright Copyright (c) 2024
 */

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <opencv2/opencv.hpp>

#include "../include/Tracker.hpp"
#include "../include/detectHuman.hpp"
#include "../include/loadModel.hpp"

/// Screen resolution used in tests
std::vector<int> _resolution = {1280, 720};

/// Pixel size used in calculations
const float pixel_size = 0.0028;
/// Height parameter for tracking
const float _height = 0.762;
/// Focal length parameter for tracking
const float _focal_length = 1.898;
/// Vertical field of view (VFOV) used in calculations
const float _vfov = 56.34;

/**
 * @class LoadModelTest
 * @brief Unit tests for the `loadModel` class, checking model loading and class
 * label functionalities.
 */
class LoadModelTest : public ::testing::Test {
 protected:
  /**
   * @brief Sets up file paths for model, configuration, and classes.
   */
  void SetUp() override {
    const char* projectRoot = PROJECT_ROOT;
    modelPath = std::string(projectRoot) + "/yolo_classes/yolov3.weights";
    configPath = std::string(projectRoot) + "/yolo_classes/yolov3.cfg";
    classesPath = std::string(projectRoot) + "/yolo_classes/coco.names";
    std::cout << "Calling Fixture SetUp\n";
  }

  std::string modelPath;    ///< Path to the model file
  std::string configPath;   ///< Path to the configuration file
  std::string classesPath;  ///< Path to the classes file
};

/**
 * @test ValidLoadTest
 * @brief Tests if the model loads successfully from valid file paths.
 */
TEST_F(LoadModelTest, ValidLoadTest) {
  loadModel model(modelPath, configPath, classesPath);
  bool result = false;
  EXPECT_NO_THROW({ result = model.loadFromFile(); });
  EXPECT_TRUE(result);
}

/**
 * @test ClassLabelsTest
 * @brief Verifies that class labels are loaded and not empty.
 */
TEST_F(LoadModelTest, ClassLabelsTest) {
  loadModel model(modelPath, configPath, classesPath);
  model.loadFromFile();
  EXPECT_FALSE(model.classLabels.empty());
}

/**
 * @test NetworkConfigTest
 * @brief Tests if the network configuration is set up correctly.
 */
TEST_F(LoadModelTest, NetworkConfigTest) {
  loadModel model(modelPath, configPath, classesPath);
  model.loadFromFile();
  EXPECT_FALSE(model.net.empty());
}

/**
 * @test FileExistenceTest
 * @brief Checks if model, config, and class files exist in the specified paths.
 */
TEST_F(LoadModelTest, FileExistenceTest) {
  EXPECT_TRUE(std::filesystem::exists(modelPath))
      << "Model file does not exist: " << modelPath;
  EXPECT_TRUE(std::filesystem::exists(configPath))
      << "Config file does not exist: " << configPath;
  EXPECT_TRUE(std::filesystem::exists(classesPath))
      << "Classes file does not exist: " << classesPath;
}

/**
 * @test ClassLabelsContentTest
 * @brief Ensures specific expected labels are found in the loaded class labels.
 */
TEST_F(LoadModelTest, ClassLabelsContentTest) {
  loadModel model(modelPath, configPath, classesPath);
  model.loadFromFile();

  std::vector<std::string> expectedLabels = {"person", "car", "dog"};
  for (const auto& label : expectedLabels) {
    EXPECT_NE(
        std::find(model.classLabels.begin(), model.classLabels.end(), label),
        model.classLabels.end())
        << "Expected label '" << label << "' not found";
  }
}

/**
 * @class detectHumanTest
 * @brief Unit tests for the `detectHuman` class, focusing on human detection
 * functionalities.
 */
class detectHumanTest : public ::testing::Test {
 protected:
  /**
   * @brief Sets up file paths for the model, configuration, classes, and loads
   * a test image.
   */
  void SetUp() override {
    const char* projectRoot = PROJECT_ROOT;
    modelPath = std::string(projectRoot) + "/yolo_classes/yolov3.weights";
    configPath = std::string(projectRoot) + "/yolo_classes/yolov3.cfg";
    classesPath = std::string(projectRoot) + "/yolo_classes/coco.names";
    image_path = std::string(projectRoot) + "/yolo_classes/bus.jpg";

    image = cv::imread(image_path);
    ASSERT_FALSE(image.empty()) << "Failed to load test image";
  }

  std::string modelPath;           ///< Path to the model file
  std::string configPath;          ///< Path to the configuration file
  std::string classesPath;         ///< Path to the classes file
  std::string image_path;          ///< Path to the test image file
  cv::Mat image;                   ///< Loaded test image
  std::vector<cv::Rect> boxes;     ///< Vector of detected bounding boxes
  std::vector<float> confidences;  ///< Confidence scores of detections
};

/**
 * @test HumanDetections
 * @brief Tests the detection of humans within a test image.
 */
TEST_F(detectHumanTest, HumanDetections) {
  detectHuman detector(modelPath, configPath, classesPath);
  std::vector<cv::Rect> detectedHumans;

  EXPECT_NO_THROW({ detector.loadFromFile(); });
  EXPECT_NO_THROW({ detectedHumans = detector.detectHumans(image); });
  EXPECT_FALSE(detectedHumans.empty())
      << "No humans were detected in the image";

  for (const auto& rect : detectedHumans) {
    EXPECT_GT(rect.width, 0) << "Detection width should be positive";
    EXPECT_GT(rect.height, 0) << "Detection height should be positive";
  }
}

/**
 * @class TrackerTest
 * @brief Unit tests for the `Tracker` class, checking tracking functionalities.
 */
class TrackerTest : public ::testing::Test {
 protected:
  /**
   * @brief Sets up file paths for the model, configuration, classes, and loads
   * a test image.
   */
  void SetUp() override {
    const char* projectRoot = PROJECT_ROOT;
    modelPath = std::string(projectRoot) + "/yolo_classes/yolov3.weights";
    configPath = std::string(projectRoot) + "/yolo_classes/yolov3.cfg";
    classesPath = std::string(projectRoot) + "/yolo_classes/coco.names";
    image_path = std::string(projectRoot) + "/yolo_classes/bus.jpg";

    image = cv::imread(image_path);
    ASSERT_FALSE(image.empty()) << "Failed to load test image";
  }

  std::string modelPath;    ///< Path to the model file
  std::string configPath;   ///< Path to the configuration file
  std::string classesPath;  ///< Path to the classes file
  std::string image_path;   ///< Path to the test image file
  cv::Mat image;            ///< Loaded test image
};

/**
 * @test ConstructorTest
 * @brief Tests the Tracker constructor initialization
 */
TEST_F(TrackerTest, ConstructorTest) {
  EXPECT_NO_THROW(
      { Tracker tracker(modelPath, configPath, classesPath, image); });
}

/**
 * @test UpdateTrackersTest
 * @brief Tests the `updateTrackers` function by updating trackers with test
 * detections.
 */
TEST_F(TrackerTest, UpdateTrackersTest) {
  Tracker tracker(modelPath, configPath, classesPath, image);
  std::vector<cv::Rect> detections = {cv::Rect(100, 100, 50, 100),
                                      cv::Rect(300, 200, 50, 100)};
  EXPECT_NO_THROW({ tracker.updateTrackers(detections, image); });
}

/**
 * @test DegreesToRadiansTest
 * @brief Tests the degrees to radians conversion
 */
TEST_F(TrackerTest, DegreesToRadiansTest) {
  Tracker tracker(modelPath, configPath, classesPath, image);
  // Test common angle conversions
  EXPECT_NEAR(tracker.degrees_to_radians(0), 0.0, 0.0001);
  EXPECT_NEAR(tracker.degrees_to_radians(90), M_PI / 2, 0.0001);
  EXPECT_NEAR(tracker.degrees_to_radians(180), M_PI, 0.0001);
  EXPECT_NEAR(tracker.degrees_to_radians(360), 2 * M_PI, 0.0001);
}

/**
 * @test RadiansToDegreesTest
 * @brief Tests the radians to degrees conversion
 */
TEST_F(TrackerTest, RadiansToDegreesTest) {
  Tracker tracker(modelPath, configPath, classesPath, image);

  // Test common angle conversions
  EXPECT_NEAR(tracker.radians_to_degrees(0), 0.0, 0.0001);
  EXPECT_NEAR(tracker.radians_to_degrees(M_PI / 2), 90.0, 0.0001);
  EXPECT_NEAR(tracker.radians_to_degrees(M_PI), 180.0, 0.0001);
  EXPECT_NEAR(tracker.radians_to_degrees(2 * M_PI), 360.0, 0.0001);
}

/**
 * @test GetLocationTest
 * @brief Tests the 3D location calculation from a detection with basic validity
 * checks
 */
TEST_F(TrackerTest, GetLocationTest) {
  Tracker tracker(modelPath, configPath, classesPath, image);

  // Test with a detection in the center of the image
  cv::Rect center_detection(640, 360, 100, 200);  // Center of 1280x720 image
  cv::Point3f location = tracker.getLocation(center_detection);

  // Basic validity checks
  // Just verify that we get some reasonable non-zero values
  EXPECT_NE(location.x, 0.0f);  // Should return some value
  EXPECT_NE(location.y, 0.0f);  // Should return some value
  EXPECT_NE(location.z, 0.0f);  // Should return some value

  // Verify that different input positions give different outputs
  cv::Rect left_detection(100, 360, 100, 200);
  cv::Point3f left_location = tracker.getLocation(left_detection);

  cv::Rect right_detection(1180, 360, 100, 200);
  cv::Point3f right_location = tracker.getLocation(right_detection);

  // Verify that different inputs produce different outputs
  EXPECT_NE(left_location.x, right_location.x);  // Different x positions should
                                                 // give different x coordinates

  // Verify no NaN or inf values
  EXPECT_FALSE(std::isnan(location.x));
  EXPECT_FALSE(std::isnan(location.y));
  EXPECT_FALSE(std::isnan(location.z));
  EXPECT_FALSE(std::isinf(location.x));
  EXPECT_FALSE(std::isinf(location.y));
  EXPECT_FALSE(std::isinf(location.z));
}
