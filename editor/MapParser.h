#ifndef MAP_PARSER_H
#define MAP_PARSER_H

#include <string>
#include <vector>
#include <cstdint>
#include <yaml-cpp/yaml.h>
#include <fstream>
#include <iostream>

class MapParser {
    public:
        static bool parseMap(const std::string& name, const int& width, const int& height,
                             const std::vector<std::vector<uint8_t>>& tile_ids);
};

#endif // MAP_PARSER_H
