//Bob 615-828-7489

#include "ListArray.h"
#include "ListArrayIterator.h"
#include "Text.h"
#include "Password.h"
using namespace CSC2110;

#include <iostream>
using namespace std;

 Password::Password() //constructor
 {
       len = 0;
       viable_words = new ListArray<String>();
       all_words = new ListArray<String>();
 }

 Password::~Password() //destructor (there is work to do here, delete the individual words)
 {
      delete viable_words;
      delete all_words;
}

int Password::getNumMatches(String* curr_word, String* word_guess)
{
      int match_count = 0;
      for(int i = 0; i <= curr_word->length(); i++)
      {
            if(curr_word->charAt(i)== word_guess->charAt(i))
            {
                  match_count++;
            }
      }

      return match_count;
}

void Password::addWord(String* word) //add a word to the list of possible passwords
{
      if(len == 0)
            len = word->length();
      if(len == word->length())
      {
            viable_words->add(word);
            all_words->add(word);
      }
}

 void Password::guess(int try_password, int num_matches)
{
      ListArray<String>* TmpList;
      ListArrayIterator<String>* iter = viable_words->iterator();
     while(iter->hasNext())
     {
           String* Word = iter->next();
           if(getNumMatches(Word, getOriginalWord(try_password)) == num_matches)
           {
                 TmpList->add(Word);
           }
     }
      delete viable_words;
      viable_words =TmpList;
      delete iter;
}

 int Password::getNumberOfPasswordsLeft() //returns the number of possible passwords remaining
 {
      return viable_words->size();
 }
void Password::displayViableWords() //display the current list of possible passwords
{
      for(int i = 1; i <= viable_words->size(); i++)
      {
            cout << viable_words->get(i) << endl;           //this prints out the memory address instead of the actual word
      }
}

String* Password::getOriginalWord(int index) //get a word from the original list of all passwords, 1-based
{
     return all_words->get(index);
}

int Password::bestGuess()
{
   int best_guess_index = -1;
   int best_num_eliminated = -1;
   int num_viable_passwords = getNumberOfPasswordsLeft();

   //loop over ALL words, even if they have been eliminated as the password
   int count = 1;
   ListArrayIterator<String>* all_iter = all_words->iterator();
   while(all_iter->hasNext())
   {
      String* original_word = all_iter->next();

      //loop over only those words that could still be the password
      //count up the number of matches between a possible password and a word in the original list
      int* count_num_matches = new int[len + 1];

      for (int i = 0; i < len; i++)
      {
         count_num_matches[i] = 0;
      }

      ListArrayIterator<String>* viable_iter = viable_words->iterator();
      while(viable_iter->hasNext())
      {
         String* viable_word = viable_iter->next();
         int num_matches = getNumMatches(viable_word, original_word);
         count_num_matches[num_matches]++;
      }
      delete viable_iter;

      //find the largest number in the count_num_matches array
      //the largest number indicates the guess that will generate the most eliminations
      int most_num_matches = 0;
      for (int j = 0; j < len; j++)
      {
         int curr_num_matches = count_num_matches[j];
         if (curr_num_matches > most_num_matches)
         {
            most_num_matches = curr_num_matches;
         }
      }

      //compute the fewest that can possibly be eliminated by guessing the current word (original list)
      int num_eliminated = num_viable_passwords - most_num_matches;

      //select the word to guess that maximizes the minimum number of eliminations (minimax)
      if (num_eliminated > best_num_eliminated)
      {
         best_num_eliminated = num_eliminated;
         best_guess_index = count;
      }

      count++;
      delete[] count_num_matches;
   }

   delete all_iter;
   return best_guess_index;  //return a 1-based index into the all_words list of words (careful)
}
