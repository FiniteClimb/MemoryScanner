#include "Mem.hpp"

#include <sys/uio.h>

void Mem::ReadAllMem(vector<MapRegion> &Maps) {
  struct iovec MapioVec[Maps.size()];
  struct iovec MemioVec[Maps.size()];
  for (unsigned long i = 0; i < Maps.size(); ++i) {
    MapioVec[i].iov_base = Maps[i].readbuff.data();
    MapioVec[i].iov_len = Maps[i].size;

    MemioVec[i].iov_base = reinterpret_cast<void *>(Maps[i].start);
    MemioVec[i].iov_len = Maps[i].size;
  }

  long read =
      process_vm_readv(pidnum, MapioVec, Maps.size(), MemioVec, Maps.size(), 0);

  printf("READ AMOUNT=%li\n", read);

  if (read == -1) {
    perror("proccess_vm_readv failed");
  }
}

void Mem::WriteValue(vector<unsigned char> WRITE_VALUE,
                     unsigned long &WRITE_ADDRESS) {
  iovec WriteThis;
  WriteThis.iov_base = WRITE_VALUE.data();
  WriteThis.iov_len = WRITE_VALUE.size();
  cout << " write value" << WRITE_VALUE.size() << "\n\n";

  iovec WriteTo;
  WriteTo.iov_base = reinterpret_cast<void *>(WRITE_ADDRESS);
  WriteTo.iov_len = WRITE_VALUE.size();

  cout << "\n\n";
  for (int i = 0; i < WriteThis.iov_len; ++i) {
    printf("%02X ", *(static_cast<unsigned char *>(WriteThis.iov_base) + i));
  }
  cout << "\n\n";

  long written = process_vm_writev(pidnum, &WriteThis, 1, &WriteTo, 1, 0);

  if (written == -1)
    perror("proccess_vm_write failed");
}