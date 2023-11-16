 #!/bin/bash

#  BUILD
# /bin/bash ./vendor/build/build.sh macos_arm64
/bin/bash ./vendor/build/build.sh raspberry-pi-os_armv8

# TESTS
# /Users/ovidiu.dragoi/my/cppserver/_build/macos_arm64/t/Debug/ini.t 

# RUN
# /Users/ovidiu.dragoi/my/cppserver/_build/macos_arm64/bin/Debug/basic_server /Users/ovidiu.dragoi/my/cppserver/src/resource/config.ini 