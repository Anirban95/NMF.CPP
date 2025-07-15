//  Developer: Dr. Anirban Dey, anirban.dey@yahoo.in
//  Date: 15/07/2025


#include "nmf.h"
#include <random>
#include <iomanip>
#include <cmath>

// Constructor
NMF::NMF(const Matrix& V, size_t K, size_t max_iter, double epsilon, double Lambda)
    : V(V), K(K), max_iter(max_iter), epsilon(epsilon), Lambda(Lambda) {
    size_t N = V.size(), M = V[0].size();
    W = Matrix(N, std::vector<double>(K));
    H = Matrix(K, std::vector<double>(M));
    random_initialize(W);
    random_initialize(H);
}

// Destructor
NMF::~NMF() {}

// Factorization with additional constraint in W update
void NMF::factorize() {
    for(size_t iter=0; iter<max_iter; ++iter) {
        // Update H
        Matrix WT = transpose(W);
        Matrix WT_V = multiply(WT, V);
        Matrix WT_W = multiply(WT, W);
        Matrix WT_W_H = multiply(WT_W, H);
        H = elem_multiply(H, elem_divide(WT_V, WT_W_H));

        // Update W with regularization
        Matrix H_T = transpose(H);
        Matrix V_HT = multiply(V, H_T);
        Matrix W_H_HT = multiply(W, multiply(H, H_T));
        Matrix U = compute_U(W);
        Matrix t1 = multiply(U, W);
        Matrix denom = elem_add(W_H_HT, scalar_multiply(t1, Lambda));
        W = elem_multiply(W, elem_divide(V_HT, denom));
    }
}

// Multiply two matrices
Matrix NMF::multiply(const Matrix& A, const Matrix& B) const {
    size_t n = A.size(), m = A[0].size(), p = B[0].size();
    Matrix C(n, std::vector<double>(p, 0.0));
    for(size_t i=0; i<n; ++i)
        for(size_t j=0; j<p; ++j)
            for(size_t k=0; k<m; ++k)
                C[i][j] += A[i][k] * B[k][j];
    return C;
}

// Transpose
Matrix NMF::transpose(const Matrix& A) const {
    size_t rows = A.size(), cols = A[0].size();
    Matrix B(cols, std::vector<double>(rows, 0.0));
    for(size_t i=0; i<rows; ++i)
        for(size_t j=0; j<cols; ++j)
            B[j][i] = A[i][j];
    return B;
}

// Element-wise multiplication
Matrix NMF::elem_multiply(const Matrix& A, const Matrix& B) const {
    size_t n = A.size(), m = A[0].size();
    Matrix C(n, std::vector<double>(m, 0.0));
    for(size_t i=0; i<n; ++i)
        for(size_t j=0; j<m; ++j)
            C[i][j] = A[i][j] * B[i][j];
    return C;
}

// Element-wise division
Matrix NMF::elem_divide(const Matrix& A, const Matrix& B) const {
    size_t n = A.size(), m = A[0].size();
    Matrix C(n, std::vector<double>(m, 0.0));
    for(size_t i=0; i<n; ++i)
        for(size_t j=0; j<m; ++j)
            C[i][j] = A[i][j] / (B[i][j] + epsilon);
    return C;
}

// Element-wise addition
Matrix NMF::elem_add(const Matrix& A, const Matrix& B) const {
    size_t n = A.size(), m = A[0].size();
    Matrix C(n, std::vector<double>(m, 0.0));
    for(size_t i=0; i<n; ++i)
        for(size_t j=0; j<m; ++j)
            C[i][j] = A[i][j] + B[i][j];
    return C;
}

// Scalar multiply
Matrix NMF::scalar_multiply(const Matrix& A, double scalar) const {
    size_t n = A.size(), m = A[0].size();
    Matrix B(n, std::vector<double>(m, 0.0));
    for(size_t i=0; i<n; ++i)
        for(size_t j=0; j<m; ++j)
            B[i][j] = scalar * A[i][j];
    return B;
}

// Initialize matrix randomly
void NMF::random_initialize(Matrix& mat) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    for(auto& row : mat)
        for(auto& val : row)
            val = dis(gen);
}

// Compute diagonal matrix U
Matrix NMF::compute_U(const Matrix& G) const {
    size_t n = G.size();
    Matrix U(n, std::vector<double>(n, 0.0));
    for(size_t i=0; i<n; ++i) {
        double norm = 0.0;
        for(double val : G[i])
            norm += val * val;
        norm = std::sqrt(norm);
        if(norm > epsilon)
            U[i][i] = 1.0 / norm;
    }
    return U;
}

// Frobenius norm
double NMF::frobenius_norm(const Matrix& A) const {
    double sum = 0.0;
    for(const auto& row : A)
        for(double val : row)
            sum += val * val;
    return std::sqrt(sum);
}

// Reconstruction error
double NMF::reconstruction_error() const {
    Matrix WH = multiply(W, H);
    double sum = 0.0;
    for(size_t i=0; i<V.size(); ++i)
        for(size_t j=0; j<V[0].size(); ++j) {
            double diff = V[i][j] - WH[i][j];
            sum += diff * diff;
        }
    return std::sqrt(sum);
}

// Relative reconstruction error
double NMF::relative_reconstruction_error() const {
    double rec_err = reconstruction_error();
    double norm_V = frobenius_norm(V);
    return rec_err / (norm_V + epsilon);
}

// Getters
const Matrix& NMF::get_W() const { return W; }
const Matrix& NMF::get_H() const { return H; }

// Pretty print
std::ostream& operator<<(std::ostream& os, const NMF& nmf) {
    os << "Matrix W:\n";
    for(const auto& row : nmf.W) {
        for(double val : row)
            os << std::setw(8) << std::setprecision(4) << val << " ";
        os << "\n";
    }
    os << "\nMatrix H:\n";
    for(const auto& row : nmf.H) {
        for(double val : row)
            os << std::setw(8) << std::setprecision(4) << val << " ";
        os << "\n";
    }
    return os;
}

