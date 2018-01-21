#!/bin/bash
echo "Active Boards"
ps aux | grep "$1"
echo "Inactive Boards"