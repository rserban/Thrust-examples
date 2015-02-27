CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(thrust_examples)

# ------------------------------------------------------------------------------
# OpenMP
# ------------------------------------------------------------------------------
find_package("OpenMP")

# Add the OpenMP-specific compiler and linker flags
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")

# ------------------------------------------------------------------------------
# CUDA -- required for the Thrust library
# ------------------------------------------------------------------------------
# Use the FindCUDA.cmake to find the CUDA toolkit and set many interesting 
# variables and compilation rules. This saves eons of time.
# Interesting vars that are set: paths CUDA_SDK_ROOT_DIR and CUDA_TOOLKIT_ROOT_DIR

find_package(CUDA QUIET)
IF(CUDA_FOUND)
    message(STATUS "CUDA found")
ELSE()
    message(FATAL_ERROR "CUDA not found!")
ENDIF()

# These need to get straightened out
# Anything using the SDK should only be used by nvcc,
# no need to include them for anything being sent to the system compiler
INCLUDE_DIRECTORIES(
    ${CUDA_TOOLKIT_ROOT_DIR}/include
    ${CUDA_SDK_ROOT_DIR}/common/inc
    ${CUDA_TOOLKIT_INCLUDE}
    )
    
set(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS};-gencode arch=compute_20,code=sm_20;-std=c++11 )
SET( CUDA_PROPAGATE_HOST_FLAGS ON ) 

# ------------------------------------------------------------------------------
SET(DEMOS
    remove_if
    sort_timing
    )

FOREACH(PROGRAM ${DEMOS})
    MESSAGE(STATUS "...add ${PROGRAM}")

    ADD_EXECUTABLE(${PROGRAM}  "${PROGRAM}.cpp")
#    SOURCE_GROUP(""  FILES "${PROGRAM}.cpp")

#    SET_TARGET_PROPERTIES(${PROGRAM} PROPERTIES
#        FOLDER demos
#        COMPILE_FLAGS "${CH_BUILDFLAGS}"
#        LINK_FLAGS "${CH_LINKERFLAG_EXE}"
#    )

#    TARGET_LINK_LIBRARIES(${PROGRAM} ${LIBRARIES})
#    ADD_DEPENDENCIES(${PROGRAM} ${LIBRARIES})

#    INSTALL(TARGETS ${PROGRAM} DESTINATION bin)

ENDFOREACH(PROGRAM)


