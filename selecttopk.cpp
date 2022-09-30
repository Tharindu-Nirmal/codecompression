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

// Test code
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
}

//push the queue elemets to a vector
std::vector<std::string> data;
// data = { "a","a","b","b","c" };
while (!stringque.empty())
{
    data.push_back(stringque.front());
    stringque.pop();
}


for (const auto& diction : topKFrequent(data, 8))
    std::cout << "Value: " << diction.first << " \t Count: " << diction.second << '\n';
return 0;
}