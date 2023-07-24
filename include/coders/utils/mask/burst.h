
#ifndef CPP_PROJECT_BURST_H
#define CPP_PROJECT_BURST_H

class Burst {

public:
    bool no_data;
    int length;

    Burst(bool no_data_){
        no_data = no_data_;
        length = 1;
    }

    Burst(bool no_data_, int length_){
        assert(length_ > 0);
        no_data = no_data_;
        length = length_;
    }

    void increaseLength(){
        length++;
    }
};

#endif //CPP_PROJECT_BURST_H
