#ifndef TIMER_H
#define TIMER_H

#ifdef WIN32
#include <Windows.h>
#else
#include <sys/time.h>
#endif

// CPU timer using the performance counter for WIN32 and
// gettimeofday() for Linux.
#ifdef WIN32

class CPUTimer {
public:
  CPUTimer() { QueryPerformanceFrequency(&m_frequency); }
  ~CPUTimer() {}

  void Start() { QueryPerformanceCounter(&m_start); }
  void Stop() { QueryPerformanceCounter(&m_stop); }

  double getElapsed() {
    return (m_stop.QuadPart - m_start.QuadPart) * 1000.0 / m_frequency.QuadPart;
  }

private:
  LARGE_INTEGER m_frequency;
  LARGE_INTEGER m_start;
  LARGE_INTEGER m_stop;
};

#else // WIN32

class CPUTimer : public Timer {
protected:
  timeval timeStart;
  timeval timeEnd;

public:
  virtual ~CPUTimer() {}

  virtual void Start() { gettimeofday(&timeStart, 0); }

  virtual void Stop() { gettimeofday(&timeEnd, 0); }

  virtual double getElapsed() {
    return 1000.0 * (timeEnd.tv_sec - timeStart.tv_sec) +
           (timeEnd.tv_usec - timeStart.tv_usec) / 1000.0;
  }
};

#endif // WIN32

#endif
