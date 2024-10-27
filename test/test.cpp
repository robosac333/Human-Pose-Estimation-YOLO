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

// Test model loading with invalid paths
// TEST_F(LoadModelTest, InvalidPathTest) {
//     loadModel model("invalid.weights", "invalid.cfg", "invalid.txt");
//     EXPECT_THROW(model.loadFromFile(), std::runtime_error);
// }

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

// Test camera matrix initialization
// TEST_F(LoadModelTest, CameraMatrixTest) {
//     loadModel model(modelPath, configPath, classesPath);
//     model.loadFromFile();
    
//     // Check camera matrix dimensions
//     EXPECT_EQ(model.Camera_Matrix.rows, 3);
//     EXPECT_EQ(model.Camera_Matrix.cols, 3);
    
//     // Check specific values
//     EXPECT_DOUBLE_EQ(model.Camera_Matrix.at<double>(0,0), 1000.0);
//     EXPECT_DOUBLE_EQ(model.Camera_Matrix.at<double>(1,1), 1000.0);
//     EXPECT_DOUBLE_EQ(model.Camera_Matrix.at<double>(0,2), 320.0);
//     EXPECT_DOUBLE_EQ(model.Camera_Matrix.at<double>(1,2), 240.0);
//     EXPECT_DOUBLE_EQ(model.Camera_Matrix.at<double>(2,2), 1.0);
// }

// Test distortion coefficients initialization
// TEST_F(LoadModelTest, DistCoeffsTest) {
//     loadModel model(modelPath, configPath, classesPath);
//     model.loadFromFile();
    
//     // Check distortion coefficients dimensions
//     EXPECT_EQ(model.Dist_Coeffs.rows, 1);
//     EXPECT_EQ(model.Dist_Coeffs.cols, 5);
    
//     // Check all values are zero
//     for(int i = 0; i < 5; i++) {
//         EXPECT_DOUBLE_EQ(model.Dist_Coeffs.at<double>(0,i), 0.0);
//     }
// }

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

// Test exception messages
// TEST_F(LoadModelTest, ExceptionMessagesTest) {
//     loadModel model("invalid.weights", "invalid.cfg", "invalid.txt");
//     try {
//         model.loadFromFile();
//         FAIL() << "Expected std::runtime_error";
//     }
//     catch(const std::runtime_error& e) {
//         EXPECT_PRED_FORMAT2(testing::IsSubstring, "Failed to load", e.what());
//     }
// }
