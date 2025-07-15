//  Developer: Dr. Anirban Dey, anirban.dey@yahoo.in
//  Date: 15/07/2025



#include "nmf.h"
#include <random>
#include <iostream>
#include <iomanip>
#include <cmath>


double epsilon = 1e-9;

// Initialize matrix with random values
void random_initialize(Matrix& mat, std::mt19937& gen) {
    std::uniform_real_distribution<> dis(0.0, 1.0);
    for(auto& row : mat)
        for(auto& val : row)
            val = dis(gen);
}

// Transpose of a matrix
Matrix transpose(const Matrix& A) {
    size_t rows = A.size(), cols = A[0].size();
    Matrix B(cols, std::vector<double>(rows, 0.0));
    for(size_t i=0; i<rows; ++i)
        for(size_t j=0; j<cols; ++j)
            B[j][i] = A[i][j];
    return B;
}

// Matrix multiplication: A (n x m) * B (m x p) = C (n x p)
Matrix multiply(const Matrix& A, const Matrix& B) {
    size_t n = A.size(), m = A[0].size(), p = B[0].size();
    Matrix C(n, std::vector<double>(p, 0.0));
    for(size_t i=0; i<n; ++i)
        for(size_t j=0; j<p; ++j)
            for(size_t k=0; k<m; ++k)
                C[i][j] += A[i][k] * B[k][j];
    return C;
}

// Element-wise division: A ./ B
Matrix elem_divide(const Matrix& A, const Matrix& B) {
    size_t n = A.size(), m = A[0].size();
    Matrix C(n, std::vector<double>(m, 0.0));
    for(size_t i=0; i<n; ++i)
        for(size_t j=0; j<m; ++j)
            C[i][j] = A[i][j] / (B[i][j] + epsilon);
    return C;
}

// Element-wise multiplication: A .* B
Matrix elem_multiply(const Matrix& A, const Matrix& B) {
    size_t n = A.size(), m = A[0].size();
    Matrix C(n, std::vector<double>(m, 0.0));
    for(size_t i=0; i<n; ++i)
        for(size_t j=0; j<m; ++j)
            C[i][j] = A[i][j] * B[i][j];
    return C;
}

// NMF implementation
void nmf(const Matrix& V, size_t K, size_t max_iter, Matrix& W, Matrix& H) {
    size_t N = V.size(), M = V[0].size();
    std::random_device rd;
    std::mt19937 gen(rd());

    // Initialize W (N x K) and H (K x M)
    W = Matrix(N, std::vector<double>(K));
    H = Matrix(K, std::vector<double>(M));
    random_initialize(W, gen);
    random_initialize(H, gen);

    for(size_t iter=0; iter<max_iter; ++iter) {
        // Update H
        Matrix WT = transpose(W);
        Matrix WT_V = multiply(WT, V);
        Matrix WT_W = multiply(WT, W);
        Matrix WT_W_H = multiply(WT_W, H);
        H = elem_multiply(H, elem_divide(WT_V, WT_W_H));

        // Update W
        Matrix H_T = transpose(H);
        Matrix V_HT = multiply(V, H_T);
        Matrix W_H_HT = multiply(W, multiply(H, H_T));
        W = elem_multiply(W, elem_divide(V_HT, W_H_HT));
    }
}


// Compute Frobenius norm of a matrix
double frobenius_norm(const Matrix& A) {
    double sum = 0.0;
    for(const auto& row : A)
        for(double val : row)
            sum += val * val;
    return std::sqrt(sum);
}

// Compute reconstruction error: ||V - W*H||_F
double reconstruction_error(const Matrix& V, const Matrix& W, const Matrix& H) {
    Matrix WH = multiply(W, H);
    double sum = 0.0;
    for(size_t i=0; i<V.size(); ++i)
        for(size_t j=0; j<V[0].size(); ++j) {
            double diff = V[i][j] - WH[i][j];
            sum += diff * diff;
        }
    return std::sqrt(sum);
}

// Relative reconstruction error: ||V - WH||_F / ||V||_F
double relative_reconstruction_error(const Matrix& V, const Matrix& W, const Matrix& H) {
    double rec_err = reconstruction_error(V, W, H);
    double norm_V = frobenius_norm(V);
    return rec_err / (norm_V + epsilon);
}


// Print matrix nicely
void print_matrix(const Matrix& A, const std::string& name) {
    std::cout << name << " = \n";
    for(const auto& row : A) {
        for(auto val : row)
            std::cout << std::setw(8) << std::setprecision(4) << val << " ";
        std::cout << "\n";
    }
    std::cout << "\n";
}

