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
    //convert binary repr in a vector to decimal
    int result = 0;
    for(int i=0;i<width;i++){result = result + data[start+i]*pow(2, width-i-1);}
    return result;
}

std::vector<int> applyMask(std::vector<int> data, std::vector<int> basevec, int start, int ml1){
    // return 32bit vector int of the 4bit bitmask applied to basis vector
    std::vector<int> outdata;
    int checkbit;
    for (int i=0;i<32;i++){
        checkbit = basevec[i];
        if ((ml1<=i && i< (ml1+4)) && (data[start+3+5+(i-ml1)]==1)){checkbit = (checkbit+1)%2 ;}
        outdata.push_back(checkbit);
    }
    return outdata;
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

    //containers for result
    std::vector<std::vector<int>> decompout;

    //outputfile
    std::ofstream decompfile("dout.txt");

    //loop through the digits
    int current_index = 0; //of the data vector
    int current_line =0;
    std::vector <int> justdecoded;

    while (current_index< (numlines*32)){
        // std::cout<<"current_index:"<<current_index<<'\n';
        int compressioncode = binaryToDec(compressedbits,current_index,3);
        int repitition;
        int firstmm;
        int secondmm;
        int baseindex;
        int lenbuf;
        std::vector<int> bitmasked;

        switch(compressioncode){
            case 0:
                repitition = 1+binaryToDec(compressedbits,current_index+3,2);
                //deal with repitition.......
                    for (int i=0;i<repitition;i++){
                        // decompfile<<"rep:"<<repitition<<'_';
                        lenbuf = justdecoded.size();
                        for(int i=lenbuf-32;i<lenbuf;i++){decompfile<<justdecoded[i];}
                        decompfile<<'\n';
                    }
                current_index = current_index + 5;
                break;

            case 1:
                //masking
                firstmm = binaryToDec(compressedbits,current_index+3,5);
                baseindex = binaryToDec(compressedbits,current_index+12,3);
                bitmasked = applyMask(compressedbits,basisvector_2d[baseindex],current_index,firstmm);
                //apply bitmask.......
                // decompfile<<"bitmasked:";
                for (int i=0;i<32;i++){
                    decompfile<<bitmasked[i];
                    justdecoded.push_back(bitmasked[i]);
                }
                
                current_index = current_index + 15;
                decompfile<<'\n';
                break;

            case 2:
                //1 bit MM 
                firstmm = binaryToDec(compressedbits,current_index+3,5);
                baseindex = binaryToDec(compressedbits,current_index+8,3);
                for(int i=0;i<32;i++){
                    if (i==firstmm){decompfile<< (basisvector_2d[baseindex][i] + 1)%2;justdecoded.push_back((basisvector_2d[baseindex][i] + 1)%2);} //flip bit
                    else{decompfile<<basisvector_2d[baseindex][i];justdecoded.push_back(basisvector_2d[baseindex][i]);}
                }
                current_index = current_index + 11;
                decompfile<<'\n';
                break;

            case 3:
                //2 bit MM consec
                firstmm = binaryToDec(compressedbits,current_index+3,5);
                baseindex = binaryToDec(compressedbits,current_index+8,3);
                for(int i=0;i<32;i++){
                    if ((i==firstmm) || (i==firstmm+1)){decompfile<< (basisvector_2d[baseindex][i] + 1)%2;justdecoded.push_back((basisvector_2d[baseindex][i] + 1)%2);} //flip bit
                    else{decompfile<<basisvector_2d[baseindex][i];justdecoded.push_back(basisvector_2d[baseindex][i]);}
                }
                current_index = current_index + 11;
                decompfile<<'\n';
                break;

            case 4:
                //2 bit MM far
                firstmm = binaryToDec(compressedbits,current_index+3,5);
                secondmm = binaryToDec(compressedbits,current_index+8,5);
                baseindex = binaryToDec(compressedbits,current_index+13,3);
                for(int i=0;i<32;i++){
                    if ((i==firstmm) || (i==secondmm)){decompfile<< (basisvector_2d[baseindex][i] + 1)%2;
                    justdecoded.push_back((basisvector_2d[baseindex][i] + 1)%2);}
                     //flip bit
                    else{decompfile<<basisvector_2d[baseindex][i];justdecoded.push_back(basisvector_2d[baseindex][i]);}
                }
                current_index = current_index + 16;
                decompfile<<'\n';
                break;

            case 5:
                //Directmatching
                baseindex = binaryToDec(compressedbits,current_index+3,3);
                for(int i=0;i<32;i++){decompfile<<basisvector_2d[baseindex][i]; justdecoded.push_back(basisvector_2d[baseindex][i]);}
                current_index = current_index + 6;
                decompfile<<'\n';
                break;

            case 6:
                //Original Binary
                for(int i=0;i<32;i++){decompfile<<compressedbits[current_index+3+i];justdecoded.push_back(compressedbits[current_index+3+i]);}
                current_index = current_index + 35;
                decompfile<<'\n';
                break;

            case 7:
                //a set of ones..end
                current_index = current_index+ 40;
                // decompfile<<'\n';
                break;
        }

        
    }
    decompfile.close();

    

    // Input file stream object
    std::ifstream inp("dout.txt");
    std::queue<std::string> editstringque;
    while(!inp.eof()){
    std::string text;
    std::getline(inp, text);
    editstringque.push (text);
    std::cout << text<<'\n';
    }

    int s = editstringque.size();
    std::cout << s;

    std::ofstream outp("dout.txt");
    for (int i=0;i<s-1;i++){
        if (i!=(s-2)){outp<<editstringque.front()<<'\n';}
        else{outp<<editstringque.front();}
        editstringque.pop();
    }
    return 0;
    
}