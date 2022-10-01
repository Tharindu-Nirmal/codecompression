#include <iostream>
#include <cstring>
#include <vector>

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



int main()
{
// std::string input_string = "0100010011110100";
// auto d = StrToIntVec(input_string);
// for(int i=0; i < d.size(); i++)
//    std::cout << d.at(i) << ' ';


return 0;
}