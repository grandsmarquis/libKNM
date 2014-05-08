libKNM
======

libKNM is a C++ library, useful for common operations. The two main ideas behind libKNM are:
- Have a an [easy-to-use][1] C++ library, available anywhere
- Peek only what you need in the library (limit class cross dependencies)

###### Tools ######
- Threads, process, mutex and semaphores
- Configuration files and parsing operations
- Tree containers and tree based object (registry)
- Sockets API and network interfaces control
- File system operations (copy, delete, change mode...)
- Dynamic libraies and C++ class modules (.so/.dll) management
- Log file interface and time manipulation

###### Usage ######
The libray is build as a .so dynamic libray (GNU/Linux only).
- Headers are installed in `/usr/local/include/libKNM`
- Libray is installed at `/usr/local/lib/`, as `libKNM.so` which is a link to `libKNM.so.X.Y.Z`

Currently, libKNM is only available under GNU/Linux. However, a multi-os version is on the road.

###### Origin ######
libKNM has been started some months ago, not as a library but as a collection of class designed to works under GNU/Linux for a big research project: [OCADrone](http://ocadrone.com).




[1]: [Boost](http://www.boost.org/) is very complete, maybe too much complete.
