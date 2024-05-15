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


#include "ImarisReader/utils/bpfH5LZ4.h"

#include <H5PLextern.h>


const H5Z_filter_t H5Z_FILTER_LZ4 = 32004;


extern "C" {

  const void *H5PLget_plugin_info_lz4(void);

}


static htri_t H5Zregister_lz4_impl()
{
  if (H5Zfilter_avail(H5Z_FILTER_LZ4) > 0) {
    // already dynamically loaded?
    // https://support.hdfgroup.org/HDF5/doc/Advanced/DynamicallyLoadedFilters/HDF5DynamicallyLoadedFilters.pdf
    return 1;
  }
  if (H5Zregister(H5PLget_plugin_info_lz4()) < 0) {
    return -1;
  }
  return H5Zfilter_avail(H5Z_FILTER_LZ4);
}


htri_t H5Zregister_lz4()
{
  static htri_t vRegistered = H5Zregister_lz4_impl();
  return vRegistered;
}


herr_t H5Pset_lz4(hid_t aPListId, unsigned int aBlockSize)
{
  if (H5Zregister_lz4() < 0) {
    return -1;
  }
  return H5Pset_filter(aPListId, H5Z_FILTER_LZ4, H5Z_FLAG_MANDATORY, 1, &aBlockSize);
}

