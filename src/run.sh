#!/bin/sh
 broadwayd -p 80 :0&
 BWD_PID=$!
 GDK_BACKEND=broadway BROADWAY_DISPAY=:0 ./gtk.app 
 kill ${BWD_PID}