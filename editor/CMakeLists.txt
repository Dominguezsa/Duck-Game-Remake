file(GLOB CPP_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB H_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

target_sources(map_editor
    PRIVATE
    ${CPP_FILES}
    PUBLIC
    ${H_FILES}
    EditorWindow.ui
    )
    
# Include current directory
target_include_directories(map_editor PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(map_editor PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Instalation
include(GNUInstallDirs)
install(TARGETS map_editor
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
    
# Finalize
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(map_editor)
endif()