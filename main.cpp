// Developer: Dr. Anirban Dey, anirban.dey@yahoo.in
// Date: 15/07/2025


#include "nmf.h"
#include <iostream>

int main() {
    // Example input matrix V (4x5)
    Matrix V = {
        {1.0, 2.0, 3.0, 4.0, 5.0},
        {5.0, 4.0, 3.0, 2.0, 1.0},
        {1.0, 3.0, 5.0, 3.0, 1.0},
        {5.0, 3.0, 1.0, 3.0, 5.0}
    };

    size_t K = 2;          // Number of components
    size_t max_iter = 500; // Number of iterations

    Matrix W, H;
    nmf(V, K, max_iter, W, H);

    print_matrix(W, "W");
    print_matrix(H, "H");
    double rec_error = reconstruction_error(V, W, H);
    double rel_rec_error = relative_reconstruction_error(V, W, H);

    std::cout << "Reconstruction error (Frobenius norm): " << rec_error << "\n";
    std::cout << "Relative reconstruction error: " << rel_rec_error << "\n";
    return 0;
}

