add_library(example)
add_library(RainGauge)

target_sources(example
  PRIVATE
    example.cpp
  PUBLIC
    example.h
  )

target_sources(RainGauge
  PRIVATE
    RainGauge.cpp
  PUBLIC
    RainGauge.h
)

target_include_directories(example
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
  )

target_include_directories(RainGauge
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)

# we use this to get code coverage
# flags are only valid with the GNU compiler and on Linux
if(CMAKE_CXX_COMPILER_ID MATCHES GNU AND CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
  target_compile_options(example
    PUBLIC
      "--coverage"
  )
  target_link_options(example
    INTERFACE
      "--coverage"
  )
  
  target_compile_options(RainGauge
    PUBLIC
      "--coverage"
  )
  target_link_options(RainGauge
    INTERFACE
      "--coverage"
  )

endif()
