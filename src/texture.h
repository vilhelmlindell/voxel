#pragma once

#include <filesystem>
#include <vector>

unsigned int load_texture(const std::filesystem::path& path);
unsigned int load_texture_array(const std::vector<std::filesystem::path>& paths);
