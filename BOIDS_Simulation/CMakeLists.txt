cmake_minimum_required(VERSION 3.0.0)
project(boids VERSION 0.1.0)

include(CTest)
enable_testing()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

string(APPEND CMAKE_CXX_FLAGS " -Wall -Wextra")

find_package(SFML 2.5.1 COMPONENTS graphics REQUIRED)

add_executable(boids main.cpp boids.cpp vector_algebra.cpp button.hpp button.cpp)

# se il testing e' abilitato...
#   per disabilitare il testing, passare -DBUILD_TESTING=OFF a cmake durante la fase di configurazione
if (BUILD_TESTING)

  
  # aggiungi l'eseguibile boids.t
  add_executable(boids.t boids.cpp vector_algebra.cpp button.cpp button.hpp boids.test.cpp)
  # aggiungi la libreria di SFML
  target_link_libraries(boids.t PRIVATE sfml-graphics)
  # aggiungi l'eseguibile boids.t alla lista dei test
  add_test(NAME boids.t COMMAND boids.t)

endif()

target_link_libraries(boids PRIVATE sfml-graphics)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
