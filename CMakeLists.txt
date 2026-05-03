cmake_minimum_required(VERSION 3.25)
project(VoxelE VERSION 1.0 LANGUAGES C CXX) # Renamed project to match your target intent

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Setup GLAD as a separate library to suppress its warnings
add_library(glad src/glad.c)
target_include_directories(glad PUBLIC include)
if(NOT MSVC)
    target_compile_options(glad PRIVATE -w) # Silence the "Hall Monitor" warnings
endif()

# 2. Collect your source files
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS 
    "src/*.cpp" 
    "src/*.c"
)
# Note: Remove src/glad.c from SRC_FILES if GLOB picks it up, 
# otherwise you'll have duplicate symbols. 
list(REMOVE_ITEM SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/glad.c")

# 3. Find Dependencies
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

# 4. Create the Main Executable
add_executable(${PROJECT_NAME} ${SRC_FILES})

# 5. Include Directories
target_include_directories(${PROJECT_NAME} PRIVATE 
    include
    ${OPENGL_INCLUDE_DIR}
)

# 6. Link Everything
target_link_libraries(${PROJECT_NAME} PRIVATE 
    glad        # Links your silenced GLAD library
    glfw        
    glm::glm
    OpenGL::GL
    ${CMAKE_DL_LIBS}
)

# 7. Strict Compiler Flags for YOUR code (but not GLAD)
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()
