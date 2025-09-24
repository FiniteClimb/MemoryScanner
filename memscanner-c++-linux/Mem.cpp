#include "Mem.hpp"

#include <sys/uio.h>

void Mem::ReadAllMem(vector<MapRegion> &Maps, const int pid) {
  struct iovec MapioVec[Maps.size()];
  struct iovec MemioVec[Maps.size()];
  for (unsigned long i = 0; i < Maps.size(); ++i) {
    MapioVec[i].iov_base = Maps[i].readbuff.data();
    MapioVec[i].iov_len = Maps[i].size;

    MemioVec[i].iov_base = reinterpret_cast<void *>(Maps[i].start);
    MemioVec[i].iov_len = Maps[i].size;
  }

  signed long read =
      process_vm_readv(pid, MapioVec, Maps.size(), MemioVec, Maps.size(), 0);

  printf("READ AMOUNT=%li\n", read);

  if (read == -1) {
    perror("proccess_vm_readv failed");
  }
}