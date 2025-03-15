# CMakeList.txt : CMake project for proxyswitch, include source and define

# Add source to this project's executable.
add_executable (proxyswitch WIN32 "proxyswitch.cpp" "proxyswitch.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET proxyswitch PROPERTY CXX_STANDARD 20)
endif()

# Link the wininet library
target_link_libraries(proxyswitch wininet Psapi)

# TODO: Add tests and install targets if needed.
