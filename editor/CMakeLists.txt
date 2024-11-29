# file(GLOB CPP_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
# file(GLOB H_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

# target_sources(taller_editor
#     PRIVATE
#     ${CPP_FILES}
#     PUBLIC
#     ${H_FILES}
#     mainwindow.ui
#     )

# # Buscar Qt
# #find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
# #find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
    
# # Crear el ejecutable
# #qt_add_executable(taller_editor
# #    MANUAL_FINALIZATION
# #    ${PROJECT_SOURCES}
# #)
    
# # Incluir y vincular Qt Widgets
# target_include_directories(taller_editor PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
# target_link_libraries(taller_editor PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# # Instalación
# include(GNUInstallDirs)
# install(TARGETS taller_editor
#     RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
# )
    
# # Finalizar el ejecutable para Qt 6
# if(QT_VERSION_MAJOR EQUAL 6)
#     qt_finalize_executable(taller_editor)
# endif()