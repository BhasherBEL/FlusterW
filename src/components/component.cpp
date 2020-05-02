//
// Created by bhasher on 02/05/2020.
//


#include <filesystem>
#include "component.h"

std::filesystem::path const Component::tmpDir = std::filesystem::temp_directory_path() / ("t" + Rand::randString(4));
std::filesystem::path const Component::outputDir = std::filesystem::temp_directory_path() / ("o" + Rand::randString(4));