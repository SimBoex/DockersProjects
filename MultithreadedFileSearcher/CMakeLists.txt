        # Recommended: Set minimum CMake version. Using 3.15 to support C++17 filesystem.
        cmake_minimum_required(VERSION 3.15)

        # Project name. Specify CXX to indicate a C++ project.
        project(MultithreadedFileSearcher CXX)

        # Recommended: Set and enforce C++17 standard.
        set(CMAKE_CXX_STANDARD 17)
        set(CMAKE_CXX_STANDARD_REQUIRED ON)
        set(CMAKE_CXX_EXTENSIONS OFF) # Prefer standard-compliant C++

        # Recommended: Add your common include directory so your code can find headers.
        include_directories(include)

        # Recommended: Find the Threads package. Essential for multi-threading (ThreadPool).
        find_package(Threads REQUIRED)

        # Recommended: Enable testing for the entire project. This prepares CTest.
        enable_testing()

        # ----------------------------------------------------
        # Recommended: Integrate Google Test from a local subdirectory.
        # This assumes the 'googletest' folder will be in your project root after 'git clone'.
        add_subdirectory(googletest)
        # After this, GTest targets (like GTest::gtest, GTest::gtest_main) are available.
        # ----------------------------------------------------


        # ----------------------------------------------------
        # Recommended: Define your core application library.
        # Grouping shared source files into a library makes it reusable for both your
        # main application and your unit tests, avoiding code duplication.
        set(CORE_LIB_SRCS
            src/file_searcher.cpp
            # Uncomment these lines BELOW once you have complete code for them:
            src/thread_pool.cpp
            src/result_aggregator.cpp
        )
        add_library(core_lib STATIC ${CORE_LIB_SRCS})

        # Link your core library with necessary system libraries.
        # Threads::Threads for multi-threading (used by ThreadPool).
        # stdc++fs for C++17 filesystem (used by FileSearcher).
        #target_link_libraries(core_lib PRIVATE Threads::Threads stdc++fs)

        # On macOS, do NOT link stdc++fs
        target_link_libraries(core_lib PRIVATE Threads::Threads)

        # ----------------------------------------------------


        # ----------------------------------------------------
        # Recommended: Define your Main Application Executable (if you have app/main.cpp).
        # This section is commented out by default. If you have an `app/main.cpp`
        # that represents your primary application, uncomment and adjust.
        add_executable(multithreaded_file_searcher_app app/main.cpp)
        target_link_libraries(multithreaded_file_searcher_app PRIVATE
            core_lib       # Link against your core library
        )
        # ----------------------------------------------------


        # --- START: ADD THIS BLOCK FOR MACOS .APP BUNDLE ---
        if (APPLE)
            # Set the MACOSX_BUNDLE property for your executable
            # This is the CRUCIAL line to make CMake build a .app bundle
            set_target_properties(multithreaded_file_searcher_app PROPERTIES
             MACOSX_BUNDLE TRUE
             MACOSX_BUNDLE_BUNDLE_NAME "Multithreaded File Searcher" # <--- ADD THIS LINE
             MACOSX_BUNDLE_GUI_IDENTIFIER "com.yourcompany.multithreadedfilesearcher" # <--- ADD THIS LINE (use your own unique identifier) 
             )

            # Optional: Set an icon for your .app bundle
            # If you have an icon.icns file, place it in your source directory
            # For example, if it's in 'app/icon.icns'
            # set_target_properties(multithreaded_file_searcher_app PROPERTIES MACOSX_BUNDLE_ICON_FILE "icon.icns")
            # set_source_files_properties(app/icon.icns PROPERTIES MACOSX_BUNDLE_COPY TRUE) # Ensure it's copied

            # Include resources/data folder into the .app bundle
            # This assumes your 'data' folder is in your source directory (e.g., project_root/data)
            # If it's *generated* in the build folder, you might need a different approach (e.g., target_link_libraries for data or add_custom_command)
            # For now, let's assume 'data' is a source directory
            install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/data"
                    DESTINATION "${CMAKE_INSTALL_BINDIR}/multithreaded_file_searcher_app.app/Contents/Resources"
                    COMPONENT Runtime
                    FILES_MATCHING PATTERN "*"
                    #EXCLUDE ".DS_Store" # Exclude macOS specific files
                    )

            # Ensure the executable itself is correctly installed into the bundle structure
            install(TARGETS multithreaded_file_searcher_app
                    BUNDLE DESTINATION "${CMAKE_INSTALL_PREFIX}"
                    COMPONENT Runtime
                    )

        endif()
        # --- END: ADD THIS BLOCK ---

        # ----------------------------------------------------
        # Recommended: Add the Test Subdirectory.
        # ... (rest of your CMakeLists.txt)

        # ----------------------------------------------------
        # Recommended: Add the Test Subdirectory.
        # This tells CMake to go into the 'test' folder and process its CMakeLists.txt.
        # It ensures your tests are built as part of the overall project.
        add_subdirectory(test)
        # ----------------------------------------------------


        # ----------------------------------------------------
        # Recommended: Set output directories for cleaner build artifacts.
        # This puts executables (like test_search) into 'build/bin', and libraries
        # into 'build/lib', instead of scattering them through the build tree.
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
        set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
        set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
        # ----------------------------------------------------