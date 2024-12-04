file(GLOB CPP_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB H_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

target_sources(map_editor
    PRIVATE
    ${CPP_FILES}
    PUBLIC
    ${H_FILES}
    EditorWindow.ui
    )

# Buscar Qt
#find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
#find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
    
# Crear el ejecutable
#qt_add_executable(map_editor
#    MANUAL_FINALIZATION
#    ${PROJECT_SOURCES}
#)
    
# Incluir y vincular Qt Widgets
target_include_directories(map_editor PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(map_editor PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Instalación
include(GNUInstallDirs)
install(TARGETS map_editor
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
    
# Finalizar el ejecutable para Qt 6
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(map_editor)
endif()