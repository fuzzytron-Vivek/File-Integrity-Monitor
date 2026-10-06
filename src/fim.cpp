#include "fim/fim.hpp"



std::array<unsigned char , SHA256_DIGEST_LENGTH> hash_file(const std::filesystem::path& path){
  //hashing function
  //takes a reference to the file path -> returns SHA256 hash of the file
  SHA256_CTX ctx;
  SHA256_Init(&ctx);

  constexpr std::size_t BUFFER_SIZE = 8192;
  std::vector <char> buffer(BUFFER_SIZE);
  std::array<unsigned char,SHA256_DIGEST_LENGTH> md;
  std::filesystem::path target = path;
  
  std::ifstream file(path);

  /* if (!file){
      return 1;
    }
    */
    //add a error handling mechanism over here


  while(file.read(buffer.data(), buffer.size())||file.gcount()>0){
    SHA256_Update(&ctx ,buffer.data(), file.gcount());
  }
  
  SHA256_Final(md.data() , &ctx);
  
  return md;
}


std::string byte_to_hex_converter(std::array<unsigned char , SHA256_DIGEST_LENGTH>& bytes ){

  //convert to hexadecimal code 
  //mental dialogue : we take the byte length , return a string result 
  //
  std::string hex_chars = "0123456789ABCDEF";

  std::string result(SHA256_DIGEST_LENGTH*2 , '\0');

  for (int i= 0 ; i<SHA256_DIGEST_LENGTH ; ++i){
  
    result[i*2]=hex_chars[bytes[i]>>4];
    result[i*2 +1] = hex_chars[bytes[i] & 0x0F];

  }

  return result;

}





