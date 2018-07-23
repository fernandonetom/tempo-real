#! /bin/bash

./unix-server server-socket &
SERVERPID=$!

./unix-client server-socket <<EOF
lucas
vinicius
hartmann
END
EOF

kill $SERVERPID
rm -f server-socket
