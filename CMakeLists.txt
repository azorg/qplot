project(qplot)

cmake_minimum_required(VERSION 2.6)

if(CMAKE_GENERATOR MATCHES "Visual Studio 10")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/.runtime)
endif(CMAKE_GENERATOR MATCHES "Visual Studio 10")

if(CMAKE_GENERATOR MATCHES "Visual Studio 9 2008")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/.runtime)
endif(CMAKE_GENERATOR MATCHES "Visual Studio 9 2008")

if(CMAKE_GENERATOR MATCHES "MinGW Makefiles")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/.runtime/mingw)
endif(CMAKE_GENERATOR MATCHES "MinGW Makefiles")

if(CMAKE_GENERATOR MATCHES "Unix Makefiles")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/.runtime/unix)
endif(CMAKE_GENERATOR MATCHES "Unix Makefiles")


find_package(Qt4)
if(QT4_FOUND)

add_definitions(-D_CRT_SECURE_NO_WARNINGS) 

# --- start of clib ---
set(PROJECT_NAME "clib")

set(Sources
./clib/aini.h
./clib/fsize.c
./clib/fsize.h
./clib/ini.c
./clib/ini.h
./clib/str.c
./clib/str.h
)

SET_SOURCE_FILES_PROPERTIES(${Sources} PROPERTIES LANGUAGE CXX)

add_library(${PROJECT_NAME} STATIC ${Sources})
# --- finish of clib ---


# --- start of qwt ---
set(PROJECT_NAME "qwt")

include(${QT_USE_FILE})

include_directories(
${QT_INCLUDES}
${QT_INCLUDE_DIR}
)

set(Sources
./qwt-src/qwt_abstract_legend.cpp
./qwt-src/qwt_abstract_scale_draw.cpp
./qwt-src/qwt_curve_fitter.cpp
./qwt-src/qwt_clipper.cpp
./qwt-src/qwt_color_map.cpp
./qwt-src/qwt_dyngrid_layout.cpp
./qwt-src/qwt_event_pattern.cpp
./qwt-src/qwt_graphic.cpp
./qwt-src/qwt_interval.cpp
./qwt-src/qwt_legend.cpp
./qwt-src/qwt_legend_data.cpp
./qwt-src/qwt_legend_label.cpp
./qwt-src/qwt_magnifier.cpp
./qwt-src/qwt_math.cpp
./qwt-src/qwt_null_paintdevice.cpp
./qwt-src/qwt_painter_command.cpp
./qwt-src/qwt_painter.cpp
./qwt-src/qwt_panner.cpp
./qwt-src/qwt_picker.cpp
./qwt-src/qwt_picker_machine.cpp
./qwt-src/qwt_plot_canvas.cpp
./qwt-src/qwt_plot.cpp
./qwt-src/qwt_plot_axis.cpp
./qwt-src/qwt_plot_curve.cpp
./qwt-src/qwt_plot_dict.cpp
./qwt-src/qwt_plot_grid.cpp
./qwt-src/qwt_plot_item.cpp
./qwt-src/qwt_plot_layout.cpp
./qwt-src/qwt_plot_legenditem.cpp
./qwt-src/qwt_plot_magnifier.cpp
./qwt-src/qwt_plot_marker.cpp
./qwt-src/qwt_plot_panner.cpp
./qwt-src/qwt_plot_picker.cpp
./qwt-src/qwt_plot_seriesitem.cpp
./qwt-src/qwt_plot_renderer.cpp
./qwt-src/qwt_plot_zoomer.cpp
./qwt-src/qwt_point_data.cpp
./qwt-src/qwt_point_mapper.cpp
./qwt-src/qwt_point_polar.cpp
./qwt-src/qwt_point_3d.cpp
./qwt-src/qwt_pixel_matrix.cpp
./qwt-src/qwt_spline.cpp
./qwt-src/qwt_scale_div.cpp
./qwt-src/qwt_scale_draw.cpp
./qwt-src/qwt_scale_engine.cpp
./qwt-src/qwt_scale_map.cpp
./qwt-src/qwt_scale_widget.cpp
./qwt-src/qwt_series_data.cpp
./qwt-src/qwt_symbol.cpp
./qwt-src/qwt_text.cpp
./qwt-src/qwt_text_engine.cpp
./qwt-src/qwt_text_label.cpp
./qwt-src/qwt_transform.cpp
./qwt-src/qwt_widget_overlay.cpp

./qwt-src/qwt_abstract_legend.h
./qwt-src/qwt_abstract_scale_draw.h
./qwt-src/qwt_curve_fitter.h
./qwt-src/qwt_clipper.h
./qwt-src/qwt_color_map.h
./qwt-src/qwt_dyngrid_layout.h
./qwt-src/qwt_event_pattern.h
./qwt-src/qwt_global.h
./qwt-src/qwt_graphic.h
./qwt-src/qwt_interval.h
./qwt-src/qwt_legend_data.h
./qwt-src/qwt_legend.h
./qwt-src/qwt_legend_label.h
./qwt-src/qwt_magnifier.h
./qwt-src/qwt_math.h
./qwt-src/qwt_null_paintdevice.h
./qwt-src/qwt_painter_command.h
./qwt-src/qwt_painter.h
./qwt-src/qwt_panner.h
./qwt-src/qwt_picker.h
./qwt-src/qwt_plot_seriesitem.h
./qwt-src/qwt_picker_machine.h
./qwt-src/qwt_plot_canvas.h
./qwt-src/qwt_plot_curve.h
./qwt-src/qwt_plot_dict.h
./qwt-src/qwt_plot_grid.h
./qwt-src/qwt_plot.h
./qwt-src/qwt_plot_item.h
./qwt-src/qwt_plot_layout.h
./qwt-src/qwt_plot_legenditem.h
./qwt-src/qwt_plot_magnifier.h
./qwt-src/qwt_plot_marker.h
./qwt-src/qwt_plot_panner.h
./qwt-src/qwt_plot_picker.h
./qwt-src/qwt_plot_renderer.h
./qwt-src/qwt_plot_zoomer.h
./qwt-src/qwt_point_data.h
./qwt-src/qwt_point_mapper.h
./qwt-src/qwt_point_polar.h
./qwt-src/qwt_point_3d.h
./qwt-src/qwt_pixel_matrix.h
./qwt-src/qwt_spline.h
./qwt-src/qwt_scale_div.h
./qwt-src/qwt_samples.h
./qwt-src/qwt_scale_draw.h
./qwt-src/qwt_scale_engine.h
./qwt-src/qwt_scale_map.h
./qwt-src/qwt_scale_widget.h
./qwt-src/qwt_series_data.h
./qwt-src/qwt_series_store.h
./qwt-src/qwt_symbol.h
./qwt-src/qwt_text_engine.h
./qwt-src/qwt_text.h
./qwt-src/qwt_text_label.h
./qwt-src/qwt_transform.h
./qwt-src/qwt_widget_overlay.h
)

SET(MOC_HDRS
./qwt-src/qwt_abstract_legend.h
./qwt-src/qwt_dyngrid_layout.h
./qwt-src/qwt_legend.h
./qwt-src/qwt_legend_label.h
./qwt-src/qwt_magnifier.h
./qwt-src/qwt_panner.h
./qwt-src/qwt_picker.h
./qwt-src/qwt_plot_canvas.h
./qwt-src/qwt_plot.h
./qwt-src/qwt_plot_magnifier.h
./qwt-src/qwt_plot_panner.h
./qwt-src/qwt_plot_picker.h
./qwt-src/qwt_plot_renderer.h
./qwt-src/qwt_plot_zoomer.h
./qwt-src/qwt_scale_widget.h
./qwt-src/qwt_text_label.h
)

QT4_WRAP_CPP(MOC_SRCS ${MOC_HDRS})

add_library(${PROJECT_NAME} STATIC ${Sources} ${MOC_SRCS})
# --- finish of qwt ---


# --- start of qplot ---
set(PROJECT_NAME "qplot")

add_definitions(-DQPLOT_WIN32 -D__STDC_FORMAT_MACROS) 

set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} /NODEFAULTLIB:libc.lib  /NODEFAULTLIB:libcmt.lib /NODEFAULTLIB:msvcrt.lib /NODEFAULTLIB:libcd.lib /NODEFAULTLIB:libcmtd.lib")

set(QT_USE_QTSVG true)

include(${QT_USE_FILE})

include_directories(
${QT_INCLUDES}
${QT_INCLUDE_DIR}
${CMAKE_CURRENT_BINARY_DIR}
${CMAKE_CURRENT_SOURCE_DIR}
${CMAKE_CURRENT_SOURCE_DIR}/qwt-src
${CMAKE_CURRENT_SOURCE_DIR}/clib
${CMAKE_CURRENT_SOURCE_DIR}/src
#${CMAKE_CURRENT_SOURCE_DIR}/../qwt-src
#${CMAKE_CURRENT_SOURCE_DIR}/../clib
)

set(Sources
./src/main.cpp
./src/plot_area.cpp
./src/plot_win.cpp
./src/plot_area.h
./src/plot_win.h
./src/qplot.cpp
./src/qplot.h
./src/qplot_tbl.cpp
./src/qplot_tbl.h
)

SET(MOC_HDRS
./src/plot_area.h
./src/plot_win.h
)

QT4_WRAP_CPP(MOC_SRCS ${MOC_HDRS})

SET(UIS
#${CMAKE_CURRENT_SOURCE_DIR}/../ui/plot_win.ui
${CMAKE_CURRENT_SOURCE_DIR}/ui/plot_win.ui
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
# --- finish of qplot ---



endif(QT4_FOUND)
