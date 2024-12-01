file(GLOB CPP_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB H_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

target_sources(taller_common
    PRIVATE
    ${CPP_FILES}
    PUBLIC
    ${H_FILES}
    weapons/common_weapon.h
    weapons/common_weapon.cpp
    )
