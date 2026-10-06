#include "fim/fim.hpp"
#include <map>



int main(){
  
  /* 
   * trying to read enumetrated files ;
   * display the results of the read file structure 
   * as per the file-type , using entry.is_directory()
   * and entry.is_regular_file()
   * */ 

  std::filesystem::path target = "./protected";
  constexpr std::size_t BUFFER_SIZE = 8192;
  std::array<unsigned char ,SHA256_DIGEST_LENGTH> md;
  std::string ftype;
  std::map <std::filesystem::path , std::string> hex_path_map;

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
   
    md = hash_file(entry.path());

   
    //converting md hash (binary) to hexadecimal 
    auto hex = byte_to_hex_converter(md);
  /*
    std::cout<<"###################################"<<std::endl;
    std::cout<<"[HEX] : "<<hex<<std::endl;
    std::cout<<"[PATH] : "<<entry.path()<<std::endl;
  */

    hex_path_map[entry.path()]=hex;
  }

    
 
  if (std::filesystem::exists("./protected/manifest.txt")){
  
    std::cout <<"Manifest already exists!"<<std::endl;

  }
  else{

    std::ofstream manifest("./protected/manifest.txt");
    manifest << "======<-FIM MANIFEST->======"<<std::endl;
    for (const auto [key,value]: hex_path_map){
      manifest << " [PATH] : "<< key <<" [HEX] : "<< value << '\n';
    }
    manifest << "++++++++++++++++++++++++++++"<<std::endl;

  }
  
  return 0;
}



