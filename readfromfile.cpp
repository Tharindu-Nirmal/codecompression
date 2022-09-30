//https://www.geeksforgeeks.org/cpp-program-to-read-content-from-one-file-and-write-it-into-another-file/
// C++ program to read contents from
// one file and write it to another file
#include <iostream>
#include <string>
#include <fstream>
#include <queue>

// using namespace std;
  
// Driver code
int main()
{  
  // Input file stream object to 
  // read from file.txt
  std::ifstream in("original.txt");
    
  // Declare your queue of type std::string to
  // store each line of input
  std::queue<std::string> stringque;

  // Reading file.txt completely using 
  // END OF FILE eof() method
  while(!in.eof())
  {
    // string to extract line from 
    // file.txt
    std::string text;
        
    // extracting line from file.txt
    std::getline(in, text);
    stringque.push (text); 

    // printing the extracted lines
    std::cout << text << std::endl;
  }

  while (! stringque.empty ())
    {
      // Output front of the queue
      std::cout << stringque.front () << std::endl;
      // Pop the queue, delete item
      stringque.pop ();
    }
  return 0;
}