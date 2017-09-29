#ifndef COMMON_TIME_STAMP_HH
#define COMMON_TIME_STAMP_HH

#include <iosfwd>
#include <iomanip>
#include <thread>
#include <mutex>
#include <chrono>

#define THREAD_CERR if(common::CerrLocker lock = common::CerrLocker()); else std::cerr << common::ThreadTimeStamp()

namespace common {

class ThreadTimeStamp
{
public:
};

class CerrLocker
{
    static std::recursive_mutex cerrMutex_;
    std::lock_guard<std::recursive_mutex> lock_;
public:
    CerrLocker(const CerrLocker &that) : lock_(cerrMutex_) /* , ias_(std::cerr) */ {}
    CerrLocker() : lock_(cerrMutex_) /* , ias_(std::cerr) */ {}
    operator bool () const {return false;}
};

std::ostream & operator << (std::ostream &os, const ThreadTimeStamp &);

} // namespace common

#endif // #ifndef COMMON_TIME_STAMP_HH

