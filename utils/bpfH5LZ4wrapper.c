/***************************************************************************
 *   Copyright (c) 2024-present Bitplane AG Zuerich                        *
 *                                                                         *
 *   Licensed under the Apache License, Version 2.0 (the "License");       *
 *   you may not use this file except in compliance with the License.      *
 *   You may obtain a copy of the License at                               *
 *                                                                         *
 *       http://www.apache.org/licenses/LICENSE-2.0                        *
 *                                                                         *
 *   Unless required by applicable law or agreed to in writing, software   *
 *   distributed under the License is distributed on an "AS IS" BASIS,     *
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or imp   *
 *   See the License for the specific language governing permissions and   *
 *   limitations under the License.                                        *
 ***************************************************************************/


#include <H5PLextern.h>

#include <stdint.h>

#define H5PLget_plugin_type H5PLget_plugin_type_lz4
#define H5PLget_plugin_info H5PLget_plugin_info_lz4

#if defined(_WIN32)
#define _WINSOCK2API_
#else
#define	_ARPA_INET_H_
#ifdef htonl
#undef htonl
#endif
#ifdef ntohl
#undef ntohl
#endif
#endif

static uint32_t htonl(uint32_t hostlong)
{
  return ((hostlong >> 24) & 0xff) | ((hostlong >> 8) & 0xff00) | ((hostlong & 0xff00) << 8) | ((hostlong & 0xff) << 24);
}

static uint32_t ntohl(uint32_t netlong)
{
  return htonl(netlong);
}

#if (defined(__GNUC__) || defined(__GNUG__)) && !defined(__clang__)
#pragma GCC system_header

#elif defined _MSC_VER
#pragma warning(push, 1)
#pragma warning(disable:4102 4389)

#elif defined(__clang__)
#pragma clang diagnostic push
#pragma GCC diagnostic ignored "-Wmacro-redefined"
#pragma GCC diagnostic ignored "-Wcast-qual"
#pragma GCC diagnostic ignored "-Wunused-label"
#pragma GCC diagnostic ignored "-Wsign-compare"

#endif

//#include "H5Zlz4.c"
/*
 * This file is an example of an HDF5 filter plugin.
 * The plugin can be used with the HDF5 library vesrion 1.8.11+ to read
 * HDF5 datasets compressed with lz4.
 */

#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#if defined(_WIN32)
#include <Winsock2.h>
#else
#include <arpa/inet.h>
#endif
#include <H5PLextern.h>
#include <lz4.h>

static size_t H5Z_filter_lz4(unsigned int flags, size_t cd_nelmts,
        const unsigned int cd_values[], size_t nbytes,
        size_t *buf_size, void **buf);

#define H5Z_FILTER_LZ4 32004

#define htonll(x) ( ( (uint64_t)(htonl( (uint32_t)((x << 32) >> 32)))<< 32) | htonl( ((uint32_t)(x >> 32)) ))
#define ntohll(x) htonll(x)

#define htobe16t(x) htons(x)
#define htobe32t(x) htonl(x)
#define htobe64t(x) htonll(x)
#define be16toht(x) ntohs(x)
#define be32toht(x) ntohl(x)
#define be64toht(x) ntohll(x)


#define DEFAULT_BLOCK_SIZE 1<<30; /* 1GB. LZ4 needs blocks < 1.9GB. */

const H5Z_class2_t H5Z_LZ4[1] = {{
        H5Z_CLASS_T_VERS,       /* H5Z_class_t version */
        (H5Z_filter_t)H5Z_FILTER_LZ4,         /* Filter id number             */
        1,              /* encoder_present flag (set to true) */
        1,              /* decoder_present flag (set to true) */
        "HDF5 lz4 filter; see http://www.hdfgroup.org/services/contributions.html",
        /* Filter name for debugging    */
        NULL,                       /* The "can apply" callback     */
        NULL,                       /* The "set local" callback     */
        (H5Z_func_t)H5Z_filter_lz4,         /* The actual filter function   */
}};

H5PL_type_t   H5PLget_plugin_type(void) {return H5PL_TYPE_FILTER;}
const void *H5PLget_plugin_info(void) {return H5Z_LZ4;}

static size_t H5Z_filter_lz4(unsigned int flags, size_t cd_nelmts,
        const unsigned int cd_values[], size_t nbytes,
        size_t *buf_size, void **buf)
{
    void * outBuf = NULL;
    size_t ret_value;

    if (flags & H5Z_FLAG_REVERSE)
    {
        uint32_t *i32Buf;
        uint32_t blockSize;
        char *roBuf;   /* pointer to current write position */
        uint64_t decompSize;
        const char* rpos = (char*)*buf; /* pointer to current read position */
        const uint64_t * const i64Buf = (uint64_t *) rpos;
        const uint64_t origSize = (uint64_t)(be64toht(*i64Buf));/* is saved in be format */
        rpos += 8; /* advance the pointer */

        i32Buf = (uint32_t*)rpos;
        blockSize = (uint32_t)(be32toht(*i32Buf));
        rpos += 4;
        if(blockSize>origSize)
            blockSize = origSize;

        if (NULL==(outBuf = malloc(origSize)))
        {
            printf("cannot malloc\n");
            goto error;
        }
        roBuf = (char*)outBuf;   /* pointer to current write position */
        decompSize     = 0;
        /// start with the first block ///
        while(decompSize < origSize)
        {
            uint32_t compressedBlockSize;  /// is saved in be format

            if(origSize-decompSize < blockSize) /* the last block can be smaller than blockSize. */
                blockSize = origSize-decompSize;
            i32Buf = (uint32_t*)rpos;
            compressedBlockSize =  be32toht(*i32Buf);  /// is saved in be format
            rpos += 4;
            if(compressedBlockSize == blockSize) /* there was no compression */
            {
                memcpy(roBuf, rpos, blockSize);
            }
            else /* do the decompression */
            {
#if LZ4_VERSION_NUMBER > 10300
                int compressedBytes = LZ4_decompress_fast(rpos, roBuf, blockSize);
#else
                int compressedBytes = LZ4_uncompress(rpos, roBuf, blockSize);
#endif
                if(compressedBytes != compressedBlockSize)
                {
                    printf("decompressed size not the same: %d, != %d\n", compressedBytes, compressedBlockSize);
                    goto error;
                }
            }

            rpos += compressedBlockSize;   /* advance the read pointer to the next block */
            roBuf += blockSize;            /* advance the write pointer */
            decompSize += blockSize;
        }
        free(*buf);
        *buf = outBuf;
        outBuf = NULL;
        ret_value = (size_t)origSize;  // should always work, as orig_size cannot be > 2GB (sizeof(size_t) < 4GB)
    }
    else /* forward filter */
    {
        size_t blockSize;
        size_t nBlocks;
        size_t outSize; /* size of the output buffer. Header size (12 bytes) is included */
        size_t space;
        size_t block;
        uint64_t *i64Buf;
        uint32_t *i32Buf;
        char *rpos;      /* pointer to current read position */
        char *roBuf;    /* pointer to current write position */

        if (nbytes > INT32_MAX)
        {
            /* can only compress chunks up to 2GB */
            goto error;
        }

        if(cd_nelmts > 0 && cd_values[0] > 0)
        {
            blockSize = cd_values[0];
        }
        else
        {
            blockSize = DEFAULT_BLOCK_SIZE;
        }
        if(blockSize > nbytes)
        {
            blockSize = nbytes;
        }
        nBlocks = (nbytes-1)/blockSize +1;
        space = LZ4_COMPRESSBOUND(nbytes);
        if (NULL==(outBuf = malloc(space
                + 4+8 + nBlocks*4)))
        {
            goto error;
        }

        rpos  = (char*)*buf;      /* pointer to current read position */
        roBuf = (char*)outBuf;    /* pointer to current write position */
        /* header */
        i64Buf = (uint64_t *) (roBuf);
        i64Buf[0] = htobe64t((uint64_t)nbytes); /* Store decompressed size in be format */
        roBuf += 8;

        i32Buf =  (uint32_t *) (roBuf);
        i32Buf[0] = htobe32t((uint32_t)blockSize); /* Store the block size in be format */
        roBuf += 4;

        outSize = 12; /* size of the output buffer. Header size (12 bytes) is included */

        for(block = 0; block < nBlocks; ++block)
        {
            uint32_t compBlockSize; /// reserve space for compBlockSize
            size_t origWritten = block*blockSize;
            if(nbytes - origWritten < blockSize) /* the last block may be < blockSize */
                blockSize = nbytes - origWritten;

#if LZ4_VERSION_NUMBER > 10300
            compBlockSize = LZ4_compress_default(rpos, roBuf+4, blockSize, space); /// reserve space for compBlockSize
#else
            compBlockSize = LZ4_compress(rpos, roBuf+4, blockSize); /// reserve space for compBlockSize
#endif
            if(!compBlockSize)
                goto error;
            if(compBlockSize >= blockSize) /* compression did not save any space, do a memcpy instead */
            {
                compBlockSize = blockSize;
                memcpy(roBuf+4, rpos, blockSize);
            }

            i32Buf =  (uint32_t *) (roBuf);
            i32Buf[0] = htobe32t((uint32_t)compBlockSize);  /* write blocksize */
            roBuf += 4;

            rpos += blockSize;     	/* advance read pointer */
            roBuf += compBlockSize;       /* advance write pointer */
            outSize += compBlockSize + 4;
            space -= compBlockSize;
        }

        free(*buf);
        *buf = outBuf;
        *buf_size = outSize;
        outBuf = NULL;
        ret_value = outSize;

    }
    done:
    if(outBuf)
        free(outBuf);
    return ret_value;


    error:
    if(outBuf)
        free(outBuf);
    outBuf = NULL;
    return 0;
}


#if defined (_MSC_VER)
#pragma warning(pop)
#endif
