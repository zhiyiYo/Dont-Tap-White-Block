cxx = g++
opencv_include_dir = D:/OpenCV/opencv/build/include
dll = -L D:/OpenCV/opencv/build/x64/MinGW/bin -l libopencv_calib3d450 -llibopencv_core450 -llibopencv_dnn450 -llibopencv_features2d450 -llibopencv_flann450 -llibopencv_gapi450 -llibopencv_highgui450 -llibopencv_imgcodecs450 -llibopencv_imgproc450 -llibopencv_ml450 -llibopencv_objdetect450 -llibopencv_photo450 -llibopencv_stitching450 -llibopencv_video450 -llibopencv_videoio450

CXXFLAGS = -std=c++17 -I include -I$(opencv_include_dir)
serial_obj = test_serial.o Serial.o
motor_obj = test_motor.o Serial.o BlockDetector.o
block_detector_obj = test_block_detector.o BlockDetector.o

vpath %.cpp src
vpath %.cpp tests
vpath %.h include
vpath %.h $(opencv_include_dir)


test_serial: $(serial_obj)
	$(cxx) -o build/$@ $(serial_obj)

test_motor: $(motor_obj)
	$(cxx) $(dll) -o build/$@ $(motor_obj)

test_block_detector: $(block_detector_obj)
	$(cxx) $(dll) -O3 -o build/$@ $(block_detector_obj)


Serial.o: Serial.h
test_serial.o: Serial.h
BlockDetector.o: BlockDetector.h
test_block_detector.o: BlockDetector.h
test_motor.o: Serial.h BlockDetector.h

.PHONY: clean
clean:
	del $(serial_obj) $(block_detector_obj) $(motor_obj)