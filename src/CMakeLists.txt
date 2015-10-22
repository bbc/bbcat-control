#sources
set(_sources
	AudioObjectParameters.cpp
	${CMAKE_CURRENT_BINARY_DIR}/version.cpp
)

# public headers
set(_headers
	AudioObject.h
	AudioObjectCursor.h
	AudioObjectParameters.h
	${CMAKE_CURRENT_BINARY_DIR}/version.h
)

#include all the parts that are consistent across all libraries
include(CMakeLists-src.txt)

TARGET_LINK_LIBRARIES(bbcat-control bbcat-dsp bbcat-base)
