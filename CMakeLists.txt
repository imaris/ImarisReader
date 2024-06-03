cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

set (CMAKE_SYSTEM_VERSION 8.1 CACHE TYPE INTERNAL FORCE)

project(ImarisReader)

if(${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.12.0") 
    cmake_policy(SET CMP0074 NEW)
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS TRUE)

if(APPLE)
    set(CMAKE_OSX_DEPLOYMENT_TARGET 11.7)
    #set(CMAKE_OSX_ARCHITECTURES "x86_64")
    set(CMAKE_OSX_ARCHITECTURES "arm64")
endif()

set(CMAKE_C_STANDARD   99)
set(CMAKE_CXX_STANDARD 14)
enable_language(CXX)

file(GLOB INTERFACE interface/*.h interfaceC/*.h)
file(GLOB HDRS ${INTERFACE} exceptions/*.h reader/*.h types/*.h utils/*.h)
file(GLOB SRCS interface/*.cxx c/*.cxx exceptions/*.cxx reader/*.cxx types/*.cxx utils/*.cxx utils/*.c)

add_definitions(-D_HDF5USEDLL_ -DH5_BUILT_AS_DYNAMIC_LIB)
add_definitions(-D_SBCS -D_SCL_SECURE_NO_WARNINGS)

if(CMAKE_SYSTEM_NAME MATCHES Windows)
    set(CMAKE_CXX_FLAGS "-Zm200 /EHsc /bigobj -w44101 -w44244 -w44267 -w34062 -w34263 -w34265 -w34287 -w34289 -w34296 -w34431 -w34057 -w34092 -w34131 -w34132 -w34189 -w34202 -w34208 -w34245 -w34268 -w34295 -w34389 -w34456 -w34457 -w34505 -w34515 -w34516" CACHE INTERNAL "")
endif()

if(NOT DEFINED CMAKE_MODULE_PATH)
    message(STATUS "define an empty cmake module path")
    set(CMAKE_MODULE_PATH)
endif()
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

find_package(HDF5 REQUIRED)
if(HDF5_FOUND)
    include_directories(${HDF5_INCLUDE_DIR})
    set(_hdf5_libs ${HDF5_LIBRARIES})
    message("Found HDF." + ${HDF5_INCLUDE_DIR} + "  " + ${HDF5_LIBRARIES})
else()
    message("Not found and not installing HDF.")
endif()

find_package(ZLIB REQUIRED)
if(ZLIB_FOUND)
    include_directories(${ZLIB_INCLUDE_DIR})
    message("Found ZLIB." + ${ZLIB_INCLUDE_DIR} + "  " + ${ZLIB_LIBRARY})
else()
    message("Not found and not installing ZLIB.")
endif()

find_package(LZ4 REQUIRED)
if(LZ4_FOUND)
    include_directories(${LZ4_INCLUDE_DIRS})
    message("Found LZ4." + ${LZ4_INCLUDE_DIRS} + "  " + ${LZ4_LIBRARIES})
else()
    message("Not found and not installing LZ4.")
endif()


if(CMAKE_SYSTEM_NAME MATCHES Linux)
	set(Boost_USE_STATIC_LIBS OFF)
	set(Boost_USE_MULTITHREADED ON)
	set(Boost_USE_STATIC_RUNTIME OFF)

	add_definitions( -DBOOST_ALL_NO_LIB )
	add_definitions( -DBOOST_ALL_DYN_LINK )
else()
	set(Boost_USE_STATIC_LIBS ON)
	set(Boost_USE_MULTITHREADED ON)
	set(Boost_USE_STATIC_RUNTIME OFF)
endif()

set(Boost_NO_BOOST_CMAKE ON)
find_package(Boost 1.70 REQUIRED COMPONENTS filesystem)
include_directories(${Boost_INCLUDE_DIRS})
get_filename_component(Boost_INCLUDE_PARENT_DIR ${Boost_INCLUDE_DIRS} DIRECTORY)
include_directories(${Boost_INCLUDE_PARENT_DIR})
message(STATUS "Boost_VERSION: ${Boost_VERSION}")
message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")


set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g -DDEBUG -D_DEBUG")

get_filename_component(SUBDIR_LIBS ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
include_directories(${SUBDIR_LIBS})
include_directories(${SUBDIR_LIBS}/ImarisReader)

include_directories(${HDF5EXT_ROOT}/src)
message(STATUS "HDF5EXT_DIR: ${HDF5EXT_ROOT}")

set(tgt ImarisReader_static)
add_library(${tgt} STATIC ${SRCS} ${HDRS})
target_include_directories(${tgt} PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(${tgt} ${_hdf5_libs} ${ZLIB_LIBRARY} ${LZ4_LIBRARIES} ${Boost_LIBRARIES})

set(tgt bpImarisReader)
add_library(${tgt} SHARED ${SRCS} ${HDRS})
target_include_directories(${tgt} PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_compile_definitions(${tgt} PRIVATE COMPILE_SHARED_LIBRARY)
target_link_libraries(${tgt} ${_hdf5_libs} ${ZLIB_LIBRARY} ${LZ4_LIBRARIES} ${Boost_LIBRARIES})

message("Found build." + ${CMAKE_BINARY_DIR})
if(${CMAKE_PROJECT_NAME} STREQUAL ImarisReader)
    install(FILES ${INTERFACE} DESTINATION ${CMAKE_BINARY_DIR}/include)
    install (TARGETS bpImarisReader
         ARCHIVE DESTINATION ${CMAKE_BINARY_DIR}/lib
         LIBRARY DESTINATION ${CMAKE_BINARY_DIR}/lib
         RUNTIME DESTINATION ${CMAKE_BINARY_DIR}/bin)
    
    install (TARGETS ImarisReader_static
         ARCHIVE DESTINATION ${CMAKE_BINARY_DIR}/lib
         LIBRARY DESTINATION ${CMAKE_BINARY_DIR}/lib)    
else()
    message("Not installing ImarisReader libraries. CMAKE_PROJECT_NAME is ${CMAKE_PROJECT_NAME}")
endif()
