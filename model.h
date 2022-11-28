#include "reflection/reflection.h"
#include <string>
#include <vector>

class Clip {
    UNFREEZE_NON_PUBLIC_FILED(Clip)

public:
    Clip() = default;
    Clip(std::string id): mId(id), mFilePath("/sdcard/Android/data/com.ss.android.ugc/files/test.mp4") {}

private:
    std::string mId;
    std::string mFilePath;
};

class Track {
    UNFREEZE_NON_PUBLIC_FILED(Track)

public:
    Track(std::string id): mId(id) {}


private:
    std::string mId;
    std::vector<std::shared_ptr<Clip>> mClipList;
};

class Sequence {
    UNFREEZE_NON_PUBLIC_FILED(Sequence)
public:
    Sequence(std::string id): mId(id) {}

private:
    std::string mId;
    std::vector<std::shared_ptr<Track>> mTrackList;
};