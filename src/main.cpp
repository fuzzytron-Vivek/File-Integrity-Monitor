#include "fim/fim.hpp"
#include <map>

int main() {

  std::filesystem::path target = "./protected";
  constexpr std::size_t BUFFER_SIZE = 8192;
  std::array<unsigned char, SHA256_DIGEST_LENGTH> md;
  nlohmann::json manifest;
  std::filesystem::path manifest_path = "./manifest.json";
  std::cout << "FIM Started..." << std::endl;

  for (const auto &entry :
       std::filesystem::recursive_directory_iterator(target)) {

    if (!entry.is_regular_file()) {
      continue;
    }

    md = hash_file(entry.path());
    auto hex = byte_to_hex_converter(md);

    manifest["files"][entry.path().string()] = hex;
  }

  bool check = generate_manifest(manifest_path, manifest);
  if (!check) {
    std::cout << "Could not generate manifest!" << std::endl;
  }

  return 0;
}
