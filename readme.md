# a tiny memory leak Sanitizer

- overload global new and delete function
- cant detect memory allocate using malloc and delete
- using std::unordered_map 
- replace std::allocator  with __gnu_cxx::malloc_allocator  
- to make stacktrace symbol demangled append -rdynamic wiht g++  like `g++ -rdynamic `

- build the project with

sh build.sh 

- install with
sh build.sh install 
- unistanll
xargs rm < install_manifest.txt