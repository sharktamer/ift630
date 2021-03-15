#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <chrono>
#include <ctime>
#include <thread>
#include "encode.h"
#define NB_THREADS 2

class Decoder{

  private:
  std::string m_encodedWord;
  std::string m_decodedWord;

  void guessWords(std::string guess, std::string fin){
    do{
      if(encode(guess) == m_decodedWord){
        m_encodedWord = guess;
        break;
      }
      int i = 0;
      while(i < 7){
        guess[i] += 1;
        if(guess[i] > 'z'){
          guess[i] = 'a';
          i++;
        }
        else break;
      }
    }
    while(m_encodedWord.size() == 0 || guess == fin)
  }

  std::string getIteration(long long int iteration){
    std::string word = "";
    for (int i = 0; i < 7; i++){
      int letter = iteration % 26;
      word += 'a' + letter;
      iteration = (iteration - letter) / 26;
    }
    return word;
  }

  public:
  Decoder(std::string encodedWord){
    m_encodedWord = encodedWord;
  }

  std::string decode(){
    //separe les tache dans les threads et part les threads

    return m_decodedWord;
  }


}

int main()
{
  std::ifstream encodedFile;
  encodedFile.open("password");

  std::string encodedWords;
  std::string decodedWord;

  std::getline(encodedFile, encodedWords);
  encodedFile.close();




  for(int i = 0; i < encodedWords.size(); i+=7){
    std::string encodedWord = encodedWords.substr(i, 7);
    auto start = std::chrono::system_clock::now();

    std::string guess = "aaaaaaa";

    while(encode(guess) != encodedWord || guess == "zzzzzzz"){
      int i = 6;
      while(i >= 0){
        guess[i] += 1;
        if(guess[i] > 'z'){
          guess[i] = 'a';
          i--;
        }
        else break;
      }
    }






    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsedSeconds = end-start;
    std::cout << "Le mot de passe est : " << guess << std::endl;
    std::cout << "Temps pour trouver le mot de passe : " << elapsedSeconds.count() << std::endl;
  }

}
