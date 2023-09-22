# Speex component for ESP-IDF

https://www.speex.org/

Version: 1.2.1

## Speex: A Free Codec For Free Speech

Speex is an Open Source/Free Software patent-free audio compression
format designed for speech. The Speex Project aims to lower the barrier
of entry for voice applications by providing a free alternative to
expensive proprietary speech codecs. Moreover, Speex is well-adapted to
Internet applications and provides useful features that are not present
in most other codecs. Finally, Speex is part of the GNU Project and is
available under the revised BSD license.

## How to use

Clone this repository into `components` subdir of yout project:

```Shell
cd ~/my_esp_idf_project
mkdir components
cd components
git clone git@github.com:UncleRus/esp-idf-speex.git
```

Or clone it to any other directory and add it to your project `Makefile` or `CMakeLists.txt`:

```Makefile
PROJECT_NAME := my-esp-project
EXTRA_COMPONENT_DIRS := $(HOME)/my/work/path/esp-idf-speex
include $(IDF_PATH)/make/project.mk
```

```cmake
cmake_minimum_required(VERSION 3.5)
set(EXTRA_COMPONENT_DIRS $ENV{HOME}/my/work/path/esp-idf-speex)
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(my-esp-project)
```

See examples.

