# PARALLEL SYSTEMS -- PAGERANK IMPLEMENTATION

This is my pagerank implementation for the extra project of the "Parallel Systems" course of the Computer Engineering Masters of NKUA.

## HOW TO BUILD
This project has two versions: A C++ version and a Zig version. Building both is easy.

### The C++ version
Just use one of the script files provided in the `scripts/` directory. You need CMake to build this project.

>[!NOTE]
> The build scripts by default assume that if you have Ninja installed, you will want to use *it* instead of Make. Use `--no-weeb` to override this assumption

### The Zig version 
Just run `zig build`. 

>[!WARNING] 
> This project has been tested to compile successfully only on development version 0.15. I cannot guarantee that it will be able to compile for previous versions.
