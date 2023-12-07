 #!/bin/bash

#  BUILD
/bin/bash ./deps/build/build.sh macos_arm64
# /bin/bash ./deps/build/build.sh raspberry-pi-os_armv8

# TESTS
# /Users/ovidiu.dragoi/my/cppserver/_build/macos_arm64/t/Debug/ini.t 

# RUN
# /Users/ovidiu.dragoi/my/cppserver/_build/macos_arm64/bin/Debug/basic_server /Users/ovidiu.dragoi/my/cppserver/src/resource/config.ini 
/Users/ovidiu.dragoi/my/cppserver/_build/macos_arm64/t/ini.t
/Users/ovidiu.dragoi/my/cppserver/_build/macos_arm64/bin/basic_websocket /Users/ovidiu.dragoi/my/cppserver/src/resource/config.ini 
