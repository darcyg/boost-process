// Copyright (c) 2016 Klemens D. Morgenstern
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROCESS_IO_HPP_
#define BOOST_PROCESS_IO_HPP_

#include <iosfwd>
#include <cstdio>
#include <functional>
#include <utility>
#include <boost/process/detail/config.hpp>
#include <boost/process/pipe.hpp>

#include <future>

#if defined(BOOST_POSIX_API)
#include <boost/process/detail/posix/close_in.hpp>
#include <boost/process/detail/posix/close_out.hpp>
#include <boost/process/detail/posix/null_in.hpp>
#include <boost/process/detail/posix/null_out.hpp>
#include <boost/process/detail/posix/file_in.hpp>
#include <boost/process/detail/posix/file_out.hpp>
#include <boost/process/detail/posix/pipe_in.hpp>
#include <boost/process/detail/posix/pipe_out.hpp>
#include <boost/process/detail/posix/asio_fwd.hpp>
#elif defined(BOOST_WINDOWS_API)
#include <boost/process/detail/windows/close_in.hpp>
#include <boost/process/detail/windows/close_out.hpp>
#include <boost/process/detail/windows/null_in.hpp>
#include <boost/process/detail/windows/null_out.hpp>
#include <boost/process/detail/windows/file_in.hpp>
#include <boost/process/detail/windows/file_out.hpp>
#include <boost/process/detail/windows/pipe_in.hpp>
#include <boost/process/detail/windows/pipe_out.hpp>
#include <boost/process/detail/windows/asio_fwd.hpp>
#endif

/** \file boost/process/io.hpp
 *
 *    Header which provides the io properties. It provides the following properties:
 *
 *   - close
 *   - null
 *   - std_out
 *   - std_err
 *   - std_in
 *
 */

namespace boost { namespace process { namespace detail {


template<typename T> using is_streambuf    = typename std::is_same<T, boost::asio::streambuf>::type;
template<typename T> using is_const_buffer =
        std::integral_constant<bool,
            std::is_same<   boost::asio::const_buffer, T>::value |
            std::is_base_of<boost::asio::const_buffer, T>::value
        >;
template<typename T> using is_mutable_buffer =
        std::integral_constant<bool,
            std::is_same<   boost::asio::mutable_buffer, T>::value |
            std::is_base_of<boost::asio::mutable_buffer, T>::value
        >;


struct null_t  {constexpr null_t() {}};
struct close_t;

template<class>
struct std_in_
{
    constexpr std_in_() {}

    api::close_in close() const {return api::close_in(); }
    api::close_in operator=(const close_t &) const {return api::close_in();}
    api::close_in operator<(const close_t &) const {return api::close_in();}

    api::null_in null() const {return api::null_in();}
    api::null_in operator=(const null_t &) const {return api::null_in();}
    api::null_in operator<(const null_t &) const {return api::null_in();}

    api::file_in operator=(const boost::filesystem::path &p) const {return p;}
    api::file_in operator=(const std::string &p)             const {return p;}
    api::file_in operator=(const char*p)                     const {return p;}

    api::file_in operator<(const boost::filesystem::path &p) const {return p;}
    api::file_in operator<(const std::string &p)             const {return p;}
    api::file_in operator<(const char*p)                     const {return p;}

    api::file_in operator=(FILE * f)                         const {return f;}
    api::file_in operator<(FILE * f)                         const {return f;}

    template<typename Char, typename Traits> api::pipe_in operator=(const basic_pipe<Char, Traits> & p)      const {return p;}
    template<typename Char, typename Traits> api::pipe_in operator<(const basic_pipe<Char, Traits> & p)      const {return p;}
    template<typename Char, typename Traits> api::pipe_in operator=(const basic_opstream<Char, Traits> & p)  const {return p.pipe();}
    template<typename Char, typename Traits> api::pipe_in operator<(const basic_opstream<Char, Traits> & p)  const {return p.pipe();}
    template<typename Char, typename Traits> api::pipe_in operator=(const basic_pstream <Char, Traits> & p)  const {return p.pipe();}
    template<typename Char, typename Traits> api::pipe_in operator<(const basic_pstream <Char, Traits> & p)  const {return p.pipe();}

    api::pipe_in operator=(const async_pipe & p) const {return p;}
    api::pipe_in operator<(const async_pipe & p) const {return p;}


    template<typename T, typename = typename std::enable_if<
            is_const_buffer<T>::value || is_mutable_buffer<T>::value
            >::type>
    api::async_in_buffer<const T> operator=(const T & buf) const {return buf;}
    template<typename T, typename = typename std::enable_if<is_streambuf<T>::value>::type >
    api::async_in_buffer<T>       operator=(T       & buf) const {return buf;}

    template<typename T, typename = typename std::enable_if<
            is_const_buffer<T>::value || is_mutable_buffer<T>::value
            >::type>
    api::async_in_buffer<const T> operator<(const T & buf) const {return buf;}
    template<typename T, typename = typename std::enable_if<is_streambuf<T>::value>::type >
    api::async_in_buffer<T>       operator<(T       & buf) const {return buf;}

};

//-1 == empty.
//1 == stdout
//2 == stderr
template<int p1, int p2 = -1>
struct std_out_
{
    constexpr std_out_() {}

    api::close_out<p1,p2> close() const {return api::close_out<p1,p2>(); }
    api::close_out<p1,p2> operator=(const close_t &) const {return api::close_out<p1,p2>();}
    api::close_out<p1,p2> operator>(const close_t &) const {return api::close_out<p1,p2>();}

    api::null_out<p1,p2> null() const {return api::null_out<p1,p2>();}
    api::null_out<p1,p2> operator=(const null_t &) const {return api::null_out<p1,p2>();}
    api::null_out<p1,p2> operator>(const null_t &) const {return api::null_out<p1,p2>();}

    api::file_out<p1,p2> operator=(const boost::filesystem::path &p) const {return api::file_out<p1,p2>(p);}
    api::file_out<p1,p2> operator=(const std::string &p)             const {return api::file_out<p1,p2>(p);}
    api::file_out<p1,p2> operator=(const char*p)                     const {return api::file_out<p1,p2>(p);}

    api::file_out<p1,p2> operator>(const boost::filesystem::path &p) const {return api::file_out<p1,p2>(p);}
    api::file_out<p1,p2> operator>(const std::string &p)             const {return api::file_out<p1,p2>(p);}
    api::file_out<p1,p2> operator>(const char*p)                     const {return api::file_out<p1,p2>(p);}

    api::file_out<p1,p2> operator=(FILE * f)  const {return f;}
    api::file_out<p1,p2> operator>(FILE * f)  const {return f;}

    template<typename Char, typename Traits> api::pipe_out<p1,p2> operator=(const basic_pipe<Char, Traits> & p)      const {return p;}
    template<typename Char, typename Traits> api::pipe_out<p1,p2> operator>(const basic_pipe<Char, Traits> & p)      const {return p;}
    template<typename Char, typename Traits> api::pipe_out<p1,p2> operator=(const basic_ipstream<Char, Traits> & p)  const {return p.pipe();}
    template<typename Char, typename Traits> api::pipe_out<p1,p2> operator>(const basic_ipstream<Char, Traits> & p)  const {return p.pipe();}
    template<typename Char, typename Traits> api::pipe_out<p1,p2> operator=(const basic_pstream <Char, Traits> & p)  const {return p.pipe();}
    template<typename Char, typename Traits> api::pipe_out<p1,p2> operator>(const basic_pstream <Char, Traits> & p)  const {return p.pipe();}

    api::pipe_out<p1, p2> operator=(const async_pipe & p) const {return api::pipe_out<p1, p2>(p);}
    api::pipe_out<p1, p2> operator>(const async_pipe & p) const {return api::pipe_out<p1, p2>(p);}

    api::async_out_buffer<p1, p2, const asio::mutable_buffer>     operator=(const asio::mutable_buffer & buf)     const {return buf;}
    api::async_out_buffer<p1, p2, const asio::mutable_buffers_1> operator=(const asio::mutable_buffers_1 & buf) const {return buf;}
    api::async_out_buffer<p1, p2, asio::streambuf>               operator=(asio::streambuf & os)                   const {return os ;}

    api::async_out_buffer<p1, p2, const asio::mutable_buffer>     operator>(const asio::mutable_buffer & buf)     const {return buf;}
    api::async_out_buffer<p1, p2, const asio::mutable_buffers_1> operator>(const asio::mutable_buffers_1 & buf) const {return buf;}
    api::async_out_buffer<p1, p2, asio::streambuf>               operator>(asio::streambuf & os)                   const {return os ;}

    api::async_out_future<p1,p2, std::string>       operator=(std::future<std::string> & fut)       const { return fut;}
    api::async_out_future<p1,p2, std::string>       operator>(std::future<std::string> & fut)       const { return fut;}
    api::async_out_future<p1,p2, std::vector<char>> operator=(std::future<std::vector<char>> & fut) const { return fut;}
    api::async_out_future<p1,p2, std::vector<char>> operator>(std::future<std::vector<char>> & fut) const { return fut;}

    template<int pin, typename = typename std::enable_if<
            (((p1 == 1) && (pin == 2)) ||
             ((p1 == 2) && (pin == 1)))
             && (p2 == -1)>::type>
    constexpr std_out_<1, 2> operator& (const std_out_<pin> &lhs) const
    {
        return std_out_<1, 2> ();
    }

};

struct close_t
{
    constexpr close_t() {}
    template<int T, int U>
    api::close_out<T,U> operator()(std_out_<T,U>) {return api::close_out<T,U>();}
};

constexpr static close_t close;
constexpr static null_t  null;
constexpr static std_in_<void>   std_in;
constexpr static std_out_<1> std_out;
constexpr static std_out_<2> std_err;

}

using boost::process::detail::close;
using boost::process::detail::null;
using boost::process::detail::std_out;
using boost::process::detail::std_err;
using boost::process::detail::std_in;

}}
#endif /* INCLUDE_BOOST_PROCESS_IO_HPP_ */
