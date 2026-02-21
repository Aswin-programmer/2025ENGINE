if(EXISTS "C:/Aswin_Game_DEV/OPENGL_TUTORIAL/GetIntoGameDevOPENGL/2025ENGINE/2025ENGINE/build/Debug/streamtests.exe")
  if(NOT EXISTS "C:/Aswin_Game_DEV/OPENGL_TUTORIAL/GetIntoGameDevOPENGL/2025ENGINE/2025ENGINE/build/DEPENDENCIES/KTX/tests/streamtests/streamtests[1]_tests.cmake" OR
     NOT "C:/Aswin_Game_DEV/OPENGL_TUTORIAL/GetIntoGameDevOPENGL/2025ENGINE/2025ENGINE/build/DEPENDENCIES/KTX/tests/streamtests/streamtests[1]_tests.cmake" IS_NEWER_THAN "C:/Aswin_Game_DEV/OPENGL_TUTORIAL/GetIntoGameDevOPENGL/2025ENGINE/2025ENGINE/build/Debug/streamtests.exe" OR
     NOT "C:/Aswin_Game_DEV/OPENGL_TUTORIAL/GetIntoGameDevOPENGL/2025ENGINE/2025ENGINE/build/DEPENDENCIES/KTX/tests/streamtests/streamtests[1]_tests.cmake" IS_NEWER_THAN "${CMAKE_CURRENT_LIST_FILE}")
    include("C:/Program Files/CMake/share/cmake-4.0/Modules/GoogleTestAddTests.cmake")
    gtest_discover_tests_impl(
      TEST_EXECUTABLE [==[C:/Aswin_Game_DEV/OPENGL_TUTORIAL/GetIntoGameDevOPENGL/2025ENGINE/2025ENGINE/build/Debug/streamtests.exe]==]
      TEST_EXECUTOR [==[]==]
      TEST_WORKING_DIR [==[C:/Aswin_Game_DEV/OPENGL_TUTORIAL/GetIntoGameDevOPENGL/2025ENGINE/2025ENGINE/build/DEPENDENCIES/KTX/tests/streamtests]==]
      TEST_EXTRA_ARGS [==[C:/Aswin_Game_DEV/OPENGL_TUTORIAL/GetIntoGameDevOPENGL/2025ENGINE/2025ENGINE/DEPENDENCIES/KTX/tests/testimages/]==]
      TEST_PROPERTIES [==[]==]
      TEST_PREFIX [==[streamtest.]==]
      TEST_SUFFIX [==[]==]
      TEST_FILTER [==[]==]
      NO_PRETTY_TYPES [==[FALSE]==]
      NO_PRETTY_VALUES [==[FALSE]==]
      TEST_LIST [==[streamtests_TESTS]==]
      CTEST_FILE [==[C:/Aswin_Game_DEV/OPENGL_TUTORIAL/GetIntoGameDevOPENGL/2025ENGINE/2025ENGINE/build/DEPENDENCIES/KTX/tests/streamtests/streamtests[1]_tests.cmake]==]
      TEST_DISCOVERY_TIMEOUT [==[20]==]
      TEST_DISCOVERY_EXTRA_ARGS [==[]==]
      TEST_XML_OUTPUT_DIR [==[]==]
    )
  endif()
  include("C:/Aswin_Game_DEV/OPENGL_TUTORIAL/GetIntoGameDevOPENGL/2025ENGINE/2025ENGINE/build/DEPENDENCIES/KTX/tests/streamtests/streamtests[1]_tests.cmake")
else()
  add_test(streamtests_NOT_BUILT streamtests_NOT_BUILT)
endif()
