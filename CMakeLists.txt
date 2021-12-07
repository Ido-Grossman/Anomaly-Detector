cmake_minimum_required(VERSION 3.20)
project(IdoSanti)

set(CMAKE_CXX_STANDARD 11)

add_executable(untitled "anomaly_detection_util.cpp" "anomaly_detection_util.h" "AnomalyDetector.h" "MainTrain.cpp"
        "SimpleAnomalyDetector.h" "SimpleAnomalyDetector.cpp" "timeseries.cpp" "timeseries.h" minCircle.cpp minCircle.h)
