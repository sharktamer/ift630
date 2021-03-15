#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <chrono>
#include <ctime>
#include "encode.h"


int main()
{
  std::ifstream encodedFile;
  encodedFile.open("password");

  std::string encodedWords;

  std::getline(encodedFile, encodedWords);
  encodedFile.close();
  for(int i = 0; i < encodedWords.size(); i+=7){
    std::string encodedWord = encodedWords.substr(i, 7);
    auto start = std::chrono::system_clock::now();

    std::string guess = "aaaaaaa";

    while(encode(guess) != encodedWord || guess == "zzzzzzz"){
      int i = 0;
      while(i < 7){
        guess[i] += 1;
        if(guess[i] > 'z'){
          guess[i] = 'a';
          i++;
        }
        else {
          if(i <= 2) std::cout << guess << std::endl;
          break;
        }
      }
    }





    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsedSeconds = end-start;
    std::cout << "Le mot de passe est : " << guess << std::endl;
    std::cout << "Temps pour trouver le mot de passe : " << elapsedSeconds.count() << std::endl;
  }

}
