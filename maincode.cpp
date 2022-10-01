//https://stackoverflow.com/questions/66168461/finding-the-top-k-frequent-elements

#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <algorithm>
#include <unordered_map>
#include <utility>
#include <string>

auto topKFrequent(std::vector<std::string>& numstrings, size_t k) {

    // Count occurences
    std::unordered_map<std::string, size_t> counter{};
    for (const std::string& i : numstrings) counter[i]++;

    // For storing the top k
    std::vector<std::pair<std::string, size_t>> top(k);

    // Get top k
    std::partial_sort_copy(counter.begin(), counter.end(), top.begin(), top.end(),
        [](const std::pair<std::string, size_t >& p1, const std::pair<std::string, size_t>& p2) { return p1.second > p2.second; });

    return top;
}

auto StrToIntVec(std::string instring){
    //convert an input string to a vector of integers
    std:: vector<int> invec ;
    for (int i=0; i<32; i++){
        invec.push_back(instring[i] - '0') ;
    }
    return invec;
}


int main() 
{
// Input file stream object to 
std::ifstream in("original.txt");

// Declare your queue of type std::string to store each line of input
std::queue<std::string> stringque;

// Reading file.txt completely using END OF FILE eof() method
while(!in.eof()){
std::string text;
    
// extracting line from file.txt
std::getline(in, text);
stringque.push (text); 
}

//push the queue elemets to a vector<string>
std::vector<std::string> datastrings;
// data = { "a","a","b","b","c" };
while (!stringque.empty()){
    datastrings.push_back(stringque.front());
    stringque.pop();
}

//copy the strings to a vector of vector<int>, intialised into vectors of zeros
int N_strings = datastrings.size();
std::vector<std::vector<int>> datavector_2d(N_strings, std::vector<int> (32, 0));
for (int i = 0; i<N_strings; i++){
    datavector_2d[i] = StrToIntVec(datastrings[i]);
}




//testing
for (const auto& diction : topKFrequent(datastrings, 8))
    std::cout << "Value: " << diction.first << " \t Count: " << diction.second << '\n';

for (int i =0; i<32; i++){
    std::cout << datavector_2d[1][i] ;
}

return 0;
}