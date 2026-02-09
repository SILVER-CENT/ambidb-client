#include <gtest/gtest.h>
#include "../src/app.h"

TEST(AppTest, InitialState) {
    ambidb::App app;
    EXPECT_FALSE(app.ShouldClose());
}

// We can't easily test the Update() loop without a real ImGui context,
// but we can verify the App object can be instantiated.
