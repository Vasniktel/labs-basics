/**
 *  big.cpp - library for multiprecision calculations (beta)
 *
 *
 *  Copyright (c) 2017-2018 Vasniktel <vasniktel@gmail.com>
 *
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.

 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 *
 *  @author Vasniktel <vasniktel@gmail.com>
 *  @version 0.1 beta 1
 */


#include <vector>
#include <cctype>
#include <cmath>
#include <string>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <type_traits>
#include <utility>
#include <stdexcept>


namespace big
{

class big_num_error : public std::runtime_error
{
    public:

        big_num_error(const std::string& __msg)
        : std::runtime_error(__msg)
        { }
};

class assign_error : public big_num_error
{
    public:

        assign_error(const std::string& __msg = "Assignment error has occurred!")
        : big_num_error(__msg)
        { }
};

class calc_error : public big_num_error
{
    public:

        calc_error(const std::string& __msg = "Calculation error has occurred!")
        : big_num_error(__msg)
        { }
};

class out_error : public big_num_error
{
    public:

        out_error(const std::string& __msg = "Output error has occurred!")
        : big_num_error(__msg)
        { }
};

class big_num
{
    public:

        /*enum class state_t
        {
            BIGNUM_GOOD, //BUGGY
            BIGNUM_BAD_ASSIGNMENT,
            BIGNUM_BAD_OUTPUT,
            BIGNUM_BAD_CALCULATION,
            BIGNUM_BAD_COMPARISON,
            //ZERO_DIVISION //for future
        };*/

        enum class sign_t
        {
            POSITIVE,
            NEGATIVE,
            ZERO
        };

        big_num() = default;

        big_num(const big_num& obj) = default;

        big_num(big_num&& obj) = default;

        template<typename integral_type>
        big_num(integral_type num)
        {
          this->assign(num);
        }

        template<typename iterator_type>
        big_num(iterator_type& begin, iterator_type& end, sign_t sign = big_num::sign_t::POSITIVE)
        { this->assign(begin, end, sign); }

        big_num(const std::string& s, sign_t sign = big_num::sign_t::POSITIVE)
        { this->assign(s, sign); }

        big_num(const char* s, sign_t sign = big_num::sign_t::POSITIVE)
        { this->assign(std::string(s), sign); }

        template<typename vector_type>
        big_num(const std::vector<vector_type>& vec, sign_t sign = big_num::sign_t::POSITIVE)
        { this->assign(vec, sign); }

        ~big_num() = default;

        sign_t get_sign() const
        { return this->_sign; }

        void set_sign(const sign_t& sign)
        { this->_sign = sign; }

        std::size_t depth() const
        { return this->_num.size(); }

        friend std::ostream& operator<<(std::ostream& out, const big_num& obj)
        {
            if (out.fail())
            {
                throw big::out_error();
            }

            if (obj._sign == sign_t::ZERO)
            {
                out << 0;
            }
            else if (!obj._num.empty())
            {
                if (obj._sign == sign_t::NEGATIVE)
                {
                    out << '-';
                }

                std::copy(obj._num.rbegin(), obj._num.rend(), std::ostream_iterator<int>(out));

                if (out.fail())
                {
                    throw big::out_error();
                }
            }
            else
            {
                throw big::out_error("Output error: uninitialized object!");
            }

            return out;
        }

        friend std::istream& operator>>(std::istream& in, big_num& obj)
        {
            if (in.fail())
            {
                throw big::assign_error();
            }

            std::string s;
            in >> s;

            if (in.fail() || s.empty())
            {
                throw big::assign_error();
            }

            sign_t sign = sign_t::POSITIVE;

            if (s[0] == '-')
            {
                sign = sign_t::NEGATIVE;
                s.erase(s.begin());
            }
            else if (s[0] == '+')
            {
                s.erase(s.begin());
            }

            obj.assign(s, sign);

            return in;
        }

        template
        <
            typename integral_type,
            typename = std::enable_if_t<std::is_integral<integral_type>::value>
        >
        big_num& assign(integral_type num)
        {
            if (!std::is_integral<integral_type>::value)
            {
                throw big::assign_error("Assignment error: type of argument is not an integral!");
            }

            sign_t __sign = num > 0 ?
                sign_t::POSITIVE :
                num < 0 ?
                    sign_t::NEGATIVE :
                    sign_t::ZERO;

            std::vector<char> __num;

            num = std::abs(num);

            for (; num > 0; num /= 10)
            {
                __num.push_back(num % 10);
            }

            this->_num = __num;
            this->_sign = __sign;

            return *this;
        }

        big_num& assign(const char* s, sign_t sign = big_num::sign_t::POSITIVE)
        { return this->assign(std::string(s), sign); }

        big_num& assign(const big_num& obj)
        {
            if (obj._sign != sign_t::ZERO && obj._num.empty()) ////TODO
            {
                throw big::assign_error();
            }

            this->_sign = obj._sign;
            this->_num = obj._num;

            return *this;
        }

        big_num& assign(big_num&& obj)
        {
            if (obj._sign != sign_t::ZERO && obj._num.empty()) ////TODO
            {
                throw big::assign_error();
            }

            if (this != &obj)
            {
                this->_num = std::move(obj._num);
                this->_sign = obj._sign;
            }

            return *this;
        }

        template
        <
            typename vector_type,
            typename = std::enable_if_t<std::is_integral<vector_type>::value>
        >
        big_num& assign(const std::vector<vector_type>& vec, sign_t sign = big_num::sign_t::POSITIVE)
        {
            if (!std::is_integral<vector_type>::value || vec.empty() || ((vec[0] == 0 || vec[0] == '0') && vec.size() > 1))
            {
                throw big::assign_error("Assignment error: invalid argument!");
            }

            std::vector<char> __num;

            if (vec[0] == 0 || vec[0] == '0')
            {
                sign = sign_t::ZERO;
            }
            else
            {
                __num.resize(vec.size());

                for (auto it = vec.rbegin(), end = vec.rend(); it != end; ++it)
                {
                    if (this->is_valid(*it))
                    {
                        __num[it - vec.rbegin()] = *it - (std::isdigit(*it) ? '0' : 0);
                    }
                    else
                    {
                        throw big::assign_error("Assignment error: invalid vector value!");
                    }
                }
            }

            this->_num = __num;
            this->_sign = sign;

            return *this;
        }

        big_num& assign(const std::string& s, sign_t sign = big_num::sign_t::POSITIVE)
        {
            if (s.empty() || (s[0] == '0' && s.length() > 1))
            {
                throw big::assign_error();
            }

            std::vector<char> __num;

            if (s[0] == '0')
            {
                sign = sign_t::ZERO;
            }
            else
            {
                __num.resize(s.length());

                for (auto it = s.rbegin(), end = s.rend(); it != end; ++it)
                {
                    if (this->is_valid(*it))
                    {
                        __num[it - s.rbegin()] = *it - (std::isdigit(*it) ? '0' : 0);
                    }
                    else
                    {
                        throw big::assign_error("Assignment error: invalid argument!");
                    }
                }
            }

            this->_num = __num;
            this->_sign = sign;

            return *this;
        }

        template
        <
            typename iterator_type,
            typename = std::enable_if_t<std::is_class<iterator_type>::value || std::is_pointer<iterator_type>::value>
        >
        big_num& assign(iterator_type& begin, iterator_type& end, sign_t sign = big_num::sign_t::POSITIVE)
        {
            if ((!std::is_class<iterator_type>::value && !std::is_pointer<iterator_type>::value) ||
                begin == end || ((*begin == '0' || *begin == 0) && begin != end - 1))
            {
                throw big::assign_error();
            }

            std::vector<char> __num;

            if (*begin == '0' || *begin == 0)
            {
                sign = sign_t::ZERO;
            }
            else
            {
                __num.resize(end - begin);

                for (iterator_type it = end - 1; it >= begin; --it)
                {
                    if (this->is_valid(*it))
                    {
                        __num[end - it - 1] = *it - (std::isdigit(*it) ? '0' : 0);
                    }
                    else
                    {
                        throw big::assign_error("Assignment error: invalid sequence value!");
                    }
                }
            }

            this->_num = __num;
            this->_sign = sign;

            return *this;
        }

        friend bool operator>(const big_num& obj_1, const big_num& obj_2)
        {
            if (obj_1._sign != obj_2._sign)
            {
                if (obj_2._sign == sign_t::ZERO)
                    return obj_1._sign == sign_t::POSITIVE;
                else if (obj_1._sign == sign_t::ZERO)
                    return obj_2._sign == sign_t::NEGATIVE;
                else
                    return obj_1._sign == sign_t::POSITIVE;
            }

            if (obj_1._sign == sign_t::ZERO && obj_2._sign == sign_t::ZERO)
                return false;

            if (obj_1._num.size() != obj_2._num.size())
                return obj_1._num.size() > obj_2._num.size();


            for (auto obj_1_it = obj_1._num.rbegin(),
                        end = obj_1._num.rend(),
                        obj_2_it = obj_2._num.rbegin();
                        obj_1_it != end; ++obj_1_it, ++obj_2_it)
            {
                if (*obj_1_it != *obj_2_it)
                    return *obj_1_it > *obj_2_it;
            }

            return false;
        }

        friend bool operator<(const big_num& obj_1, const big_num& obj_2)
        { return obj_2 > obj_1; }

        friend bool operator>=(const big_num& obj_1, const big_num& obj_2)
        { return !(obj_2 > obj_1); }

        friend bool operator<=(const big_num& obj_1, const big_num& obj_2)
        { return !(obj_1 > obj_2); }

        friend bool operator==(const big_num& obj_1, const big_num& obj_2)
        { return (obj_1._num == obj_2._num && obj_1._sign == obj_2._sign); }

        friend bool operator!=(const big_num& obj_1, const big_num& obj_2)
        { return !(obj_1 == obj_2); }

        big_num& operator=(const char*) = delete;

        big_num& operator=(const std::string&) = delete;

        template<typename vector_type>
        big_num& operator=(const std::vector<vector_type>&) = delete;

        big_num& operator=(const big_num&) = default;

        big_num& operator=(big_num&&) = default;

        template
        <
            typename integral_type,
            typename = std::enable_if_t<std::is_integral<integral_type>::value>
        >
        big_num& operator=(integral_type num)
        { return this->assign(num); }

        friend big_num operator+(const big_num& obj_1, const big_num& obj_2) noexcept/// DONE
        {
            if (obj_1._sign != obj_2._sign && obj_1._sign != sign_t::ZERO && obj_2._sign != sign_t::ZERO)
            {
                big_num result = obj_1.abs() - obj_2.abs();

                if (result._sign == sign_t::NEGATIVE)
                    result._sign = obj_2._sign;
                else if (result._sign == sign_t::POSITIVE)
                    result._sign = obj_1._sign;

                return result;
            }

            if (obj_1._sign == sign_t::ZERO)
                return obj_2;
            else if (obj_2._sign == sign_t::ZERO)
                return obj_1;

            big_num result;
            result._num.resize(std::max(obj_1._num.size(), obj_2._num.size()) + 1, 0);
            result._sign = obj_1._sign;

            auto obj_1_it = obj_1._num.cbegin(),
                 obj_1_end = obj_1._num.cend(),
                 obj_2_it = obj_2._num.cbegin(),
                 obj_2_end = obj_2._num.cend();

            for (auto it = result._num.begin(),
                      end = result._num.end();
                      it != end; ++it, ++obj_1_it, ++obj_2_it)
            {
                *it += (obj_1_it < obj_1_end ? *obj_1_it : 0) + (obj_2_it < obj_2_end ? *obj_2_it : 0);

                if (*it >= 10)
                {
                    auto add = *it / 10;
                    *(it + 1) += add;
                    *it -= add * 10;
                }
            }

            result.delete_zeroes();

            return result;
        }

        friend big_num operator-(const big_num& obj_1, const big_num& obj_2) noexcept //DONE
        {
            if (obj_1._sign != obj_2._sign && obj_1._sign != sign_t::ZERO && obj_2._sign != sign_t::ZERO)
            {
                big_num result = obj_1.abs() + obj_2.abs();

                result._sign = obj_1._sign;

                return result;
            }

            if (obj_1 == obj_2)
                return big_num(0);

            if (obj_1._sign == sign_t::ZERO)
            {
                big_num result = obj_2;

                result._sign = obj_2._sign == sign_t::POSITIVE ? sign_t::NEGATIVE : sign_t::POSITIVE;

                return result;
            }
            else if (obj_2._sign == sign_t::ZERO)
            {
                return obj_1;
            }

            std::vector<char>::const_iterator big_it, big_end, small_it, small_end;
            sign_t __sign;

            if (obj_1.abs() > obj_2.abs())
            {
                big_it = obj_1._num.cbegin();
                big_end = obj_1._num.cend();
                small_it = obj_2._num.cbegin();
                small_end = obj_2._num.cend();
                __sign = obj_1._sign;
            }
            else
            {
                big_it = obj_2._num.cbegin();
                big_end = obj_2._num.cend();
                small_it = obj_1._num.cbegin();
                small_end = obj_1._num.cend();
                __sign = obj_2._sign == sign_t::POSITIVE ? sign_t::NEGATIVE : sign_t::POSITIVE;
            }

            big_num result;
            result._num.resize(std::max(obj_1._num.size(), obj_2._num.size()), 0);
            result._sign = __sign;

            for (auto it = result._num.begin(),
                      end = result._num.end();
                      it != end; ++it, ++small_it, ++big_it)
            {

                *it += (big_it < big_end ? *big_it : 0) - (small_it < small_end ? *small_it : 0);

                if (*it < 0)
                {
                    (*(it + 1))--;
                    *it += 10;
                }
            }

            result.delete_zeroes();

            return result;
        }

        friend big_num operator*(const big_num& obj_1, const big_num& obj_2) noexcept // DONE
        {
            if (obj_1._sign == sign_t::ZERO || obj_2._sign == sign_t::ZERO)
            {
                return big_num(0);
            }

            big_num result;
            result._num.resize(obj_1._num.size() + obj_2._num.size() + 1, 0);
            result._sign = obj_1._sign == obj_2._sign ? sign_t::POSITIVE : sign_t::NEGATIVE;

            auto obj_1_begin = obj_1._num.cbegin(), obj_2_begin = obj_2._num.cbegin();
            auto res_begin = result._num.begin();

            for (auto it_1 = obj_1_begin, end_1 = obj_1._num.cend(); it_1 != end_1; ++it_1)
            {
                for (auto it_2 = obj_2_begin, end_2 = obj_2._num.cend(); it_2 != end_2; ++it_2)
                {
                    auto it = res_begin + (it_1 - obj_1_begin + it_2 - obj_2_begin);

                    *it += *it_1 * *it_2;

                    if (*it >= 10)
                    {
                        auto add = *it / 10;
                        *(it + 1) += add;
                        *it -= add * 10;
                    }
                }
            }

            result.delete_zeroes();

            return result;
        }

        big_num pow(unsigned long long power) const noexcept
        {
            big_num result(1), temp(*this);

            for (; power > 0; power >>= 1)
            {
                if (power & 1)
                    result *= temp;

                temp *= temp;
            }

            return result;
        }

        /*friend big_num operator/(const big_num& obj_1, const big_num& obj_2)
        {
            try
            {
                if (obj_1._state != state_t::BIGNUM_GOOD || obj_2._state != state_t::BIGNUM_GOOD)
                    throw state_t::BIGNUM_BAD_CALCULATION;

                if (obj_2._sign == sign_t::ZERO)
                    throw state_t::ZERO_DIVISION;

                if (obj_1._sign == sign_t::ZERO || obj_1.abs() < obj_2.abs())
                    return big_num(0);

                if (obj_1 == obj_2)
                    return big_num(1);

                if (obj_2 == big_num(1))
                    return obj_1;

                big_num result(state_t::BIGNUM_GOOD);
                result._num.resize(obj_1._num.size(), 0);
                result._sign = obj_1._sign == obj_2._sign ? sign_t::POSITIVE : sign_t::NEGATIVE;

                {
                    auto obj_1_it =

                    for (auto

            }
            catch (const big_num::state_t e)
            {
                if (obj_1._state == state_t::BIGNUM_GOOD)
                    obj_1._state = e;
                else if (obj_2._state == state_t::BIGNUM_GOOD)
                    obj_2._state = e;

                return big_num(e);
            }
        }*/

        big_num& operator+=(const big_num& obj)
        { return this->assign(*this + obj); }

        big_num& operator-=(const big_num& obj)
        { return this->assign(*this - obj); }

        big_num& operator*=(const big_num& obj)
        { return this->assign(*this * obj); }

        /*big_num& operator/=(const big_num& obj)
        { return this->assign(*this / obj); }*/

        big_num& operator++()
        { return *this += 1; }

        big_num& operator--()
        { return *this -= 1; }

        big_num operator++(int)
        {
            big_num result(*this);

            ++(*this);

            return result;
        }

        big_num operator--(int)
        {
            big_num result(*this);

            --(*this);

            return result;
        }

        big_num abs() const noexcept
        {
            big_num result(*this);

            if (result._sign == sign_t::NEGATIVE)
            {
                result._sign = sign_t::POSITIVE;
            }

            return result;
        }

        big_num fact() const
        {
            if (this->_sign == sign_t::NEGATIVE)
            {
                throw big::calc_error("Calculation error: number must be positive or zero to calculate factorial!");
            }

            big_num result(1);

            for (big_num num = *this, zero = 0; num > zero; --num)
            {
                result *= num;
            }

            return result;
        }

        std::string to_string() const
        {
            std::string result;
            std::vector<char> num(this->_num.size());
            transform(this->_num.begin(), this->_num.end(), num.begin(), [](const char& c)
            {
                return c + '0';
            });

            if (this->_sign == sign_t::NEGATIVE)
            {
                result = "-" + std::string(num.rbegin(), num.rend());
            }
            else if (this->_sign == sign_t::POSITIVE)
            {
                result = std::string(num.rbegin(), num.rend());
            }
            else
            {
                result = "0";
            }
            return result;
        }

    private:

        template<typename T>
        inline bool is_valid(const T& c) const
        { return (std::is_integral<T>::value && ((c >= 0 && c <= 9) || std::isdigit(c))); }

        inline void delete_zeroes()
        {
            for (auto it = this->_num.rbegin(), end = this->_num.rend(); it != end; ++it)
            {
                if (*it == 0)
                    this->_num.pop_back();

                else
                    break;
            }
        }

        std::vector<char> _num;
        sign_t _sign = sign_t::ZERO;
};

} //namespace big
