#ifndef MAP_PARSER_H
#define MAP_PARSER_H

#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <yaml-cpp/yaml.h>

class MapParser {
public:
    static bool parseMap(const std::string& name, const int& width, const int& height,
                         const std::vector<std::vector<uint8_t>>& tile_ids);
};

#endif  // MAP_PARSER_H
