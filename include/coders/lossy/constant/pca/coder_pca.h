
#ifndef CPP_PROJECT_CODER_PCA_H
#define CPP_PROJECT_CODER_PCA_H

#include "coder_cols.h"
#include "pca_window.h"

class CoderPCA: public CoderCols {

private:
    std::vector<int> epsilons_vector;
    PCAWindow* window;
    
    void codeColumnBefore() override;
    void codeColumnWhile(std::string csv_value) override;
    void codeColumnAfter() override;

    PCAWindow* createWindow();
    void codeWindow(PCAWindow* window);
    void codeConstantWindow(PCAWindow* window);
    void codeNonConstantWindow(PCAWindow* window);

public:
    using CoderCols::CoderCols;
    void setCoderParams(int window_size_, std::vector<int> epsilons_vector_);
    static std::vector<int> codeTimeDelta(CoderPCA* coder, int window_size);
};

#endif //CPP_PROJECT_CODER_PCA_H
