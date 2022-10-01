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

auto CheckSim(std::vector<int> data_vec, std::vector<int> base_vec){
    //return a vector with the integer indices of dissimilarity
    std::vector<int> result ;
    for (int i=0; i<32; i++){
        if (data_vec[i]-base_vec[i]){result.push_back(i);}
    }
    return result;
}

auto decToBinary(int n, int bitsize){
    std::vector<int> result ;
    // array to store binary number
    int binaryNum[32];
 
    // counter for binary array
    int i = 0;
    while (n > 0) {
 
        // storing remainder in binary array
        binaryNum[i] = n % 2;
        n = n / 2;
        i++;
    }
 
    // printing binary array in reverse order
    for (int j = i - 1; j >= 0; j--){result.push_back(binaryNum[j]) ;}
    while (result.size()<bitsize){result.insert(result.begin(),0);}
    return result;
}

std::vector<int> GetCompressionBits(std::vector<int> data_vec, std::vector<int> base_vec){
    //return the number of bits after compression other than RLE
    std::vector<int> changes_vec = CheckSim(data_vec,base_vec);
    //number of bits, code, ML1/ML, ML2, Mask
    std::vector<int> result = {0,0,0,0,0}; 

    if (changes_vec.size()){
        int numchanges = changes_vec.size();
        if (numchanges == 1){
            // 1 bit Mismatch
            result[0] = 8;
            result[1] = 2;
            result[2] = changes_vec[0];
        }
        else if(numchanges==2){
            int bitdis = changes_vec[1]-changes_vec[0];
            if (bitdis ==1){
                //2 consec bit MM
                result[0] =8;
                result[1] =3;
                result[2] = changes_vec[0];
            }

            else if (bitdis<4){
                //2 apart but within one 4-bit Mask
                result[0] =12;
                result[1] =1;
                result[2] =changes_vec[0];
                if(bitdis==2){
                    //1010
                    result[4] = 10;
                }
                else if(bitdis==3){
                    //1001
                    result[4] = 9;
                }
            }

        }

        else if (numchanges>2){
            int bitdis = changes_vec[numchanges-1]-changes_vec[0];
            if (bitdis<4){
                //many apart but within one 4-bit Mask
                result[0] =12;
                result[1] =1;
                result[2] =changes_vec[0];//mask location
                if (numchanges==4){result[4]=15;}//1111
                else if (numchanges==3){
                    if(changes_vec[1]-changes_vec[0] == 2){result[4]=11;}//1011
                    else if(changes_vec[2]-changes_vec[1] == 2){result[4]=13;}//1101
                    else{result[4]=14;}//1110
                }                
            }

        else if (numchanges==2){
                // 2 bit MM far apart
                result[0] = 13;
                result[1] =4;
                result[2] =changes_vec[0];//ML1
                result[3] =changes_vec[1];//ML2
            }

        else{
                //no compression
                result[0] =32;
                result[1] =6;
            }
        }


    }

    else{
        //no changes, exactly matches base
        result[0] = 3;
        result[1] = 5;
    }
}


int main() 
{
// Input file stream object to 
std::ifstream in("original.txt");

// Declare your queue of type std::string to store each line of input
std::queue<std::string> stringque;

while(!in.eof()){
std::string text;
std::getline(in, text);
stringque.push (text); 
}

//push the queue elemets to a vector<string>
std::vector<std::string> datastrings;
while (!stringque.empty()){
    datastrings.push_back(stringque.front());
    stringque.pop();
}

//copy data strings to a vector of vector<int>, intialised into vectors of zeros
int N_strings = datastrings.size();
std::vector<std::vector<int>> datavector_2d(N_strings, std::vector<int> (32, 0));
for (int i = 0; i<N_strings; i++){
    datavector_2d[i] = StrToIntVec(datastrings[i]);
}

//copy diction strings to a vector of vector<int>, intialised into vectors of zeros
std::vector<std::vector<int>> basisvector_2d(8, std::vector<int> (32, 0));
std::vector<std::pair<std::string, size_t>> basis_dic = topKFrequent(datastrings, 8);

for (int i = 0; i<8; i++){
    std::cout << basis_dic[i].first <<'\n'; 
    basisvector_2d[i] = StrToIntVec(basis_dic[i].first);
}



//testing
// for (const auto& diction : topKFrequent(datastrings, 8))
//     std::cout << "Value: " << diction.first << " \t Count: " << diction.second << '\n';

// for (int i =0; i<32; i++){
//     std::cout << basisvector_2d[6][i] ;
// }
if(-1){std::cout<<11;}
return 0;
}