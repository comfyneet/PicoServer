include_directories("../../include")
include_directories(".")

file(GLOB PS_SOURCES "../../include/PicoServer/*.hpp" "*.cpp")

if (WIN32)
   add_definitions(-DNOMINMAX)
   add_definitions(-DWIN32_LEAN_AND_MEAN)
   #add_definitions(-D_WINSOCK_DEPRECATED_NO_WARNINGS)

   file(GLOB PS_WIN32SOURCES "Win32/*.hpp" "Win32/*.cpp")
   set(PS_SOURCES ${PS_SOURCES} ${PS_WIN32SOURCES})
else()
   file(GLOB PS_UNIXSOURCES "Unix/*.hpp" "Unix/*.cpp")
   set(PS_SOURCES ${PS_SOURCES} ${PS_UNIXSOURCES})
endif()

if (BUILD_SHARED_LIBS)
   add_definitions(-DPS_BUILD_DLL)
   add_library(PicoServer SHARED ${PS_SOURCES})
else()
   add_library(PicoServer STATIC ${PS_SOURCES})
endif()

if (WIN32)
   target_link_libraries(PicoServer ws2_32)
else()
   set(THREADS_PREFER_PTHREAD_FLAG ON)
   find_package(Threads REQUIRED)
   target_link_libraries(PicoServer Threads::Threads)
endif()

install(TARGETS PicoServer DESTINATION ${INSTALL_DIR})
