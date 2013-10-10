BasicHTTP
=========

HTTP Server that is: Simple - Stupid - Static - Self-Sufficient - Some-other-things

Clone:

  git clone git@github.com:/scunz/BasicHTTP
  git submodule init
  git submodule update

Compile:

  mkdir bin
  cd bin
  cmake .. -DQt5=[On|Off]
  make

Run:

  bin/bin/bHTTP /where/my/files/are 1234


Test:

  curl http://localhost:1234/testfile.txt
