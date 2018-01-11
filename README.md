# Smart-Lane-Marker
Smart Lane Market module
Inside of the package have 2 files

1. PsuedoCarmodule_testsuccessrate_20180110.ino
This is a car module code
require HC-12 module. The pin connection is include in the .iso file

Function: Send out <CAR> signal by baut rate at 4800 baut rate.
                Open Serial terminal at 4800 baut rate.
                Be able to print the 4800 baut rate signal when recieve it

2. SmartlanemarkerV1.0.3.ino
This is smart lane marker module
require HC-12 module. The pin connection is include in the .iso file
Function: Be able to detect the <CAR> signal on baut rate at 4800 baut rate.
                Boradcast lane marker information 10 times.
