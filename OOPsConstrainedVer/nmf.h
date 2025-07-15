// nmf.h
#pragma once
#include <vector>
#include <string>

typedef std::vector<std::vector<double>> Matrix;

void nmf(const Matrix& V, size_t K, size_t max_iter, Matrix& W, Matrix& H);
void print_matrix(const Matrix& A, const std::string& name);

// These functions will compute the reconstruction performance of the decomposed matrices
double frobenius_norm(const Matrix& A);
double reconstruction_error(const Matrix& V, const Matrix& W, const Matrix& H);
double relative_reconstruction_error(const Matrix& V, const Matrix& W, const Matrix& H);
