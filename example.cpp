#include "model.h"
#include <iostream>

DEFINE_SCHEMA(Clip, 
              DEFINE_SCHEMA_FILED(mId),
              DEFINE_SCHEMA_FILED(mFilePath));

int main(int argc, char* argv[]) {
#if 0
    Clip clip("clip-0");
#else
    nlohmann::json jj = nlohmann::json::parse(R"(
        {
            "mFilePath": "/sdcard/Android/data/com.ss.android.ugc/files/test.mp4",
            "mId": "clip-2"
        }
    )");
    Clip clip;
    jj.get_to(clip);
#endif
    nlohmann::json j = nlohmann::json(clip);
    std::cout << j.dump(4) << std::endl;
    return 0;
}