#ifndef SAMPLE_SIGPIPE_HPP
#define SAMPLE_SIGPIPE_HPP

#ifndef _MSC_VER
#include <csignal>

namespace sigpipe {

class CSigpipe_scoped {
  public:
    CSigpipe_scoped();
    ~CSigpipe_scoped();

  private:
    bool m_sigpipe_pending;
    bool m_sigpipe_unblock;
};

} // namespace sigpipe

#define SCOPED_NO_SIGPIPE sigpipe::CSigpipe_scoped sigpipe;
#else
// This will the ported program also compile on win32 without error.
#define SCOPED_NO_SIGPIPE
#endif // #ifndef _MSC_VER

#endif // SAMPLE_SIGPIPE_HPP
