#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <algorithm>
#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <string>

template <typename T>
class Matrix {
   public:
    Matrix(std::size_t rows, std::size_t cols)
        : rows_(rows), cols_(cols), data_(nullptr) {
        allocateMemory();
    }

    Matrix(std::initializer_list<std::initializer_list<T>> list)
        : rows_(list.size()),
          cols_(rows_ ? list.begin()->size() : 0),
          data_(nullptr) {
        allocateMemory();
        auto it = list.begin();
        for (std::size_t i = 0; i < rows_; ++i) {
            if (it->size() != cols_) {
                throw std::invalid_argument(
                    std::string(
                        "Initializer list is not rectangular at line ") +
                    std::to_string(__LINE__) + ", function: " + __func__);
            }
            std::copy(it->begin(), it->end(), data_ + i * cols_);
            ++it;
        }
    }

    ~Matrix() { deallocateMemory(); }

    Matrix(const Matrix& other)
        : rows_(other.rows_), cols_(other.cols_), data_(nullptr) {
        allocateMemory();
        copyFrom(other);
    }

    Matrix(Matrix&& other) noexcept : rows_(0), cols_(0), data_(nullptr) {
        moveFrom(std::move(other));
    }

    Matrix& operator=(const Matrix& other) {
        if (this != &other) {
            deallocateMemory();
            rows_ = other.rows_;
            cols_ = other.cols_;
            allocateMemory();
            copyFrom(other);
        }
        return *this;
    }

    Matrix& operator=(Matrix&& other) noexcept {
        if (this != &other) {
            deallocateMemory();
            moveFrom(std::move(other));
        }
        return *this;
    }

    T& operator()(std::size_t row, std::size_t col) {
        if (row >= rows_ || col >= cols_) {
            throw std::out_of_range(
                std::string("Index out of range: row=") + std::to_string(row) +
                ", col=" + std::to_string(col) +
                ", max_row=" + std::to_string(rows_) +
                ", max_col=" + std::to_string(cols_) + " at line " +
                std::to_string(__LINE__) + ", function: " + __func__);
        }
        return data_[row * cols_ + col];
    }

    const T& operator()(std::size_t row, std::size_t col) const {
        if (row >= rows_ || col >= cols_) {
            throw std::out_of_range(
                std::string("Index out of range: row=") + std::to_string(row) +
                ", col=" + std::to_string(col) +
                ", max_row=" + std::to_string(rows_) +
                ", max_col=" + std::to_string(cols_) + " at line " +
                std::to_string(__LINE__) + ", function: " + __func__);
        }
        return data_[row * cols_ + col];
    }

    std::size_t rows() const { return rows_; }
    std::size_t cols() const { return cols_; }

    Matrix operator+(const Matrix& other) const {
        if (rows_ != other.rows_ || cols_ != other.cols_) {
            throw std::invalid_argument(
                std::string(
                    "Matrices must have the same dimensions for addition: ") +
                "rows1=" + std::to_string(rows_) +
                ", cols1=" + std::to_string(cols_) +
                ", rows2=" + std::to_string(other.rows_) +
                ", cols2=" + std::to_string(other.cols_) + " at line " +
                std::to_string(__LINE__) + ", function: " + __func__);
        }
        Matrix result(rows_, cols_);
        for (std::size_t i = 0; i < rows_ * cols_; ++i) {
            result.data_[i] = data_[i] + other.data_[i];
        }
        return result;
    }

    Matrix operator-(const Matrix& other) const {
        if (rows_ != other.rows_ || cols_ != other.cols_) {
            throw std::invalid_argument(
                std::string("Matrices must have the same dimensions for "
                            "subtraction: ") +
                "rows1=" + std::to_string(rows_) +
                ", cols1=" + std::to_string(cols_) +
                ", rows2=" + std::to_string(other.rows_) +
                ", cols2=" + std::to_string(other.cols_) + " at line " +
                std::to_string(__LINE__) + ", function: " + __func__);
        }
        Matrix result(rows_, cols_);
        for (std::size_t i = 0; i < rows_; ++i) {
            for (std::size_t j = 0; j < cols_; ++j) {
                result(i, j) = (*this)(i, j) - other(i, j);
            }
        }
        return result;
    }

    bool operator==(const Matrix& other) const {
        if (rows_ != other.rows_ || cols_ != other.cols_) {
            return false;
        }
        for (size_t i = 0; i < rows_; ++i) {
            for (size_t j = 0; j < cols_; ++j) {
                if ((*this)(i, j) != other(i, j)) {
                    return false;
                }
            }
        }
        return true;
    }

    bool operator!=(const Matrix& other) const { return !(*this == other); }

    Matrix operator*(const Matrix& other) const {
        if (cols_ != other.rows_) {
            throw std::invalid_argument(
                std::string("Number of columns in first matrix must be equal "
                            "to number of rows in second matrix: ") +
                "cols1=" + std::to_string(cols_) +
                ", rows2=" + std::to_string(other.rows_) + " at line " +
                std::to_string(__LINE__) + ", function: " + __func__);
        }
        Matrix result(rows_, other.cols_);
        for (std::size_t i = 0; i < rows_; ++i) {
            for (std::size_t j = 0; j < other.cols_; ++j) {
                T sum = 0;
                for (std::size_t k = 0; k < cols_; ++k) {
                    sum += (*this)(i, k) * other(k, j);
                }
                result(i, j) = sum;
            }
        }
        return result;
    }

    Matrix transpose() const {
        Matrix result(cols_, rows_);
        for (std::size_t i = 0; i < rows_; ++i) {
            for (std::size_t j = 0; j < cols_; ++j) {
                result(j, i) = (*this)(i, j);
            }
        }
        return result;
    }

   private:
    void allocateMemory() {
        if (rows_ && cols_) {
            data_ = new T[rows_ * cols_];
        }
    }

    void deallocateMemory() {
        delete[] data_;
        data_ = nullptr;
    }

    void copyFrom(const Matrix& other) {
        std::copy(other.data_, other.data_ + other.rows_ * other.cols_, data_);
    }

    void moveFrom(Matrix&& other) {
        rows_ = other.rows_;
        cols_ = other.cols_;
        data_ = other.data_;
        other.rows_ = 0;
        other.cols_ = 0;
        other.data_ = nullptr;
    }

    std::size_t rows_;
    std::size_t cols_;
    T* data_;
};

#endif  // MATRIX_HPP