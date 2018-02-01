if (BUILD_SHARED_LIBS)
   add_definitions(-DPS_DLL)
endif()

include_directories("../../include")

file(GLOB WEB_SOURCES "*.cpp")

add_executable(Web ${WEB_SOURCES})

if (UNIX)
   target_link_libraries(Web stdc++fs)
endif()
target_link_libraries(Web PicoServer)

install(TARGETS Web DESTINATION ${INSTALL_DIR})
install(DIRECTORY "wwwroot" DESTINATION ${INSTALL_DIR})
