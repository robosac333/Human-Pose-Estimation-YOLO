add_library(perception_task STATIC loadModel.cpp detectHuman.cpp Tracker.cpp)
target_include_directories(perception_task PUBLIC 
    ${CMAKE_SOURCE_DIR}/include
    ${OpenCV_INCLUDE_DIRS}
)

target_link_libraries(perception_task PUBLIC ${OpenCV_LIBS})