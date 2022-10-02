#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <algorithm>
#include <unordered_map>
#include <utility>
#include <string>
#include <cmath>

auto StrToIntVec(std::string instring){
    //convert an input 32 bit string to a vector of integers
    std:: vector<int> invec ;
    for (int i=0; i<32; i++){
        invec.push_back(instring[i] - '0') ;
    }
    return invec;
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

int binaryToDec(std::vector<int> data, int start, int width){
    int result = 0;
    for(int i=0;i<width;i++){ result = result + data[start+i]*pow(2, width-i-1);}
    return result;
}

int main(){
    std::ifstream in("compressed.txt");
    std::queue<std::string> stringque;
    while(!in.eof()){std::string text;std::getline(in, text);stringque.push (text);} 

    int numlines = stringque.size()-8-1;
    std::vector<int> compressedbits ;
    std::vector<std::vector<int>> basisvector_2d(8, std::vector<int> (32, 0));

    //apppend only the datavectors into compressedbits vector
    for(int i=0;i<numlines;i++){
        std::vector <int> linedata = StrToIntVec(stringque.front());
        stringque.pop();
        compressedbits.insert(compressedbits.end(),linedata.begin(),linedata.end());
    }
    //pop the xxxx line
    stringque.pop();

    //basisvector2D vector
    for(int i=0;i<8;i++){
        basisvector_2d[i] = StrToIntVec(stringque.front());
        stringque.pop();
    }

    // tests
    // std::cout<< binaryToDec(basisvector_2d[4],1,3);

    return 0;
}