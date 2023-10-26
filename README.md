# Kompute Test

Just a hello world of the Kompute library. This library is something like an abstraction layer around vulkan that gives reasonable access to vulkan compute to non-graphical applications. Manually integrating vulkan for this very reason was taking me days, but setting this up and reworking this example took an hour! Kompute also seems very promising for allowing granular performance tuning. 

## Compiling example

The vulkan developer software will need to be present on the machine. Fortunately this is just a process of running an installer from: https://vulkan.lunarg.com/sdk/home

After that is installed, git this repo recursively (kompute is added as a submod) and do the usual cmake build routine. 