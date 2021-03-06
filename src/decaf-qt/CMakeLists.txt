project(decaf-qt)
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)

include_directories("src")

file(GLOB_RECURSE SOURCE_FILES src/*.cpp)
file(GLOB_RECURSE HEADER_FILES src/*.h)
file(GLOB_RECURSE UI_FILES ui/*.ui)
file(GLOB_RECURSE QRC_FILES resources/*.qrc)

if(MSVC)
   set(RESOURCE_FILES
      ${CMAKE_SOURCE_DIR}/resources/decaf-sdl.rc
      ${CMAKE_SOURCE_DIR}/resources/hidpi.manifest)
else()
   set(RESOURCE_FILES "")
endif()

qt5_wrap_ui(UIS_HDRS ${UI_FILES})
qt5_add_resources(QT_RESOURCES ${QRC_FILES})

add_executable(decaf-qt
   ${SOURCE_FILES}
   ${HEADER_FILES}
   ${UIS_HDRS}
   ${QT_RESOURCES}
   ${RESOURCE_FILES}
   ${QHEXVIEW_HEADERS}
   ${QHEXVIEW_SOURCES})
GroupSources(resources)
GroupSources(src)
GroupSources(ui)

target_include_directories(decaf-qt PRIVATE
   ${Qt5Gui_PRIVATE_INCLUDE_DIRS}
   ${SDL2_INCLUDE_DIRS})

target_link_libraries(decaf-qt
   common
   libconfig
   libcpu
   libdecaf
   ${CPPTOML_LIBRARIES}
   ${EXCMD_LIBRARIES}
   ${SDL2_LIBRARIES}
   qtadvanceddocking
   Qt5::Core
   Qt5::Concurrent
   Qt5::Svg
   Qt5::Widgets
   Qt5::Xml)

if(MSVC)
   target_link_libraries(decaf-qt
      Setupapi
      Qt5::WinMain)

   set_target_properties(decaf-qt PROPERTIES
      LINK_FLAGS "/SUBSYSTEM:WINDOWS")

   add_custom_command(
       TARGET decaf-qt POST_BUILD
       COMMAND ${CMAKE_COMMAND} -E copy_if_different
           $<TARGET_FILE:Qt5::Core>
           $<TARGET_FILE_DIR:decaf-qt>)

   add_custom_command(
       TARGET decaf-qt POST_BUILD
       COMMAND ${CMAKE_COMMAND} -E copy_if_different
           $<TARGET_FILE:Qt5::Gui>
           $<TARGET_FILE_DIR:decaf-qt>)

   add_custom_command(
       TARGET decaf-qt POST_BUILD
       COMMAND ${CMAKE_COMMAND} -E copy_if_different
           $<TARGET_FILE:Qt5::Svg>
           $<TARGET_FILE_DIR:decaf-qt>)

   add_custom_command(
       TARGET decaf-qt POST_BUILD
       COMMAND ${CMAKE_COMMAND} -E copy_if_different
           $<TARGET_FILE:Qt5::Widgets>
           $<TARGET_FILE_DIR:decaf-qt>)

   add_custom_command(
       TARGET decaf-qt POST_BUILD
       COMMAND ${CMAKE_COMMAND} -E copy_if_different
           $<TARGET_FILE:Qt5::Xml>
           $<TARGET_FILE_DIR:decaf-qt>)

   install(FILES
      $<TARGET_FILE:Qt5::Core>
      $<TARGET_FILE:Qt5::Gui>
      $<TARGET_FILE:Qt5::Svg>
      $<TARGET_FILE:Qt5::Widgets>
      $<TARGET_FILE:Qt5::Xml>
      DESTINATION "${DECAF_INSTALL_BINDIR}")
endif()

install(TARGETS decaf-qt RUNTIME DESTINATION "${DECAF_INSTALL_BINDIR}")
