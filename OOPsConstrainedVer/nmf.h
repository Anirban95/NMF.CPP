//Developer: Dr. Anirban Dey, anirban.dey@yahoo.in


#pragma once
#include <vector>
#include <string>
#include <iostream>

typedef std::vector<std::vector<double>> Matrix;

class NMF {
private:
    Matrix V, W, H;
    size_t K;
    size_t max_iter;
    double epsilon;
    double Lambda;   // regularization coefficient

    Matrix multiply(const Matrix& A, const Matrix& B) const;
    Matrix transpose(const Matrix& A) const;
    Matrix elem_multiply(const Matrix& A, const Matrix& B) const;
    Matrix elem_divide(const Matrix& A, const Matrix& B) const;
    Matrix elem_add(const Matrix& A, const Matrix& B) const;
    Matrix scalar_multiply(const Matrix& A, double scalar) const;
    void random_initialize(Matrix& mat);
    Matrix compute_U(const Matrix& G) const;

public:
    NMF(const Matrix& V, size_t K, size_t max_iter=500, double epsilon=1e-9, double Lambda=0.01);
    ~NMF();

    void factorize();

    double reconstruction_error() const;
    double relative_reconstruction_error() const;
    double frobenius_norm(const Matrix& A) const;

    const Matrix& get_W() const;
    const Matrix& get_H() const;

    friend std::ostream& operator<<(std::ostream& os, const NMF& nmf);
};

