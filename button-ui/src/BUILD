cc_library(
    name = "button",
    srcs = ["button.cpp"],
    hdrs = ["hdr/button.hpp"],
    deps = [
        "@opencv//:opencv",
    ],
)


cc_binary(
    name = "test-button",
    srcs = ["TestButton.cpp"],
    deps = [
        ":button",
        "@opencv//:opencv",
    ],
)

