#include "FileIO.h"
#include <Updater.h>

class UpdaterISPPFS {
private:
  void performUpdate(Stream& updateSource, size_t updateSize);

public:
  bool updateFromFS(FileIO** fileIO);
};
