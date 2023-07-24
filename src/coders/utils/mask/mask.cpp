
#include "mask.h"
#include "constants.h"

Mask::Mask(){
    total_no_data = 0;
    total_data = 0;
    bursts.clear();
    current_burst = NULL;
}

void Mask::setBurst(int index){
    assert(index < bursts.size());
    Burst* burst = bursts.at(index);
    burst_is_no_data = burst->no_data;
    burst_length = burst->length;
}

void Mask::add(Burst* burst){
    bursts.push_back(burst);
    if (burst->no_data) { total_no_data += burst->length; } else { total_data += burst->length; }
}

void Mask::add(bool no_data, int length){
    add(new Burst(no_data, length));
}

void Mask::add(bool no_data){
    if (current_burst == NULL){
        current_burst = new Burst(no_data);
        return;
    }
    bool same_burst = current_burst->no_data == no_data;
    if (same_burst) {
        current_burst->increaseLength();
    }
    else {
        add(current_burst);
        current_burst = new Burst(no_data);
    }
}

void Mask::close(){
#if CHECKS
    assert(current_burst != NULL);
#endif
    add(current_burst);
}

void Mask::print(){
    std::cout << "Mask::print()" << std::endl;
    std::cout << "total_data = " << total_data << std::endl;
    std::cout << "total_no_data = " << total_no_data << std::endl;
    std::cout << "total rows = " << total_data + total_no_data << std::endl;
    int current_index = 0;
    for(int i=0; i < bursts.size(); i++){
        Burst* burst = bursts.at(i);
        std::string str = burst->no_data ? "N" : "1";
        int new_current_index = current_index + burst->length;
        std::cout << "[" << current_index << ", " << new_current_index - 1 << "] => " << str << std::endl;
        current_index = new_current_index;
    }
}

void Mask::reset(){
    current_index = 0;
    setBurst(0);
}

bool Mask::isNoData(){
    if (burst_length == 0){
        current_index++;
        setBurst(current_index);
    }
    burst_length--;
    return burst_is_no_data;
}
