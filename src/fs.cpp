#include <fs.h>

namespace fsbridge {

FILE *fopen( fs::path& p,  char *mode)
{
    return ::fopen(p.string().c_str(), mode);
}

FILE *freopen( fs::path& p,  char *mode, FILE *stream)
{
    return ::freopen(p.string().c_str(), mode, stream);
}

} // fsbridge
