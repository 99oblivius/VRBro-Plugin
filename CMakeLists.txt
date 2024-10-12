cmake_minimum_required(VERSION 3.16...3.26)

include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/common/bootstrap.cmake" NO_POLICY_SCOPE)

project(${_name} VERSION ${_version})

option(ENABLE_FRONTEND_API "Use obs-frontend-api for UI functionality" OFF)
option(ENABLE_QT "Use Qt functionality" OFF)

include(compilerconfig)
include(defaults)
include(helpers)

# Add FetchContent for Asio and nlohmann/json
include(FetchContent)
FetchContent_Declare(
  asio
  GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
)
FetchContent_Declare(
  json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
)
FetchContent_MakeAvailable(asio json)

add_library(${CMAKE_PROJECT_NAME} MODULE)

find_package(libobs REQUIRED)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE OBS::libobs ws2_32 nlohmann_json::nlohmann_json)

if(ENABLE_FRONTEND_API)
  find_package(obs-frontend-api REQUIRED)
  target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE OBS::obs-frontend-api)
endif()

if(ENABLE_QT)
  find_package(Qt6 COMPONENTS Widgets Core)
  target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Qt6::Core Qt6::Widgets)
  target_compile_options(
    ${CMAKE_PROJECT_NAME} PRIVATE $<$<C_COMPILER_ID:Clang,AppleClang>:-Wno-quoted-include-in-framework-header
                                  -Wno-comma>)
  set_target_properties(
    ${CMAKE_PROJECT_NAME}
    PROPERTIES AUTOMOC ON
               AUTOUIC ON
               AUTORCC ON)
endif()

target_sources(${CMAKE_PROJECT_NAME} PRIVATE src/vrbro.cpp src/network/network.cpp src/config.cpp src/forms/server-settings.cpp src/handler/handler.cpp src/handler/events/request-handlers.cpp src/handler/events/event-handlers.cpp src/handler/errors/handle-errors.cpp)

# Add include directories for Asio and nlohmann/json
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE 
  ${asio_SOURCE_DIR}/include
  ${json_SOURCE_DIR}/include
)

# Add compile definitions for Asio
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE ASIO_STANDALONE)

set_target_properties_plugin(${CMAKE_PROJECT_NAME} PROPERTIES OUTPUT_NAME ${_name})