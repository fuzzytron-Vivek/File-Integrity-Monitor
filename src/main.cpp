#include "fim/fim.hpp"
#include <map>

int main(int argc, char *argv[]) {

  std::filesystem::path target;
  constexpr std::size_t BUFFER_SIZE = 8192;
  std::array<unsigned char, SHA256_DIGEST_LENGTH> md;
  nlohmann::json manifest;
  std::filesystem::path manifest_path = "./manifest.json";

  std::cout << "<-*------------*-< File Integrity Manager >-*-----------*->"
            << std::endl;
  if (argc == 1) {
    std::cout << "       Use 'fim --help' to view available commands.  "
              << std::endl;
  }
  if (argc > 1 && std::string(argv[1]) == "--help") {
    std::cout << R"(
    Usage:
      fim [OPTIONS]

    Options:
      --help       Display this help menu
      --init PATH  Generate a manifest
      --check PATH Verify file integrity
    )";
    return 0;
  }

  if (argc >= 3 && std::string(argv[1]) == "init") {
    target = std::filesystem::path(argv[2]);
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
        std::cout
            << "[MAIN]: <Info> check complete with no manifest generation "
            << std::endl;
      }
    } else {
      std::cout << "Manifest exists at ->" << manifest_path << std::endl;
      std::ifstream file(manifest_path);
      file >> manifest;
    }
  }
  if (argc >= 3 && std::string(argv[argc - 2]) == "check") {
    target = std::filesystem::path(argv[argc - 1]);
    std::ifstream file(manifest_path);
    file >> manifest;

    for (const auto &entry :
         std::filesystem::recursive_directory_iterator(target)) {
      if (!entry.is_regular_file()) {
        continue;
      }
      if (!(manifest["files"].contains(entry.path()))) {
        std::cout << "[MAIN] ||NEW|| -> " << entry.path().string() << std::endl;
      } else {
        bool check_change = check_file(manifest_path, entry.path().string());
        if (!check_change) {
          std::cout << "[MAIN] ||MODIFIED|| -> " << entry.path().string()
                    << std::endl;
        } else {
          std::cout << "[MAIN] ||INTACT|| ->" << entry.path().string()
                    << std::endl;
        }
      }
    }
    // std::cout << manifest["files"].type_name() << std::endl;
    // std::cout << manifest["files"].size() << std::endl;
    // std::cout << "Entering deletion checks" << std::endl;
    for (auto it = manifest["files"].begin(); it != manifest["files"].end();
         ++it) {
      const bool exists = std::filesystem::exists(it.key());
      if (!std::filesystem::exists(it.key())) {
        std::cout << "[MAIN] ||DELETED|| -> " << it.key() << std::endl;
      }
    }

    return 0;
  }
}
