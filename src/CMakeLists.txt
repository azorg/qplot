set(PROJECT_NAME "qplot")

add_definitions(-D_CRT_SECURE_NO_WARNINGS -DQPLOT_WIN32 -D__STDC_FORMAT_MACROS) 


set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} /NODEFAULTLIB:libc.lib  /NODEFAULTLIB:libcmt.lib /NODEFAULTLIB:msvcrt.lib /NODEFAULTLIB:libcd.lib /NODEFAULTLIB:libcmtd.lib")

set(QT_USE_QTSVG true)

include(${QT_USE_FILE})

include_directories(
${QT_INCLUDES}
${QT_INCLUDE_DIR}
${CMAKE_CURRENT_BINARY_DIR}
${CMAKE_CURRENT_SOURCE_DIR}
${CMAKE_CURRENT_SOURCE_DIR}/../qwt-src
${CMAKE_CURRENT_SOURCE_DIR}/../clib
)

set(Sources
main.cpp
plot_area.cpp
plot_win.cpp
plot_area.h
plot_win.h
qplot.cpp
qplot.h
qplot_tbl.cpp
qplot_tbl.h
)

SET(MOC_HDRS
plot_area.h
plot_win.h
)

QT4_WRAP_CPP(MOC_SRCS ${MOC_HDRS})

SET(UIS
${CMAKE_CURRENT_SOURCE_DIR}/../ui/plot_win.ui
)
QT4_WRAP_UI(UIS_H ${UIS})

add_executable(${PROJECT_NAME} WIN32 ${Sources} ${MOC_SRCS} ${UIS_H} ${RCC_SRCS})

target_link_libraries(${PROJECT_NAME} ${QT_LIBRARIES} ${QT_QTMAIN_LIBRARY} clib qwt)
add_dependencies(${PROJECT_NAME} clib qwt)

install(
TARGETS ${PROJECT_NAME}
RUNTIME
DESTINATION bin
)
