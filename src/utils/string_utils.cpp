
#include "string_utils.h"
#include "math_utils.h"
#include "conversor.h"
#include <iostream>


std::string StringUtils::intToStringPos(int & integer, int size){
    std::string str = Conversor::intToString(integer);
    while (str.size() < size){ str = "0" + str; }
    return str;
}

//
// SOURCE: https://stackoverflow.com/a/14516630/4547232
//
bool StringUtils::stringInList(std::string & str, std::vector<std::string> arr){
    for (int i = 0; i < arr.size(); i++) {
        if (str == arr[i]) { return true; }
    }
    return false;
}

//
// SOURCE: https://stackoverflow.com/a/2340309/4547232
//
bool StringUtils::find(std::string string, std::string string_to_find) {
    return (string.find(string_to_find) != std::string::npos);
}

//
// SOURCE: https://stackoverflow.com/a/46943631/4547232
//
std::vector<std::string> StringUtils::splitByString(std::string str, std::string delimiter){
    std::vector<std::string>result;
    int delimiter_size = delimiter.size();
    while(str.size()){
        int index = str.find(delimiter);
        if(index!=std::string::npos){
            result.push_back(str.substr(0,index));
            str = str.substr(index+delimiter_size);
            if(str.size()==0)result.push_back(str);
        }else{
            result.push_back(str);
            str = "";
        }
    }
    return result;
}

//
// SOURCE: https://stackoverflow.com/a/19751779/4547232
//
std::vector<std::string> StringUtils::splitByChar(std::string str, const char delimiter){
    std::vector<std::string> result;
    std::string acc = "";
    for(int i = 0; i < str.size(); i++){
        if(str[i] == delimiter){
            result.push_back(acc);
            acc = "";
        }
        else{
            acc += str[i];
        }
    }
    result.push_back(acc); // here we assume that str[str.size()-1] != delimiter
    return result;
}

std::string StringUtils::splitByCharWithIndex(std::string str, const char delimiter, int index){
    int delimiter_count = 0;
    std::string acc = "";
    for(int i = 0; i < str.size(); i++){
        if (index != delimiter_count){
            if(str[i] == delimiter){
                delimiter_count++;
            }
        }
        else {
            if(str[i] != delimiter){
                acc += str[i];
            }
            else {
                return acc;
            }
        }
    }
    return acc;
}

//
// SOURCE: https://stackoverflow.com/a/36280865/4547232
//
std::string StringUtils::join(std::vector<std::string> arr, std::string token){
    if (arr.empty()) return "";

    std::string str;
    for (auto i : arr)
        str += i + token;
    str = str.substr(0, str.size() - token.size());
    return str;
}

//
// SOURCE: https://stackoverflow.com/a/5891683/4547232
//
std::string StringUtils::removeChars(const std::string& source, const std::string& chars){
    std::string result="";
    for (unsigned int i=0; i<source.length(); i++) {
        bool foundany=false;
        for (unsigned int j=0; j<chars.length() && !foundany; j++) {
            foundany=(source[i]==chars[j]);
        }
        if (!foundany) {
            result+=source[i];
        }
    }
    return result;
}


std::string StringUtils::removeLastChar(const std::string& source){
    std::string result="";
    for (unsigned int i=0; i<source.length() - 1; i++) {
        result+=source[i];
    }
    return result;
}

bool StringUtils::stringCointainsSubstring(std::string string, std::string substring){
    return string.find(substring) != std::string::npos;
}
