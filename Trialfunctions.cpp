#include <iostream>
#include <cstring>
#include <vector>
#include <string>

void display_day(int formats)
{
    switch (formats)
    {
        case 1:
            std::cout<<"MONDAY";
            break;

        case 2:
            std::cout<<"TUESDAY";
            break;
        
        case 3:
            std::cout<<"WEDNESDAY";
            break;

        case 4:
            std::cout<<"THURSDAY";
            break;

        case 5:
            std::cout<<"FRIDAY";
            break;

        case 6:
            std::cout<<"SATURDAY";
            break;

        case 7:
            std::cout<<"SUNDAY";
            break;
            
        default:
            std::cout<<"INVALID INPUT";
            break;
    }
};


auto StrToIntVec(std::string instring){
    //convert an input string to a vector of integers
    std:: vector<int> invec ;
    for (int i=0; i<16; i++){
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

int main()
{
std::vector<int> output = decToBinary(10,5);

for (int i=0; i<output.size(); i++){
    std::cout<<output[i] ;
}

return 0;
}