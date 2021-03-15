#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

using namespace std;

/************************************************
  Cette fonction effectue une substitution de
  caracteres en additionnant une valeur
************************************************/
string ADD(string mot, int cle)
{
   int taille;

   taille = mot.length();  
   for (int i=0;  i< taille; i++)
       mot[i] = mot[i]+cle*i;
   return mot;
}
/************************************************
  Cette fonction effectue un ou exclusif
  entre la chaine de caracteres et la cle  
************************************************/
string XOR(string mot, string cle)
{
   int taille;
   taille = mot.length();
   
   for (int i=0;  i< taille; i++)
       mot[i] = mot[i]^cle[i];
   return mot;
}
/*************************************************
  Cette fonction effectue un decalage circulaire 
  vers la droite de "nb" caracteres
**************************************************/
string decale(string mot, int nb)
{
  char t[nb];
  int max = mot.length()-1;

     cout << "----" << max << "..." << nb << endl;
  for(int i=0; i<nb; i++)
      t[i] = mot[max-i];
  for (int i = max; i>=nb; i--)
      mot[i] = mot[i-nb];
  for (int i=0; i<nb; i++)
      mot[i] = t[nb-i-1];
  return mot;
}
/*************************************************
  Cette fonction effectue une translation  
  entre les caracteres du mot d'une distance 
  deduite de la cle
**************************************************/
string echange(string mot, int cle)
{
  int taille = mot.length();

  for(int i=0; i+cle < taille; i++)
  { 
    char temp;
    temp = mot[i];
    mot[i] = mot[i+cle];
    mot[i+cle] = temp;
  }
  return mot;
}
/*************************************************
  Cette fonction genere une cle numerique
**************************************************/
int getCle(string mot)
{
   int cle,temp;
   temp =0;
   for (int i=0;i<mot.length() ; i++) 
       temp = temp + (unsigned int) mot[i];
   cle = (temp % 4)+1;
   cout << "La clé est .... " << cle << endl;
   return abs(cle);
}
/*************************************************
  Cette fonction encode le message
**************************************************/
string encode(string mot)
{
   string copie;
   int cle;

   copie = mot;
   for (int j=0; j<3; j++)
   {
     cle = getCle(mot);
     mot = decale(mot,cle/2);
     mot = ADD(mot,cle);
     mot = echange(mot,cle);
     mot = XOR(mot,copie); 
   }
   return mot;
}

/******************************************************
  Ce programme encode une série de mots et les placent 
  dans le fichier "password"
******************************************************/
int main()
{
   string password, copie, p1,p2,p3,p4;
   ofstream res("password", ios::out|ios::binary);
   cout << "Entrez la chaine de caractere de longueur 7"<< endl;
   cin >> password;

   while(password != "fin")
   {
      copie = password;
      password = encode(password);
      cout << endl<< " le mot encode est : " << password << endl;
      for (int i=0; i < password.length(); i++)
      {
        res.put((unsigned char)password[i]);
        cout <<  (int)password[i] << " ";
      }
      cout << "Entrez la prochaine chaine de caractere de longueur 7"<< endl;
      cin >> password;
    }
 
}
