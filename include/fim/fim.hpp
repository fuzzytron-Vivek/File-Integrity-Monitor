#pragma once 

#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <sstream>
#include <openssl/sha.h>
#include <array>

std::array<unsigned char , SHA256_DIGEST_LENGTH> hash_file(const std::filesystem::path &path);
std::string byte_to_hex_converter(std::array<unsigned char , SHA256_DIGEST_LENGTH>& bytes );
