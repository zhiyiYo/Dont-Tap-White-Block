cxx = g++
CXXFLAGS = -g -std=c++17 -I ../include
obj = test_serial.o Serial.o
vpath %.h ../include
vpath %.cpp ../src

test_serial: $(obj)
	$(cxx) -o ../build/$@ $(obj)

test_serial.o: Serial.h
Serial.o: Serial.h

.PHONY: clean
clean:
	del $(obj)