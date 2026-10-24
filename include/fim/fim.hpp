#pragma once

#include <array>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <nlohmann/json.hpp>
#include <openssl/sha.h>
#include <sstream>
#include <string>
#include <vector>

std::array<unsigned char, SHA256_DIGEST_LENGTH>
hash_file(const std::filesystem::path &path);

std::string
byte_to_hex_converter(std::array<unsigned char, SHA256_DIGEST_LENGTH> &bytes);

bool generate_manifest(const std::filesystem::path &manifest_path,
                       const nlohmann::json &manifest);

bool check_file(const std::filesystem::path &manifest_path,
                const std::filesystem::path &path);
