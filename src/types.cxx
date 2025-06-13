#include "types.hxx"

#include <string_view>

using namespace utility;

Matrix::Matrix(
    uint32_t x,
    uint32_t y)
    : data(x*y)
    , dims{x, y} {}

auto Matrix::operator()(
    const uint32_t x,
    const uint32_t y) -> double& {
    if (x >= dims[0] || y >= dims[1])
        throw Error{ std::string_view(__FILE__), Error::ErrorCode::OUT_OF_BOUNDS_ERR };

    return this->data[x + dims[0]*y];
}

template<uint32_t dim>
auto Matrix::get_dimension() const -> uint32_t {
    if constexpr (dim == 0)
        return this->dims[0];
    else if constexpr (dim == 1)
        return this->dims[1];
}

auto Matrix::operator=(Matrix& right) -> Matrix& {
    if (this->get_dimension<0>() != right.get_dimension<0>()
        && this->get_dimension<1>() != right.get_dimension<1>())
        throw Error{ std::string_view(__FILE__), Error::ErrorCode::WRONG_DIMS_ERR };

    for (uint32_t i{0}; i < this->get_dimension<0>(); i++) {
        for (uint32_t j{0}; j < this->get_dimension<1>(); j++) {
            (*this)(i, j) = right(i, j);
        }
    }

    return (*this);
}

auto Matrix::operator+=(Matrix& right) -> Matrix& {
    if (this->get_dimension<0>() != right.get_dimension<0>()
        && this->get_dimension<1>() != right.get_dimension<1>())
        throw Error{ "", Error::ErrorCode::WRONG_DIMS_ERR };

    for (uint32_t i{0}; i < this->get_dimension<0>(); i++) {
        for (uint32_t j{0}; j < this->get_dimension<1>(); j++) {
            (*this)(i, j) = (*this)(i, j) + right(i, j);
        }
    }

    return *this;
}

auto Matrix::operator*(const double factor) -> Matrix& {
    for (uint32_t i{ 0 }; i < this->get_dimension<0>(); i++) {
        for (uint32_t j{ 0 }; j < this->get_dimension<1>(); j++) {
            (*this)(i, j) *= factor;
        }
    }

    return (*this);
}

auto Matrix::operator*(Matrix& right) -> Matrix {
    if (this->get_dimension<1>() != right.get_dimension<0>())
        throw Error{ "", Error::ErrorCode::WRONG_DIMS_ERR };

    Matrix result(
        this->get_dimension<0>(),
        right.get_dimension<1>());
    for (uint32_t i{0}; i < this->get_dimension<0>(); i++) {
        for (uint32_t j{0}; j < right.get_dimension<1>(); j++) {
            for (uint32_t k{0}; k < this->get_dimension<1>(); k++) {
                result(i, j) += (*this)(i, k)*right(k, j);
            }
        }
    }

    return result;
}

