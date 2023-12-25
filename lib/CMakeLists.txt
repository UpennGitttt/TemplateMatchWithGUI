cmake_minimum_required (VERSION 3.5)

project(Execution)

file(GLOB_RECURSE SRC_LIST ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp )
file(GLOB Execution_EXPORT_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

add_library(${PROJECT_NAME} SHARED ${SRC_LIST} ${Execution_EXPORT_HEADERS})

target_include_directories(${PROJECT_NAME} PRIVATE
                           ${Vitip_INCLUDE_DIRS}
                           ${OpenCV_INCLUDE_DIRS}
)

target_link_libraries(
	${PROJECT_NAME} PRIVATE
        vitip::vitip3
	${OpenCV_LIBS}
)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)

