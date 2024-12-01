file(GLOB CPP_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB H_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
file(GLOB WEAPON_FILES "${CMAKE_CURRENT_SOURCE_DIR}/weapons/*.cpp")
file(GLOB WEAPON_H_FILES "${CMAKE_CURRENT_SOURCE_DIR}/weapons/*.h")

target_sources(taller_common
    PRIVATE
    ${CPP_FILES}
    PUBLIC
    ${H_FILES}
    ${WEAPON_FILES}
    ${WEAPON_H_FILES}
    )
