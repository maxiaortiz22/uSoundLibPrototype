import argparse
import os
import platform
from pathlib import Path

def main():
    main_dir = os.getcwd()

    # Create the build folder if it doesn't exist:
    build_dir = Path("build")
    build_dir.mkdir(exist_ok=True)

    # Parser object:
    parser = argparse.ArgumentParser(description='audioengine library compilation settings')

    # Adding building argument:
    parser.add_argument('--build', default='all', help="Compilation types: 'library', 'test', 'all'.")

    # Parse command line arguments
    args = parser.parse_args()

    if args.build == 'library':
        print('Compilation mode:', args.build)
        compile_library(main_dir)
        print('Compilation completed')

    elif args.build == 'test':
        print('Compilation mode:', args.build)
        compile_test(main_dir)
        print('Compilation completed')

    elif args.build == 'all':
        print('Compilation mode:', args.build)
        compile_library(main_dir)
        compile_test(main_dir)
        print('Compilation completed')

    else:
        raise ValueError("Invalid compilation type, try: 'library', 'test' or 'all'.")

def compile_library(main_dir):
    # Compile the library
    os.chdir(main_dir + '/build')

    if platform.system() == "Windows":
        # Use MSVC on Windows with Release mode
        os.system("cmake .. -G \"Visual Studio 17 2022\" -A x64 -DCMAKE_BUILD_TYPE=Release")
        os.system("cmake --build . --config Release")
    else:
        # Use standard Unix Makefiles for Mac/Linux
        os.system("cmake .. -G \"Unix Makefiles\" -DCMAKE_BUILD_TYPE=Release")
        os.system("cmake --build .")

    if platform.system() == "Windows":
        # Copy the library to the test folder
        os.system("copy Release\\usound_native.lib .")

def compile_test(main_dir):
    # Compile the tests
    os.chdir(main_dir + '/test')

    # Create the build folder if it doesn't exist:
    build_dir = Path("build")
    build_dir.mkdir(exist_ok=True)

    os.chdir(str(build_dir))

    if platform.system() == "Windows":
        # Use MSVC on Windows with Release mode
        os.system("cmake .. -G \"Visual Studio 17 2022\" -A x64 -DCMAKE_BUILD_TYPE=Release")
    else:
        # Use standard Unix Makefiles for Mac/Linux
        os.system("cmake .. -G \"Unix Makefiles\" -DCMAKE_BUILD_TYPE=Release")

    os.system("cmake --build . --config Release")

    if platform.system() == "Windows":
        # Copy the library to the test folder
        os.system("copy Release\\*.pyd .")
        os.system("copy Release\\*.exp .")
        os.system("copy Release\\*.lib .")

if __name__ == '__main__':
    """Cross-platform building script using GCC or MSVC"""
    main()