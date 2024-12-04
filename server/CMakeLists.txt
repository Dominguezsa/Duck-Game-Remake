file(GLOB CPP_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB H_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

target_sources(duck_game_server
    PRIVATE
    ${CPP_FILES}
    PUBLIC
    ${H_FILES}
    )
