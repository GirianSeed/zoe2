#ifndef __ZOE2_LIBFS_H__
#define __ZOE2_LIBFS_H__

#include "common.h"

typedef struct {
    char *name;             /* +0x00 */
    uint32 pos;             /* +0x04 */
    unsigned char media;    /* +0x08 */
    unsigned char layer;    /* +0x09 */
    unsigned char alt;      /* +0x0a */
    unsigned char pad;      /* +0x0b */
} FS_FILE_INFO;

extern FS_FILE_INFO fs_file_info[]; /* in file.cnf */

#define FS_FILEID_STAGE         (0)     // stage.dat
#define FS_FILEID_MOVIE         (1)     // movie.dat
#define FS_FILEID_VOX           (2)     // vox.dat
#define FS_FILEID_DEMO          (3)     // demo.dat
#define FS_MAX_FILEID           (4)

// #include "strcode.h"
// inline uint32 FS_StrCode( const char *str );

#endif // {{{ END OF FILE }}}
