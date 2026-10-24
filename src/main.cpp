#include "fim/fim.hpp"
#include <map>

int main() {

  std::filesystem::path target = "./protected";
  constexpr std::size_t BUFFER_SIZE = 8192;
  std::array<unsigned char, SHA256_DIGEST_LENGTH> md;
  nlohmann::json manifest;
  std::filesystem::path manifest_path = "./manifest.json";
  std::cout << "FIM Started..." << std::endl;

  if (!std::filesystem::exists(manifest_path)) {
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
      std::cout << "[MAIN]: <Info> check complete with no manifest generation "
                << std::endl;
    }
  } else {
    std::ifstream file(manifest_path);
    file >> manifest;
  }

  for (const auto &entry :
       std::filesystem::recursive_directory_iterator(target)) {
    if (!entry.is_regular_file()) {
      continue;
    }
    bool check_change = check_file(manifest_path, entry.path().string());
    if (!check_change) {
      std::cout << "Changes were made in the file at path "
                << entry.path().string() << std::endl;
    } else {
      std::cout << "Filesystem is intact!" << entry.path().string()
                << std::endl;
    }
  }
  // std::cout << manifest["files"].type_name() << std::endl;
  // std::cout << manifest["files"].size() << std::endl;
  // std::cout << "Entering deletion checks" << std::endl;
  for (auto it = manifest["files"].begin(); it != manifest["files"].end();
       ++it) {
    const bool exists = std::filesystem::exists(it.key());

    std::cout << it.key() << "exists -> " << exists << std::endl;
    if (!std::filesystem::exists(it.key())) {
      std::cout << "Checking path at " << it.key() << std::endl;
      std::cout << "File with path " << it.key() << " was deleted "
                << std::endl;
    }
  }

  return 0;
}
