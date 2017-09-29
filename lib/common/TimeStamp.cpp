#include "common/TimeStamp.hh"

#include <boost/date_time/posix_time/posix_time.hpp>

namespace common {
std::recursive_mutex CerrLocker::cerrMutex_;

std::ostream & operator << (std::ostream &os, const ThreadTimeStamp &)
{
    const auto now = std::chrono::high_resolution_clock::now();
    const auto duration = now.time_since_epoch();
    const auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    const auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration).count();
    const auto minutes = std::chrono::duration_cast<std::chrono::minutes>(duration).count();
    const auto hours = std::chrono::duration_cast<std::chrono::hours>(duration).count();
    return os << "[" << std::setfill('0')
              << std::setw(16) << std::this_thread::get_id() << " - "
              << std::setw(2) << (hours % 24) << ":"
              << std::setw(2) << (minutes % 60) << ":"
              << std::setw(2) << (seconds % 60) << "."
              << std::setw(3) << (millis % 1000)  << "]\t";
}

} // namespace common
