#include "MapParser.h"

#include <filesystem>

bool MapParser::parseMap(const std::string& name, const int& width, const int& height,
                         const std::vector<std::vector<uint8_t>>& tile_ids) {
    try {
        // Verifica que el directorio exista, si no, lo crea
        const std::string output_dir = "/var/duck_game/maps/";
        std::filesystem::create_directories(output_dir);

        // Crea el emisor YAML
        YAML::Emitter emitter;
        emitter.SetIndent(2);

        // Starts the YAML document
        emitter << YAML::BeginMap;
        emitter << YAML::Key << "name" << YAML::Value << name;
        emitter << YAML::Key << "width" << YAML::Value << width;
        emitter << YAML::Key << "height" << YAML::Value << height;

        // Starts the tiles sequence
        emitter << YAML::Key << "tiles" << YAML::Value << YAML::BeginSeq;
        for (const auto& row: tile_ids) {
            emitter << YAML::Flow << YAML::BeginSeq;
            for (const auto& tile: row) {
                emitter << static_cast<int>(tile);  // Cast to int to avoid ASCII conversion
            }
            emitter << YAML::EndSeq;
        }
        emitter << YAML::EndSeq;

        emitter << YAML::EndMap;

        // Saves the YAML document to a file
        std::ofstream fout(name + ".yaml");
        if (!fout.is_open()) {
            throw std::ios_base::failure("No se pudo abrir el archivo para escribir.");
        }
        fout << emitter.c_str();  // Escribe el contenido generado
        fout.close();
    } catch (const std::ios_base::failure& e) {
        std::cerr << "Error de archivo: " << e.what() << std::endl;
        return false;
    } catch (const YAML::Exception& e) {
        std::cerr << "Error de YAML: " << e.what() << std::endl;
        return false;
    } catch (const std::exception& e) {
        std::cerr << "Error inesperado: " << e.what() << std::endl;
        return false;
    }
    return true;
}
