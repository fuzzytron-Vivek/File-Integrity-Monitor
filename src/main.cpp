#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <sstream>
#include <openssl/sha.h>



std::string byte_to_hex_lookup(const unsigned char* bytes){
  static const char hex_chars[]= "0123456789ABCDEF";


  std::string result(SHA256_DIGEST_LENGTH * 2,'\0');// think of this as  string result(size , what to fill it with)
  for(auto i = 0 ; i < SHA256_DIGEST_LENGTH;++i){

    result[i*2] = hex_chars[bytes[i] >> 4];     //gives the upper nibble of 'i'th byte 
    result[i*2+1] = hex_chars[bytes[i] & 0X0F];     //gives the lower nibble of the 'i'th byte

  }
  return result;
}
int main(){
  
  /* 
   * trying to read enumetrated files ;
   * display the results of the read file structure 
   * as per the file-type , using entry.is_directory()
   * and entry.is_regular_file()
   * */ 

  std::filesystem::path target = "./protected";
  constexpr std::size_t BUFFER_SIZE = 8192;
  unsigned char md[SHA256_DIGEST_LENGTH];
  std::string ftype;

  std::cout <<"FIM Started..."<<std::endl;

  /*
    *   1.  the directory_iterator omits displaying c.txt 
    *       as it does not traverse the entire hierarchy 
    *
    *  2. the recursive_directory_iterator traverses 
    *     the entire hierarchy and hence displays 
    *    'c.txt' as a regular file.
    */

 
  for (const auto& entry : std::filesystem::recursive_directory_iterator(target)){

    //std::cout<<"Path: "<<entry.path()<<std::endl;
    
    if (entry.is_regular_file()){
      ftype = " -> [FILE]\n";
    }

    if (entry.is_directory()){
      ftype = " ->[DIRECTORY]\n";
    }


    std::ifstream file (entry.path());
    if (!file ){
      continue ;
    }
    //dont declare and init in a global scope , each file needs its own hash!
    SHA256_CTX ctx;
    SHA256_Init(&ctx);
    
    //extracting contents line by line 
    /*if (file.is_open()){
      std::string line;
      while (std::getline(file , line )){
        std::cout<<line<<std::endl;
      }
    }
    */ 

     
    std::vector<char> buffer(BUFFER_SIZE);

    while (file.read(buffer.data() ,buffer.size())|| file.gcount() > 0)
    {
      SHA256_Update(&ctx , buffer.data() , file.gcount());
    }

    SHA256_Final(md , &ctx);

    //converting md hash (binary) to hexadecimal 
    auto hex = byte_to_hex_lookup(md);
    std::cout<<"###################################"<<std::endl;
    std::cout<<"[HEX] : "<<hex<<std::endl;
    std::cout<<"[FTYPE] : "<<ftype;
    std::cout<<"[PATH] : "<<entry.path()<<std::endl;

  }
  
  return 0;
}
