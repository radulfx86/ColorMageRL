#ifndef _LOG_H_
#define _LOG_H_

template<typename S, typename A>
S&_logfun(S &s, const A &a)
{
    return (s << a);
}

template<typename S, typename A, typename... B>
S &_logfun(S &s, const A &a, const B&... b)
{
    return _logfun((s << a), b...);
}

#define LOG(...) \
do {\
    _logfun(std::cout,"LOG: ",__VA_ARGS__, "\n"); \
} while(0)

#endif // _LOG_H_