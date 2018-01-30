if (BUILD_SHARED_LIBS)
   add_definitions(-DPS_DLL)
endif()

include_directories("../../include")

file(GLOB FORM_SOURCES "*.cpp")

add_executable(Form ${FORM_SOURCES})

target_link_libraries(Form PicoServer)

install(TARGETS Form DESTINATION ${INSTALL_DIR})