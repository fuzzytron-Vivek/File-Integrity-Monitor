#include "fim/fim.hpp"

std::array<unsigned char, SHA256_DIGEST_LENGTH>
hash_file(const std::filesystem::path &path) {
  // hashing function
  // takes a reference to the file path -> returns SHA256 hash of the file
  SHA256_CTX ctx;
  SHA256_Init(&ctx);

  constexpr std::size_t BUFFER_SIZE = 8192;
  std::vector<char> buffer(BUFFER_SIZE);
  std::array<unsigned char, SHA256_DIGEST_LENGTH> md;
  std::filesystem::path target = path;

  std::ifstream file(path);

  /* if (!file){
      return 1;
    }
    */
  // add a error handling mechanism over here

  while (file.read(buffer.data(), buffer.size()) || file.gcount() > 0) {
    SHA256_Update(&ctx, buffer.data(), file.gcount());
  }

  SHA256_Final(md.data(), &ctx);

  return md;
}

std::string
byte_to_hex_converter(std::array<unsigned char, SHA256_DIGEST_LENGTH> &bytes) {

  // convert to raw bytes to hexadecimal
  // mental dialogue : we take the byte length , return a string result
  //
  std::string hex_chars = "0123456789ABCDEF";

  std::string result(SHA256_DIGEST_LENGTH * 2,
                     '\0'); // 64 chars , SHA_DIGEST_LENGTH is 32 bytes

  for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {

    result[i * 2] = hex_chars[bytes[i] >> 4];
    result[i * 2 + 1] = hex_chars[bytes[i] & 0x0F];
  }

  return result;
}

// the v0.2.1 will consist of generate_manifest() and check_file()
/* the generate_manifest(path , manifest ) takes the path and its -
 * associated manifest json object and writes them to the disk as a .json file;
 * the .json file acts as a manifest.
 */
// avoid copying and modifying anything that does not need processing
bool generate_manifest(const std::filesystem::path &manifest_path,
                       const nlohmann::json &manifest) {

  if (std::filesystem::exists(manifest_path)) {
    std::cout << "[FIM]: <Info> The manifest already exists....skipping "
                 "manifest generation "
              << std::endl;
    return false;
  }
  std::ofstream file(manifest_path);

  if (!file.is_open()) {
    std::cerr << "[FIM]: <Error> Unable to open the file." << std::endl;
    return false;
  }

  file << manifest.dump(4);
  file.close();

  if (!file) {
    std::cerr << "[FIM]: <Error> Manifest generation failed " << std::endl;
    return false;
  }

  std::cout << "[FIM]: <Info> Manifest generated successfully!" << std::endl;
  return true;
}

bool check_file(const std::filesystem::path &manifest_path,
                const std::filesystem::path &target_path) {

  nlohmann::json manifest_;
  if (!std::filesystem::exists(manifest_path)) {
    std::cerr << "[FIM]:<Error> Manifest does not exist on specified path! "
              << std::endl;
    return false;
  }
  std::ifstream file(manifest_path);
  if (!file) {
    std::cerr << "[FIM]: <Error> Unable to open the manifest file ! "
              << std::endl;
    return false;
  }

  file >> manifest_;

  if (!std::filesystem::exists(target_path)) {
    std::cerr << "[FIM]:<Error> Target file does not exist on specified path! "
              << std::endl;
    return false;
  }

  if (!manifest_["files"].contains(target_path.string())) {
    std::cerr << "[FIM]:<Error> Target file is not included in the "
                 "manifest!"
              << std::endl;
    return false;
  }

  auto hash = hash_file(target_path);
  auto hex = byte_to_hex_converter(hash);

  return manifest_["files"][target_path.string()] == hex;
}
