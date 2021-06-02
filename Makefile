cxx = g++
opencv_include_dir = D:/OpenCV/opencv/build/include
dll = -L D:/OpenCV/opencv/build/x64/MinGW/bin -l libopencv_calib3d450 -llibopencv_core450 -llibopencv_dnn450 -llibopencv_features2d450 -llibopencv_flann450 -llibopencv_gapi450 -llibopencv_highgui450 -llibopencv_imgcodecs450 -llibopencv_imgproc450 -llibopencv_ml450 -llibopencv_objdetect450 -llibopencv_photo450 -llibopencv_stitching450 -llibopencv_video450 -llibopencv_videoio450

CXXFLAGS = -std=c++17 -Wall -I include -I$(opencv_include_dir)

serial_obj = test_serial.o Serial.o
motor_obj = test_motor.o Serial.o
screenshot_obj = test_screenshot.o Screenshot.o
block_detector_obj = test_block_detector.o BlockDetector.o
screen_detector_obj = test_screen_detector.o ScreenDetector.o
io_utils_obj = test_io_utils.o IOUtils.o
pc_game_obj = test_pc_game.o BlockDetector.o Screenshot.o IOUtils.o
main_obj = main.o Serial.o BlockDetector.o ScreenDetector.o


vpath %.cpp src
vpath %.cpp tests
vpath %.h include
vpath %.h $(opencv_include_dir)


dont_tap_while_block: $(main_obj)
	$(cxx) $(dll) -O3 -o build/$@ $(main_obj)

test_pc_game: $(pc_game_obj)
	$(cxx) $(dll) -O3 -o build/$@ $(pc_game_obj) -lgdi32

test_serial: $(serial_obj)
	$(cxx) -o build/$@ $(serial_obj)

test_motor: $(motor_obj)
	$(cxx) -o build/$@ $(motor_obj)

test_io_utils: $(io_utils_obj)
	$(cxx) -o build/$@ $(io_utils_obj)

test_block_detector: $(block_detector_obj)
	$(cxx) $(dll) -O3 -o build/$@ $(block_detector_obj)

test_screen_detector: $(screen_detector_obj)
	$(cxx) $(dll) -O3 -o build/$@ $(screen_detector_obj)

test_screenshot: $(screenshot_obj)
	$(cxx) $(dll) -O3 -o build/$@ $(screenshot_obj) -lgdi32


Serial.o: Serial.h
IOUtils.o: IOUtils.h
Screenshot.o: Screenshot.h
BlockDetector.o: BlockDetector.h

main.o: Serial.h BlockDetector.h ScreenDetector.h
test_serial.o: Serial.h
test_io_utils.o: IOUtils.h
test_screenshot.o: Screenshot.h
test_block_detector.o: BlockDetector.h
test_screen_detector.o: BlockDetector.h
test_motor.o: Serial.h BlockDetector.h
test_pc_game.o: IOUtils.h BlockDetector.h Screenshot.h


.PHONY: clean
clean:
	del $(serial_obj) $(block_detector_obj) $(motor_obj) $(screenshot_obj) $(io_utils_obj) $(pc_game_obj) $(screen_detector_obj) $(main_obj)