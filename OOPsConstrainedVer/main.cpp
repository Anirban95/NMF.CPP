// Developer: Dr. Anirban Dey, anirban.dey@yahoo.in
// Date: 15/07/2025


#include "nmf.h"
#include <iostream>


#include "nmf.h"
#include <iostream>

int main() {
    Matrix V = {
        {1,2,3,4,5},
        {5,4,3,2,1},
        {1,3,5,3,1},
        {5,3,1,3,5}
    };

    size_t K = 2;
    size_t max_iter = 500;
    double Lambda = 0.01;

    NMF nmf(V, K, max_iter, 1e-9, Lambda);
    nmf.factorize();

    std::cout << nmf << std::endl;

    std::cout << "Reconstruction error (Frobenius norm): " << nmf.reconstruction_error() << "\n";
    std::cout << "Relative reconstruction error: " << nmf.relative_reconstruction_error() << "\n";

    return 0;
}

