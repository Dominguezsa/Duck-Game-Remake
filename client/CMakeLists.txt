file(GLOB CPP_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB H_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

target_sources(duck_game
    PRIVATE
    ${CPP_FILES}
    PUBLIC
    ${H_FILES}
    lobbywindow.ui
    )

# Incluir y vincular Qt Widgets
target_include_directories(duck_game PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(duck_game PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Instalación
include(GNUInstallDirs)
install(TARGETS duck_game
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
    
# Finalizar el ejecutable para Qt 6
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(duck_game)
endif()