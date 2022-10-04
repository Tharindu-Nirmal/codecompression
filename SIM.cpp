#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <algorithm>
#include <unordered_map>
#include <utility>
#include <string>
#include <cmath>

//https://stackoverflow.com/questions/66168461/finding-the-top-k-frequent-elements
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

std::vector<int> GetCompressionInfo(std::vector<int> data_vec, std::vector<int> base_vec){
    //return the number of bits after compression other than RLE
    std::vector<int> changes_vec = CheckSim(data_vec,base_vec);
    
    //number of bits, code, ML1/ML, ML2, Mask
    std::vector<int> result = {0,0,0,0,0}; 

    if (changes_vec.size()){
        int twobutfar = 1;
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
                twobutfar =0;
                result[0] =8;
                result[1] =3;
                result[2] = changes_vec[0];
            }

            else if (bitdis<4){
                //2 apart but within one 4-bit Mask
                twobutfar = 0;
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

            else{
                //no compression
                result[0] =32;
                result[1] =6;
            }
        }
        if (numchanges==2 && twobutfar){
                // 2 bit MM far apart
                result[0] = 13;
                result[1] =4;
                result[2] =changes_vec[0];//ML1
                result[3] =changes_vec[1];//ML2
        }

        
    }

    else{
        //no changes, exactly matches base
        result[0] = 3;
        result[1] = 5;
    }
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


int main(int argc, char *argv[]){
    if ((std::string)argv[1]=="1"){
        //compression
        // Input file stream object
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
// int N_strings = 80;

std::vector<std::vector<int>> datavector_2d(N_strings, std::vector<int> (32, 0));
for (int i = 0; i<N_strings; i++){
    datavector_2d[i] = StrToIntVec(datastrings[i]);
}

//copy diction strings to a vector of vector<int>, intialised into vectors of zeros
std::vector<std::vector<int>> basisvector_2d(8, std::vector<int> (32, 0));
std::vector<std::pair<std::string, size_t>> basis_dic = topKFrequent(datastrings, 8);

for (int i = 0; i<8; i++){
    // std::cout << basis_dic[i].first <<'\n'; 
    basisvector_2d[i] = StrToIntVec(basis_dic[i].first);
}

//container for output bits
std:: vector<int> outbits;

int next_index = 0;
while (next_index<N_strings){
    int repitition = 0;
    while(datavector_2d[next_index+repitition]==datavector_2d[next_index+repitition+1]){repitition++;}

    int selectedbase = 0;
    int bitcost = 40;
    std::vector <int> selectedinfo ;
    std::vector <int> codedinfo ;


    for (int i=0;i<8;i++){
        std::vector <int> info;
        info = GetCompressionInfo(datavector_2d[next_index],basisvector_2d[i]);

        if (info[0]<bitcost){selectedbase=i; selectedinfo=info;bitcost=info[0];}
    }
    // info is selected. Code to bits

    std::vector <int> compresscode;
    compresscode = decToBinary(selectedinfo[1],3);

    codedinfo.insert(codedinfo.end(),compresscode.begin(),compresscode.end());

    std::vector <int> ml1 = decToBinary(selectedinfo[2],5);
    std::vector <int> ml2 = decToBinary(selectedinfo[3],5);
    std::vector <int> bm = decToBinary(selectedinfo[4],4);
    std::vector <int> dicindex = decToBinary(selectedbase,3);

    switch(selectedinfo[1]){
        //based on the compression format
        case 1:
            codedinfo.insert(codedinfo.end(),ml1.begin(),ml1.end());
            codedinfo.insert(codedinfo.end(),bm.begin(),bm.end());
            codedinfo.insert(codedinfo.end(),dicindex.begin(),dicindex.end());
            break;
        case 2:
            codedinfo.insert(codedinfo.end(),ml1.begin(),ml1.end());
            codedinfo.insert(codedinfo.end(),dicindex.begin(),dicindex.end());
            break;
        case 3:
            codedinfo.insert(codedinfo.end(),ml1.begin(),ml1.end());
            codedinfo.insert(codedinfo.end(),dicindex.begin(),dicindex.end());
            break;
        case 4:
            codedinfo.insert(codedinfo.end(),ml1.begin(),ml1.end());
            codedinfo.insert(codedinfo.end(),ml2.begin(),ml2.end());
            codedinfo.insert(codedinfo.end(),dicindex.begin(),dicindex.end());
            break;
        case 5:
            codedinfo.insert(codedinfo.end(),dicindex.begin(),dicindex.end());
            break;
        case 6:
            codedinfo.insert(codedinfo.end(),datavector_2d[next_index].begin(),datavector_2d[next_index].end());
            break;
    }

    if (repitition >0){codedinfo.push_back(0);codedinfo.push_back(0);codedinfo.push_back(0);std::vector <int> codedrepition = decToBinary(repitition-1,2);
    codedinfo.insert(codedinfo.end(),codedrepition.begin(),codedrepition.end());
    }

    next_index = next_index+repitition+1;

    // cmd debug
    // for (int i =0; i<codedinfo.size(); i++){std::cout << codedinfo[i];}
    // std::cout << '\n';

    //store in output vector
    outbits.insert(outbits.end(),codedinfo.begin(),codedinfo.end());

}

//buffer 1s
int lastlinexnum = 32- outbits.size()%32;
for (int i=0;i<lastlinexnum;i++){outbits.push_back(1);}

if( !(outbits.size()%32 ==0)){std::cout<<"error last line 1s"<<'\n';}

//write output
std::ofstream compressedout("cout.txt");

int lines_num = outbits.size()/32;
for (int i=0;i<lines_num;i++){
    for(int j=0; j<32; j++){compressedout<< outbits[i*32 +j];}
    compressedout<<'\n';
}

compressedout<< "xxxx"<<'\n';

for (int i=0;i<8;i++){
    for(int j=0; j<32; j++){compressedout<< basisvector_2d[i][j];}
    if(i!=7){compressedout<<'\n';}
}

compressedout.close();

return 0;
 }


    else if ((std::string)argv[1]=="2"){
        //decompression
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
    // tests
    // std::cout<< binaryToDec(basisvector_2d[4],1,3);

    decompfile.close();
    return 0;
        
    }

}