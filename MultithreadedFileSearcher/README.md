1. installing docker:
sudo docker build -t cpp_dev_env .

2.  build:
sudo docker build -t cpp_dev_env .

3. mount files:
docker run -it --rm -v "$(pwd):/app" cpp_dev_env

4. dependencies:
brew install googletest

5. git submodule (after having in the git root, otherwise donwload it)
git submodule add https://github.com/google/googletest.git
git submodule update --init --recursive # Ensures all sub-submodules are also fetched


7. build
rm -rf build
mkdir build
cd build
cmake ..
make

8. to run:
./multithreaded_file_searcher_app

9. add GitHub actions
