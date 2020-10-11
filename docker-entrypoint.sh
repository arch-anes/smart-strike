#!/bin/sh

if [ -n "$DEVICE_AUTH_FILE" ]; then export DEVICE_AUTH="$(cat $DEVICE_AUTH_FILE)"; fi;
if [ -n "$JWT_SECRET_FILE" ]; then export JWT_SECRET="$(cat $JWT_SECRET_FILE)"; fi;
if [ -n "$APP_PASS_FILE" ]; then export APP_PASS="$(cat $APP_PASS_FILE)"; fi;

node dist/main
