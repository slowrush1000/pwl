/**
 * @file pwl_runtime.cpp
 * @author Cheon Younghoe (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-04-02
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "pwl_runtime.hpp"
#include <fmt/printf.h>
#include <sys/time.h>
#include <sys/resource.h>

pwl::Runtime::Runtime()
{
    mCPUTime    = std::clock();
    mWallTime   = std::chrono::high_resolution_clock::now();
}

pwl::Runtime::~Runtime()
{
}

double          
pwl::Runtime::getCPUTimeSecs()
{
    return double(std::clock() - mCPUTime)/double(CLOCKS_PER_SEC);
}

double          
pwl::Runtime::getWallTimeSecs()
{
    return std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - mWallTime).count();
}

std::size_t     
pwl::Runtime::getPeakRSSBytes()
{
#if defined(_WIN32)
	/* Windows -------------------------------------------------- */
	PROCESS_MEMORY_COUNTERS info;
	GetProcessMemoryInfo( GetCurrentProcess( ), &info, sizeof(info) );
	return std::size_t(info.PeakWorkingSetSize);

#elif (defined(_AIX) || defined(__TOS__AIX__)) || (defined(__sun__) || defined(__sun) || defined(sun) && (defined(__SVR4) || defined(__svr4__)))
	/* AIX and Solaris ------------------------------------------ */
	struct psinfo psinfo;
	int fd = -1;
	if ( (fd = open( "/proc/self/psinfo", O_RDONLY )) == -1 )
        return std::size_t(0L);         /* Can't read? */
    if ( read( fd, &psinfo, sizeof(psinfo) ) != sizeof(psinfo) )
    {
        close( fd );
        return std::size_t(0L);         /* Can't read? */
    }
    close( fd );
    return std::size_t(psinfo.pr_rssize * 1024L);

#elif defined(__unix__) || defined(__unix) || defined(unix) || (defined(__APPLE__) && defined(__MACH__))
    /* BSD, Linux, and OSX -------------------------------------- */
    struct rusage rusage;
    getrusage( RUSAGE_SELF, &rusage );
    #if defined(__APPLE__) && defined(__MACH__)
        return std::size_t(rusage.ru_maxrss);
    #else
        return std::size_t(rusage.ru_maxrss * 1024L);
    #endif

#else
    /* Unknown OS ----------------------------------------------- */
    return std::size_t(0L);             /* Unsuppored. */

#endif
}

std::size_t     
pwl::Runtime::getCurrentRSSBytes()
{
#if defined(_WIN32)
	/* Windows -------------------------------------------------- */
	PROCESS_MEMORY_COUNTERS info;
	GetProcessMemoryInfo( GetCurrentProcess( ), &info, sizeof(info) );
    return std::size_t(info.PeakWorkingSetSize);

#elif (defined(_AIX) || defined(__TOS__AIX__)) || (defined(__sun__) || defined(__sun) || defined(sun) && (defined(__SVR4) || defined(__svr4__)))
	/* AIX and Solaris ------------------------------------------ */
	struct psinfo psinfo;
	int fd = -1;
	if ( (fd = open( "/proc/self/psinfo", O_RDONLY )) == -1 )
		return std::size_t(0L);      /* Can't open? */
    if ( read( fd, &psinfo, sizeof(psinfo) ) != sizeof(psinfo) )
    {
        close( fd );
        return std::size_t(0L);      /* Can't read? */
    }
    close( fd );
    return std::size_t(psinfo.pr_rssize * 1024L);

#elif defined(__unix__) || defined(__unix) || defined(unix) || (defined(__APPLE__) && defined(__MACH__))
    /* BSD, Linux, and OSX -------------------------------------- */
    struct rusage rusage;
    getrusage( RUSAGE_SELF, &rusage );

    #if defined(__APPLE__) && defined(__MACH__)
        return std::size_t(rusage.ru_maxrss);
    #else
        return std::size_t(rusage.ru_maxrss * 1024L);
    #endif

#else
    /* Unknown OS ----------------------------------------------- */
    return std::size_t(0L);          /* Unsupported. */
    
#endif
}

std::string     
pwl::Runtime::getRuntimeStr()
{
    std::string     runtimeStr  = fmt::sprintf("cpu %.1f secs, wall %.1f secs, peak %.1f MB, current %.1f MB", 
            this->getCPUTimeSecs(),
            this->getWallTimeSecs(),
            this->getPeakRSSBytes()/1.0e6,
            this->getCurrentRSSBytes()/1.0e6
            );

    return runtimeStr;
}
